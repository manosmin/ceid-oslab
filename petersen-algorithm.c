/* gcc -pthread petersen-algorithm.c */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#define N   2
#define MAX 500

int flag[N], turn, X=0;

void lock_init();
void lock(int process);
void unlock(int process);
void* child_process_i(void *p);

int main() {

    pthread_t p1, p2;
    lock_init();

    pthread_create(&p1, NULL, child_process_i, (void*) 0);
    pthread_create(&p2, NULL, child_process_i, (void*) 1);

    pthread_join(p1, NULL);
    pthread_join(p2, NULL);

    printf("Actual Count: %d | Expected Count: %d", X, MAX*2); 

    return 0;
}

void* child_process_i(void *p) {

    int i, tmp;
    int process = *(int *)p;

    lock(process);

    for (i=0; i<=MAX; i++) {
        tmp = X;
        tmp = tmp+1;
        X = tmp;
    } 

    unlock(process);

}

void lock_init() {
    
    flag[0] = flag[1] = 0;
    turn = 0;
}

void lock(int process) {

    flag[process] = 1;
    turn = 1-process;

    while (flag[1-process]==1 && turn==1-process); 
}

void unlock(int process) {
    
    flag[process] = 0;
}