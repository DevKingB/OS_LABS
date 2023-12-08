#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>
#include "./Headers/list.h"
#include "./Headers/util.h"

void TOUPPER(char * arr){
  
    for(int i=0;i<strlen(arr);i++){
        arr[i] = toupper(arr[i]);
    }
}

void get_input(char *args[], int input[][2], int *n, int *size, int *policy) 
{
  	FILE *input_file = fopen(args[1], "r");
	  if (!input_file) {
		    fprintf(stderr, "Error: Invalid filepath\n");
		    fflush(stdout);
		    exit(0);
	  }

    parse_file(input_file, input, n, size);
  
    fclose(input_file);
  
    TOUPPER(args[2]);
  
    if((strcmp(args[2],"-F") == 0) || (strcmp(args[2],"-FIFO") == 0))
        *policy = 1;
    else if((strcmp(args[2],"-B") == 0) || (strcmp(args[2],"-BESTFIT") == 0))
        *policy = 2;
    else if((strcmp(args[2],"-W") == 0) || (strcmp(args[2],"-WORSTFIT") == 0))
        *policy = 3;
    else {
       printf("usage: ./mmu <input file> -{F | B | W }  \n(F=FIFO | B=BESTFIT | W-WORSTFIT)\n");
       exit(1);
    }
        
}

void allocate_memory(list_t *freelist, list_t *alloclist, int pid, int blocksize, int policy) {
    node_t *current = freelist->head;
    node_t *best_fit = NULL;
    node_t *worst_fit = NULL;

    // Iterate through the free list to find a suitable block
    while (current != NULL) {
        int current_size = current->blk->end - current->blk->start + 1;
        if (current_size >= blocksize) {
            if (policy == 1) {  // First Fit
                best_fit = current;
                break;
            } else if (policy == 2) {  // Best Fit
                if (!best_fit || current_size < best_fit->blk->end - best_fit->blk->start + 1) {
                    best_fit = current;
                }
            } else if (policy == 3) {  // Worst Fit
                if (!worst_fit || current_size > worst_fit->blk->end - worst_fit->blk->start + 1) {
                    worst_fit = current;
                }
            }
        }
        current = current->next;
    }

    node_t *selected_block = (policy == 3) ? worst_fit : best_fit;
    if (selected_block) {
        // Allocate the block
        block_t *new_block = malloc(sizeof(block_t));
        *new_block = *(selected_block->blk);  // Copy block data
        new_block->pid = pid;
        new_block->end = new_block->start + blocksize - 1;

        list_add_ascending_by_address(alloclist, new_block);

        // Handle the remaining memory (fragment)
        if (new_block->end < selected_block->blk->end) {
            block_t *fragment = malloc(sizeof(block_t));
            fragment->pid = 0;  // Free block
            fragment->start = new_block->end + 1;
            fragment->end = selected_block->blk->end;
            list_add_to_freelist(freelist, fragment, policy);  // Add back to free list
        }

        // Remove the original block from the free list
        remove_block_from_freelist(freelist, selected_block->blk);

    } else {
        printf("Error: Not Enough Memory\n");
    }
}

void deallocate_memory(list_t *alloclist, list_t *freelist, int pid, int policy) {
    block_t *block_to_deallocate = NULL;
    node_t *current = alloclist->head;
    node_t *prev = NULL;

    // Find the block with the given PID
    while (current != NULL) {
        if (current->blk->pid == pid) {
            block_to_deallocate = current->blk;
            break;
        }
        prev = current;
        current = current->next;
    }

    if (block_to_deallocate) {
        // Deallocate the block
        block_to_deallocate->pid = 0;  // Mark as free

        // Add the block back to the free list
        list_add_to_freelist(freelist, block_to_deallocate, policy);

        // Remove the block from the allocated list
        if (prev) {
            prev->next = current->next;
        } else {
            alloclist->head = current->next;
        }
        free(current->blk);
        free(current);

    } else {
        printf("Error: Can't locate Memory Used by PID: %d\n", pid);
    }
}
