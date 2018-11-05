#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include <sys/time.h>
#include <time.h>
#include <jmorecfg.h>
#include <stdbool.h>
#include "list.h"

/**
 * Starts N processes and lets them count up to K
 * or a random number if -r is provided.
 * All child processes get stored in a linked list.
 * The method exits once all childs have terminated.
 *
 * @param N Number of processes to spawn
 * @param K Counter
 * @param isRand true if K should be randomized
 */
void forkmany(int N, int K, bool isRand) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    printf("Start: %s", ctime(&tv.tv_sec));
    list_t *li;

    // Init our child process storage
    if ((li = list_init()) == NULL) {
        perror("Cannot allocate memory");
        exit(-1);
    }

    // Fork N child processes
    for(int j = 0; j < N; j++) {
        pid_t child_id = fork();
        if(child_id == 0) {
            int i;
            // If isRand is provided seed the randomizer engine with
            // the process id of the child to make sure we have random
            // values in each child.
            if(isRand) {
                srand(getpid());
                K = rand() % ((int)(K * 1.5) + 1 - K/2) + K/2;
            }
            for (i = 0; i < K; i++) {
                sleep(1);
                printf("%d %d %d\n", getpid(), getppid(), i+1);
            }
            int exitCode = (getpid()+i) % 100;
            exit(exitCode);
        } else {
            // Store the newly forked child pid in our list
            list_append(li, child_id);
        }
    }
    // Wait until all child processes have terminated
    pid_t wpid;
    int stat;
    while (li->first != NULL) {
        wpid = wait(&stat);
        list_remove(li, list_find(li, wpid, cmp_pid));
        // We have to use the WEXITSTATUS Makro to unmask the exit code of the child
        printf("Exit-Code: %d\n", WEXITSTATUS(stat));
    };
    gettimeofday(&tv, NULL);
    printf("Ende: %s", ctime(&tv.tv_sec));
}

int main(int argc, char *argv[]) {

    int N = 1;
    int K = 10;
    bool isRand = false;

    // Parse the arguments
    int option = 0;
    while ((option = getopt(argc, argv,"k:n:r")) != -1) {
        switch (option) {
            case 'k' :
                K = atoi(optarg);
                break;
            case 'n' :
                N = atoi(optarg);
                break;
            case 'r':
                isRand = true;
                break;
            default:
                exit(EXIT_FAILURE);
        }
    }
    forkmany(N, K, isRand);
    return 0;
}