/* gcc -pthread -std=c99 readers-writers.c */
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>

sem_t wrt;
pthread_mutex_t mutex;
int count = 1;
int numreader = 0;

void *writer(void *wno)
{   
    sem_wait(&wrt);
    count = count*2;
    printf("Writer %d: count = %d\n",(*((int *)wno)),count);
    sem_post(&wrt);

}
void *reader(void *rno)
{
    pthread_mutex_lock(&mutex);
    numreader++;
    if(numreader == 1) {
        sem_wait(&wrt); 
    }
    pthread_mutex_unlock(&mutex);
    printf("Reader %d: count = %d\n",*((int *)rno),count);
    pthread_mutex_lock(&mutex);
    numreader--;
    if(numreader == 0) {
        sem_post(&wrt); 
    }
    pthread_mutex_unlock(&mutex);
}

int main()
{
    pthread_t read[5];
    pthread_t write[5];
    pthread_mutex_init(&mutex, NULL);
    sem_init(&wrt,0,1);
    int table[5] = {1,2,3,4,5}; 
    for(int i = 0; i < 5; i++) {
        pthread_create(&read[i], NULL, (void *)reader, (void *)&table[i]);
        pthread_create(&write[i], NULL, (void *)writer, (void *)&table[i]);
        pthread_join(read[i], NULL);
        pthread_join(write[i], NULL);
    }
    pthread_mutex_destroy(&mutex);
    sem_destroy(&wrt);
    return 0;
}