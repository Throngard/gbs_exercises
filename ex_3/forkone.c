#include<stdio.h>
#include<stdlib.h>
#include<sys/wait.h>
#include<unistd.h>
#include <sys/time.h>
#include <time.h>

/**
 * Creates one new child process and waits for this new process to finish.
 * Childs simply count up to @param count once a second.
 *
 * @param count
 */
void forkone(int count) {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    printf("Start: %s", ctime(&tv.tv_sec));
    int i;

    if(fork() == 0) {
        for (i = 0; i < count; i++) {
            sleep(1);
            printf("%d %d %d\n", getpid(), getppid(), i+1);
        }
        int exitCode = (getpid()+i) % 100;
        exit(exitCode);
    } else {
        int stat;
        wait(&stat);
        // We have to use the WEXITSTATUS Makro to unmask the exit code of the child
        printf("Exit-Code: %d\n", WEXITSTATUS(stat));
    }
    gettimeofday(&tv, NULL);
    printf("Ende: %s", ctime(&tv.tv_sec));
}

int main(int argc, char *argv[]) {
    int maxCount = 10;
    if(argc == 2) {
        maxCount = atoi(argv[1]);
    }
    forkone(maxCount);
    return 0;
}
