#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "memory.h"

int main(int argc,char **argv){
    unsigned int block_size = 4096;
    unsigned int mem_size = 4096 * 256;
    enum mem_algo algo = first;
    int was_init = 0;
    int index = 0;
    void **arr = malloc(sizeof(void *) * 200);

    int option = 0;
    while((option = getopt(argc,argv,"m:b:1wa:f:"))!=-1){
        switch(option){
            case 'm' :
                mem_size = atoi(optarg);
                break;
            case 'b':
                block_size = atoi(optarg);
                break;
            case '1':
                algo = first;
                break;
            case 'w':
                algo = worst;
                break;
            case 'a' :
                if (was_init == 0) {
                    mem_init(mem_size, block_size, algo);
                    was_init = 1;
                }
                int curr_block = atoi(optarg);
                arr[index] = mem_alloc(curr_block);
                index ++;
                mem_dump();
                break;
            case 'f' :
                if (was_init == 0) {
                    break;
                }
                int block = atoi(optarg);
                mem_free(arr[block-1]);
                mem_dump();
                break;

        }
    }
    return 0;
}