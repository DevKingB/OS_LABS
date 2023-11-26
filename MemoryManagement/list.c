// list/list.c
// 
// Implementation for linked list.

/***** Necessary Headers FIles ********/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "./Headers/list.h"

/***** Function Definitions ********/

list_t *list_alloc() {
  list_t *list = malloc(sizeof(list_t));
  if (list == NULL) {
    printf("Error: list_alloc failed\n");
    exit(EXIT_FAILURE);
  }

  list->head = NULL;
  list->tail = NULL;
  list->length = 0;

  return list;
}

node_t *node_alloc(block_t *blk) {
  node_t *node = malloc(sizeof(node_t));
  if (node == NULL) {
    printf("Error: node_alloc failed\n");
    exit(EXIT_FAILURE);
  }

  node->next = NULL;
  node->blk = blk;
  return node;
}
void node_free(node_t *node) {
  if (node != NULL) {
    if (node->blk != NULL)
      free(node->blk); // Free the associated block inside the node
    free(node); // Free the node itself
  }
}

void list_free(list_t *list) {
  node_t *curr = list->head;
  node_t *next_node;

  // Free each node in the list first; then free the space allocated for the list
  while (curr != NULL) {
    next_node = curr->next;
    node_free(curr); //Use node_free to handle node and block freeing
    curr = next_node;
  }

  free(list); //free the list itself
}


void list_print(list_t *list) {
    node_t *curr = list->head;
    block_t *my_block;
    if (curr == NULL){ 
        printf("List is empty\n");
    }
    while (curr != NULL) {
        my_block = curr->blk;
        printf("Block Info: PID=%d, START=%d, end=%d\n", my_block->pid, my_block->start, my_block->end);
        curr = curr->next;  
    }
}

//? Should I just add the list_length function inside of the list structure
int list_length(list_t *list) {
    return list->length; // This is an O(1) operation
}

void list_coalese_nodes(list_t *list){
    node_t *curr = list->head;

    while (curr != NULL && curr->next != NULL) {
        if (curr->blk->end + 1 == curr->next->blk->start) {
            curr->blk->end = curr->next->blk->end;
            node_t *temp = curr->next;
            curr->next = temp->next;
            node_free(temp);
        }
        else {
            // Move to the next block if not adjacent
            curr = curr->next;
        }
    }
}

node_t* find_node_at_index(node_t* head, int index) {
    node_t *current = head;
    int count = 0;
    while (current != NULL && count < index) {
        current = current->next;
        count++;
    }
    return current;
}

/********* Function Defintiions: Adding **************/

void list_add_to_back(list_t *list, block_t *blk) {
    node_t *new_node = node_alloc(blk);
    if (list->head == NULL) { /* this is for the list being empty */
        list->head = new_node;
        list->tail = new_node; // Updating tail pointer to the end of list
    }
    else { /* this is for the list having at least one element */
        list->tail->next = new_node;
        list->tail = new_node; // Updating tail pointer to the end of list
    }
    list->length++; // Incrementing length of list
}

void list_add_to_front(list_t *list, block_t *blk) {
    node_t *new_node = node_alloc(blk);
    node_t *curr = list->head;
    if (curr == NULL) {
        list->head = new_node;
        list->tail = new_node; // Updating tail pointer to the end of list
    }
    else {
        new_node->next = curr;
        list->head = new_node;
    }
    list->length++; // Incrementing length of list
}

void list_add_at_index(list_t *list, block_t *blk, int index) {
   if (index == 0) { // Assuming linked list is 0 indexed, if index is 0, add to front
       list_add_to_front(list, blk);
   }
   else if (index >= list->length) { // Assuming linked list is 0 indexed, if index is greater than or equal to length, add to back
        list_add_to_back(list, blk);
   }
   else { // Index value is somewhere in the middle of the linked list
        node_t *new_node = node_alloc(blk);
        node_t *prev = find_node_at_index(list->head, index - 1); //Finding node before actual index value 
        new_node->next = prev->next;
        prev->next = new_node;
        list->length++; // Incrementing length of list
   }

   //? what should be return if the index is below 0? 
}

