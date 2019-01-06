#include <stdlib.h>
#include "memory.h"
#include "list.h"
#include <stdio.h>

enum mem_algo current_strat = none;
unsigned int current_block_size = 0;
unsigned int current_size = 0;
list_t *mem_list;

int mem_init (unsigned int size, unsigned int blocksize, enum mem_algo strategy) {
    current_size = size;
    current_block_size = blocksize;
    current_strat = strategy;
    void *memory = malloc(current_size);
    mem_list = list_init();
    list_insert(mem_list, memory, current_size, 0);
    mem_dump();
    return 0;
}

void *mem_alloc(unsigned int size) {
    struct list_elem *iterator;
    struct list_elem *worst_fit = NULL;
    void *result = NULL;

    switch(current_strat) {
        case first:
            iterator = mem_list->first;


            while (iterator != NULL) {
                if (iterator->status == unused && iterator->size >= size) {

                    unsigned int needed_mem = size;
                    if (size % current_block_size != 0) {
                        needed_mem += (current_block_size - (size % current_block_size));
                    }
                    unsigned int remaining_mem = iterator->size - needed_mem;
                    iterator->size=needed_mem;
                    iterator->in_use = size;
                    iterator->status = used;
                    if (remaining_mem != 0) {
                        list_put(mem_list, iterator, iterator->mem_pointer + needed_mem, remaining_mem, 0);
                    }
                    result = iterator->mem_pointer;
                    break;
                }
                iterator = iterator->next;
            }
            break;
        case worst:
            iterator = mem_list->first;

            while (iterator != NULL) {
                if (iterator->status == unused && iterator->size >= size) {

                    if (worst_fit == NULL) {
                        worst_fit = iterator;
                    } else if(worst_fit->size < iterator->size) {
                            worst_fit = iterator;
                    }
                }
                iterator = iterator->next;
            }
            if (worst_fit != NULL) {
                unsigned int needed_mem = size;
                if (size % current_block_size != 0) {
                    needed_mem += (current_block_size - (size % current_block_size));
                }
                unsigned int remaining_mem = worst_fit->size - needed_mem;
                worst_fit->size=needed_mem;
                worst_fit->in_use = size;
                worst_fit->status = used;
                if (remaining_mem != 0) {
                    list_put(mem_list, worst_fit, worst_fit->mem_pointer + needed_mem, remaining_mem, 0);
                }
                result = worst_fit->mem_pointer;
            }
            break;
        case none:break;
    }
    return result;
}

void mem_dump() {
    list_print(mem_list, print_string);
}

void mem_free(void *addr) {
    struct list_elem *elem = list_find_addr(mem_list, addr);
    if (elem == NULL) {
        return;
    }

    elem->in_use = 0;
    elem->status = unused;

    struct list_elem *iterator;
    iterator = mem_list->first;

    while (iterator != NULL && iterator->next != NULL) {
        while (iterator->next != NULL && iterator->status == unused && iterator->next->status == unused) {
            iterator->size += iterator->next->size;
            list_remove(mem_list, iterator->next);
        }
        iterator = iterator->next;
    }
}