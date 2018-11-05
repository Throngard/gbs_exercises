#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "list.h"

int main(int argc, char *argv[]) {
    list_t *li;

    if ((li = list_init()) == NULL) {
        perror("Cannot allocate memory");
        exit(-1);
    }
    int i;
    for(i=1; i < argc-1; i+=2) {
        switch (argv[i][1]) {
            case 'a':
                if (list_append(li, argv[i+1]) == NULL) {
                    perror("Cannot allocate memory");
                    exit(-1);
                }
                break;
            case 'i':
                if (list_insert(li, argv[i+1]) == NULL) {
                    perror("Cannot allocate memory");
                    exit(-1);
                }
                break;
            case 'r':
                list_remove(li, list_find(li, argv[i+1], strcmp));
                break;
            default:
                perror("Invalid argument, exiting...");
                exit(EXIT_FAILURE);
        }
    }

    list_print(li, print_string);
    list_finit(li);
    return 0;
}