void list_add_ascending_by_address(list_t *list, block_t *newblk) {
    node_t *new_node = node_alloc(newblk);
    node_t *curr = list->head;
    node_t *prev = NULL;
    if (curr == NULL) {
        list->head = new_node;
    }
    else {
        while (curr != NULL && curr->blk->start < newblk->start) {
            prev = curr;
            curr = curr->next;
        }

        new_node->next = curr;
        if (prev == NULL) {
            list->head = new_node;
        }
        else {
            prev->next = new_node;
        }
    }
}

void list_add_ascending_by_blocksize (list_t *list, block_t *newblk) {
    node_t *new_node = node_alloc(newblk);
    node_t *curr = list->head;
    node_t *prev = NULL;
    if (curr == NULL) {
        list->head = new_node;
    }
    else {
        while (curr != NULL && ((curr->blk->end - curr->blk->start) < (newblk->end - newblk->start))) {
            prev = curr;
            curr = curr->next;
        }

        new_node->next = curr;
        if (prev == NULL) {
            list->head = new_node;
        }
        else {
            prev->next = new_node;
        }
    }
}

void list_add_descending_by_blocksize (list_t *list, block_t *newblk) {
    node_t *new_node = node_alloc(newblk);
    node_t *curr = list->head;
    node_t *prev = NULL;
    int newblk_size = newblk->end - newblk->start;

    if (curr == NULL) {
        list->head = new_node;
    }
    else {
        prev = curr;
        int curr_blk_size = curr->blk->end - curr->blk->start + 1;
        if (curr->next == NULL) { // Only one node in the list
            if (newblk_size > curr_blk_size) { //place in front of the current node
                new_node->next = curr;
                list->head = new_node;
            }
            else { //place behind the current node
                curr->next = new_node;
                new_node->next = NULL;
            }
        }
        else { //two or more nodes in the list

            if (newblk_size >= curr_blk_size) { //place in front of the current node
                new_node->next = curr;
                list->head = new_node;
            }
            else { 
                while (curr != NULL  && newblk_size < curr_blk_size) {
                    prev = curr;
                    curr = curr->next;
                    if (curr != NULL) { //The last one in the list
                        curr_blk_size = curr->blk->end - curr->blk->start + 1;
                    }
                }
                prev->next = new_node;
                new_node->next = curr;
            }
        } 
    }
}

/********* Function Defintiions: Removing **************/

block_t* list_remove_from_front(list_t *list) {
    if (list->head == NULL) {
        return NULL; // List is empty
    }
    node_t *temp = list->head;
    block_t *removed_block = temp->blk;
    list->head = list->head->next;
    if (list->head == NULL) {
        list->tail = NULL; // List became empty
    }
    free(temp); //Frees the node; not the block itself
    list->length--; // Update length
    return removed_block;
}

block_t* list_remove_from_back(list_t *list) {
    if (list->head == NULL) {
        return NULL; // List is empty
    }
    block_t *removed_block = list->tail->blk; // gets the block structure from the last node
    if (list->head == list->tail) { // Only one node in the list
        return list_remove_from_front(list);
    }
    else { //Assuming multiple nodes in the list
        node_t *node_before_tail = find_node_at_index(list->head, list->length - 2); // Finding the node before the tail
        free(list->tail);
        list->tail = node_before_tail;
        list->tail->next = NULL; // setting the next pointer to NULL
    }
    list->length--; // Update length
    return removed_block; // returning pointer to the block structure in the removed node
}

block_t* list_remove_at_index(list_t *list, int index) {
    /* Handling out of index error */
   if (index < 0 || index >= list->length || list->head == NULL) {
    printf ("The list is empty or Invalid Index\n");
    return NULL; //? what should be returned here
   }

   if (index == 0) { // Assuming linked list is 0 indexed, if index is 0, remove from front
       return list_remove_from_front(list);
   }
   else if (index == list->length - 1) { // Assuming linked list is 0 indexed, if index is greater than or equal to length, remove from back
        return list_remove_from_back(list);
   }
   else { // Index value is somewhere in the middle of the linked list
        node_t *prev = find_node_at_index(list->head, index - 1); //Finding node before actual index value 
        node_t *curr = prev->next;
        block_t *removed_block = curr->blk;
        prev->next = curr->next;
        free(curr); //Frees the node; not the block itself
        list->length--; // Update length
        return removed_block; // returning pointer to the block structure in the removed node
   }
}

