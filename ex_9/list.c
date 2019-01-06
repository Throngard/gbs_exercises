#include <stdio.h>
#include <stdlib.h>
#include "list.h"

/**
 * Initialises a new list.
 *
 * @return pointer to the new list
 */
list_t *list_init() {
    list_t *res;
    // Init an empty list
    res = malloc(sizeof(list_t));
    res->first = NULL;
    res->last = NULL;
    return res;
}

/**
 * Inserts a new list element at the start of
 * the provided list.
 *
 * @param list pointer (list_t)
 * @param data pointer (char)
 * @return pointer to the new element / NULL
 */
struct list_elem *list_insert(list_t *list, void *mem_pointer, unsigned int size, unsigned int in_use) {
    if (list == NULL) {
        return NULL;
    }

    // Init the new element
    struct list_elem *elem;
    elem = malloc(sizeof(struct list_elem));
    elem->mem_pointer = mem_pointer;
    elem->addr = malloc(20* sizeof(char));
    sprintf(elem->addr, "%p", mem_pointer);
    elem->size = size;
    elem->in_use = in_use;
    elem->status = in_use == 0 ? unused : used;
    elem->next = list->first;
    // New elem is now the new start of the list
    list->first = elem;

    // Check if this is the first element -> therefor also the end of the list
    if (list->last == NULL) {
        list->last = elem;
    }

    return elem;
}

/**
 * Appends a new list element at the end of
 * the provided list.
 *
 * @param list pointer (list_t)
 * @param data pointer (char)
 * @return pointer to the new element / NULL
 */
struct list_elem *list_append(list_t *list, void *mem_pointer, unsigned int size, unsigned int in_use) {
    if (list == NULL) {
        return NULL;
    }

    // Init the new element
    struct list_elem *elem;
    elem = malloc(sizeof(struct list_elem));
    elem->mem_pointer = mem_pointer;
    elem->addr = malloc(20* sizeof(char));
    sprintf(elem->addr, "%p", mem_pointer);
    elem->size = size;
    elem->in_use = in_use;
    elem->status = in_use == 0 ? unused : used;
    elem->next = NULL;

    // Check if this is the first element -> therefor also the start of the list
    if (list->last == NULL) {
        list->first = elem;
    } else {
        // Set the next pointer of the old end element to the new element
        list->last->next = elem;
    }
    list->last = elem;
    return elem;
}

struct list_elem *list_put (list_t *list, struct list_elem *current_elem, void *mem_pointer, unsigned int size, unsigned int in_use) {
    if (list == NULL) {
        return NULL;
    }

    // Init the new element
    struct list_elem *elem;
    elem = malloc(sizeof(struct list_elem));
    elem->mem_pointer = mem_pointer;
    elem->addr = malloc(20* sizeof(char));
    sprintf(elem->addr, "%p", mem_pointer);
    elem->size = size;
    elem->in_use = in_use;
    elem->status = in_use == 0 ? unused : used;
    elem->next = current_elem->next;
    current_elem->next = elem;
    // Check if this is the first element -> therefor also the start of the list
    if (elem->next == NULL) {
        list->last = elem;
    }
    return elem;
}


/**
 * Removes an existing element from the provided list.
 *
 * @param list pointer (list_t)
 * @param elem pointer (list_elem)
 * @return int, -1 on error or 0 on success
 */
int list_remove(list_t *list, struct list_elem *elem) {
    int return_val = -1;
    if (list == NULL) {
        return return_val;
    }

    // Initialize an iterator structure
    struct list_elem *mem_elem;
    struct list_elem *iterator;
    iterator = malloc(sizeof(struct list_elem));
    iterator->addr = NULL;
    iterator->next = list->first;

    // This is needed to free the newly created iterator in the end
    mem_elem = iterator;


    while (iterator->next != NULL) {
        if (iterator->next == elem) {

            // Take care of border cases: remove first / last elem of list
            if (list->first == elem) {
                list->first = iterator->next->next;
            }
            if (list->last == elem) {
                list->last = NULL;
            }

            // Change the next pointer of the previous item
            // to the next item of the search element.
            struct list_elem *temp = iterator->next;
            iterator->next = temp->next;
            free(elem);
            return_val = 0;
            break;
        }
        iterator = iterator->next;
    }
    // Take care of the iterator mem
    free(mem_elem);
    return return_val;
}

/**
 * Frees the memory of the provided list
 * and all its elements.
 *
 * @param list pointer (list_t)
 */
void list_finit(list_t *list) {
    if (list == NULL) {
        return;
    }
    // Init the iterator pointer
    struct list_elem *iterator;
    iterator = list->first;

    while (iterator != NULL) {
        struct list_elem *temp = iterator->next;
        // Use list_remove to free the element
        list_remove(list, iterator);
        iterator = temp;
    }
    // Finally free the mem of the list object
    free(list);
}

/**
 * Searches for an data value in the provided list.
 *
 * @param list pointer (list_t)
 * @param data pointer (char) to the search value
 * @param cmp_elem compare function for the list elements
 * @return pointer (list_element) to the search result
 */
struct list_elem *list_find(list_t *list, char *data, int (*cmp_elem) (const char *, const char *)) {
    if (list == NULL || data == NULL) {
        return NULL;
    }

    // Init the iterator pointer
    struct list_elem *iterator;
    iterator = list->first;

    while (iterator != NULL) {
        // Return the first element in the list that is equal the data
        if (cmp_elem(iterator->addr, data) == 0) {
            return iterator;
        }
        iterator = iterator->next;
    }

    return NULL;
}

struct list_elem *list_find_addr(list_t *list, void *addr) {
    if (list == NULL || addr == NULL) {
        return NULL;
    }

    // Init the iterator pointer
    struct list_elem *iterator;
    iterator = list->first;

    while (iterator != NULL) {
        // Return the first element in the list that is equal the data
        if (iterator->mem_pointer == addr) {
            return iterator;
        }
        iterator = iterator->next;
    }

    return NULL;
}

/**
 * Simple string print
 *
 * @param data pointer (char)
 */
void print_string(struct list_elem *elem) {
    printf("[%s %c %d/%d] ", elem->addr, elem->status == unused ? 'F' : 'P', elem->in_use, elem->size);
}

/**
 * Prints the provided list to the command line.
 *
 * @param list pointer (list_t)
 * @param print_elem element print function
 */
void list_print(list_t *list, void (*print_elem) (struct list_elem *elem)) {
    if (list == NULL) {
        return;
    }

    // Init the iterator pointer
    struct list_elem *iterator;
    iterator = list->first;
    int index = 1;

    while (iterator != NULL) {
        printf("%d:", index);
        print_elem(iterator);
        iterator = iterator->next;
        index++;
    }
    printf("\n");
}

