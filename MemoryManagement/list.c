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
  list->head = NULL;
  return list;
}

node_t *node_alloc(block_t *blk) {
  node_t *node = malloc(sizeof(node_t));
  node->next = NULL;
  node->blk = blk;
  return node;
}

void list_free(list_t *l) {
  free(l);
}

void node_free(node_t *node) {
  free(node);
}

void list_print(list_t *list) {
    node_t *curr = list->head;
    block_t *my_block;
    if (curr == NULL){ 
        printf("List is empty\n");
    }
    while (curr != NULL) {
        my_block = curr->blk;
        printf("Block Info: PID=%d, START=%d, end=%d", my_block->pid, my_block->start, my_block->end);
        curr = curr->next;  
    }
}

//? Should I just add the list_length function inside of the list structure
int list_length(list_t *list) {
    node_t *curr = list->head;
    int count = 0;
    while (curr != NULL) {
        count++;
        curr = curr->next;
    }
    return count;
}

void list_coalese_nodes(list_t *list){
    node_t *curr = list->head;

    while (curr != NULL && curr->next != NULL) {
        if (curr->blk->end + 1 == curr->next->blk->start) {
            curr->blk->end = curr->next->blk->end;
            curr->next = curr->next->next;
            node_free(curr->next);
        }
        else {
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
    node_t *curr = list->head;
    if (curr == NULL) {
        list->head = new_node;
    }
    else {
        while (curr->next != NULL) {
            curr = curr->next;
        }
        curr->next = new_node;
    }    
}

void list_add_to_front(list_t *list, block_t *blk) {
    node_t *new_node = node_alloc(blk);
    node_t *curr = list->head;
    if (curr == NULL) {
        list->head = new_node;
    }
    else {
        new_node->next = curr;
        list->head = new_node;
    }
}

void list_add_at_index(list_t *list, block_t *blk, int index) {
    node_t *new_node = node_alloc(blk);
    node_t *curr = list->head;
    int count = 0;
    if (index == 0) {
        list_add_to_front(list, blk);
    }
    else if (index >= list_length(list)) { //! might change if I incoroporate the length function into the list structure
        list_add_to_back(list, blk);
    }
    else {
        while (count < index - 1) {
            curr = curr->next;
            count++;
        }
        new_node->next = curr->next;
        curr->next = new_node;
    }
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
        while (curr != NULL && ((curr->blk->end - newblk->start) < (newblk->end - newblk->start))) {
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
    node_t *curr = list->head;
    block_t *blk;
    if (curr == NULL) { //if the list is empty
        printf("List is empty\n");
    }
    else { // if there is at least one element in the list
        blk = curr->blk;
        list->head = curr->next;
        node_free(curr);
    }
    return blk; //? Does this return the block that was removed or the memory address of the block that was removed
}

block_t* list_remove_from_back(list_t *list) {
    node_t *curr = list->head;
    node_t *prev = NULL;
    block_t *blk;
    if (curr == NULL) { //if the list is empty
        printf("List is empty\n");
    }
    else { // if there is at least one element in the list
        while (curr->next != NULL) {
            prev = curr;
            curr = curr->next;
        }
        blk = curr->blk;
        prev->next = NULL;
        node_free(curr);
    }
    return blk; //? Does this return the block that was removed or the memory address of the block that was removed
}

block_t* list_remove_at_index(list_t *list, int index) {
    node_t *curr = list->head;
    node_t *prev = NULL;
    block_t *blk;
    int count = 0; //! Assuming the linked list starts at 0
    if (curr == NULL) { //if the list is empty
        printf("List is empty\n");
        return blk; //? Does return the block that was removed or the memory address of the block that was removed
    }
    else if (index == 0) { //if the index is 0
        return list_remove_from_front(list);
    }
    //? what should happen if the index is out of range? for noq, I will just return the last element
    else if (index >= list_length(list)) { // if the index is greater than the length of the list
        return list_remove_from_back(list);
    }
    else { //if the index is in the middle of the list
        while (count < index) { //? should be able to assume that each element points to another element
            prev = curr;
            curr = curr->next;
            count++;
        }

        blk = curr->blk;
        prev->next = curr->next;
        node_free(curr);
    }
    return blk; //? does this return the block that was removed or the memory address of the block that was removed
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
    }
    else { // if there is at least one element in the list
        blk = curr->blk;
    }
    return blk; //? Does this return the block that was removed or the memory address of the block that was removed
}

block_t* list_get_from_back(list_t *list) {
    node_t *curr = list->head;
    node_t *prev = NULL;
    block_t *blk;
    if (curr == NULL) { //if the list is empty
        printf("List is empty\n");
    }
    else { // if there is at least one element in the list
        while (curr->next != NULL) {
            prev = curr;
            curr = curr->next;
        }
        blk = curr->blk;
    }
    return blk; //? Does this return the block that was removed or the memory address of the block that was removed
}

block_t* list_get_elem_at_index (list_t *list, int index) {
    node_t *curr = list->head;
    node_t *prev = NULL;
    block_t *blk;
    int count = 0; //! Assuming the linked list starts at 0
    if (curr == NULL) { //if the list is empty
        printf("List is empty\n");
        return blk; //? Does return the block that was removed or the memory address of the block that was removed
    }
    else if (index == 0) { //if the index is 0
        return list_get_from_front(list);
    }
    //? what should happen if the index is out of range? for noq, I will just return the last element
    else if (index >= list_length(list)) { // if the index is greater than the length of the list
        return list_get_from_back(list);
    }
    else { //if the index is in the middle of the list
        while (count < index) { //? should be able to assume that each element points to another element
            prev = curr;
            curr = curr->next;
            count++;
        }

        blk = curr->blk;
    }
    return blk; //? does this return the block that was removed or the memory address of the block that was removed
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