/*************** Function Definitions: Comparing ***********************/

bool compare_blocks(block_t *blk1, block_t *blk2) {
    if (blk1->start == blk2->start && blk1->end == blk2->end && blk1->pid == blk2->pid) {
        return true;
    }
    return false;
}

bool compare_size(int number, block_t *blk) {
    if ((blk->end - blk->start) + 1 >= number) {
        return true;
    }
    return false;
}

bool compare_pid(int pid, block_t *blk) {
    if (blk->pid == pid) {
        return true;
    }
    return false;
}

/************** Function Definitions: Is in *************************/

bool list_is_in(list_t *list, block_t *blk) {
    node_t *curr = list->head;
    while (curr != NULL) {
        if (compare_blocks(curr->blk, blk)) {
            return true;
        }
        curr = curr->next;
    }
    return false;
}

bool list_is_in_by_size(list_t *list, int number) {
    node_t *curr = list->head;
    while (curr != NULL) {
        if (compare_size(number, curr->blk)) {
            return true;
        }
        curr = curr->next;
    }
    return false;
}

bool list_is_in_by_pid(list_t *list, int pid) {
    node_t *curr = list->head;
    while (curr != NULL) {
        if (compare_pid(pid, curr->blk)) {
            return true;
        }
        curr = curr->next;
    }
    return false;
}

/****************** Function Definitions: Getters *************************/

block_t* list_get_from_front(list_t *list) {
    node_t *curr = list->head;
    block_t *blk;
    if (curr == NULL) { //if the list is empty
        printf("List is empty\n");
        return NULL;
    }
    else { // if there is at least one element in the list
        blk = curr->blk;
    }
    return blk; // returns pointer to block structure in first node
}

block_t* list_get_from_back(list_t *list) {
    node_t *curr = list->head;
    block_t *blk;
    if (curr == NULL) { //if the list is empty
        printf("List is empty\n");
        return NULL;
    }
    blk = list->tail->blk;
    return blk; // returns pointer to block structure in last node
}

block_t* list_get_elem_at_index (list_t *list, int index) {
    block_t *blk;
    int count = 0; //! Assuming the linked list starts at 0
    /* If the linked list is empty*/
    if (list->head == NULL || index <  0 || index >= list->length) {
        printf("List is empty or invalid index\n");
        return NULL; 
    }

    /* If the index is the first element in the list element */
    if (index == 0) {
        return list_get_from_front(list);
    }
    else if (index == list->length - 1) {
        return list_get_from_back(list);
    }
    else {
        node_t *elem_node = find_node_at_index(list->head, index);
        return elem_node ? elem_node->blk : NULL; // Safely returns the block or NULL if the node doesnt exist
    }
}

int list_get_index_of(list_t *list, block_t *blk) {
    node_t *curr = list->head;
    int count = 0;

    if (curr == NULL) {
        return -1;
    }

    while (curr != NULL) {
        if (compare_blocks(curr->blk, blk)) {
            return count;
        }
        curr = curr->next;
        count++;
    }
    return -1;
}

int list_get_index_of_by_Size (list_t *list, int number) {
    node_t *curr = list->head;
    int count = 0;

    if (curr == NULL) {
        return -1;
    }

    while (curr != NULL) {
        if (compare_size(number, curr->blk)) {
            return count;
        }
        curr = curr->next;
        count++;
    }
    return -1;
}

int list_get_index_of_by_Pid(list_t *list, int pid) {
    node_t *curr = list->head;
    int count = 0;

    if (curr == NULL) {
        return -1;
    }

    while (curr != NULL) {
        if (compare_pid(pid, curr->blk)) {
            return count;
        }
        curr = curr->next;
        count++;
    }
    return -1;
}