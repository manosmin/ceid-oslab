/*gcc -pthread test.c*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h> /*sem_open(), sem_destroy(), sem_wait()..*/
#include <sys/types.h> /*key_t, sem_t, pid_t*/
#include <sys/shm.h> /*shmat(), IPC_RMID*/
#include <sys/wait.h> /*waitpid(),...*/
#include <errno.h> /*errno, ECHILD*/
#include <fcntl.h> /*O_CREAT, O_EXEC*/

sem_t *S1,*S_D1_ends,*S_D2_ends,*S_D3_ends,*S_D4_ends;

void print_func(int id) {
	printf("Process %d\n",id);
}

void D1() {
sem_wait(S1);
	char command[]="pwd";
	print_func(1);
	system(command);
	printf("P1 has finished\n");
sem_post(S_D1_ends);
}

void D2() {
sem_wait(S_D1_ends);
	char command[]="pwd";
	print_func(2);
	system(command);
	printf("P2 has finished\n");
sem_post(S_D1_ends);
sem_post(S_D2_ends);
}

void D3() {
sem_wait(S_D1_ends);
	char command[]="pwd";
	print_func(3);
	system(command);
	printf("P3 has finished\n");
sem_post(S_D1_ends);
sem_post(S_D3_ends);
}

void D4() {
sem_wait(S_D1_ends);
sem_wait(S_D2_ends);
	char command[]="pwd";
	print_func(4);
	system(command);
	printf("P4 has finished\n");
sem_post(S_D1_ends);
sem_post(S_D2_ends);
sem_post(S_D4_ends);
}

void D5() {
sem_wait(S_D3_ends);
sem_wait(S_D4_ends);
	char command[]="pwd";
	print_func(5);
	system(command);
	printf("P5 has finished\n");
sem_post(S_D3_ends);
sem_post(S_D4_ends);
}

typedef int (*f)();
f process[] = {(void *)&D1,(void *)&D2,(void *)&D3,(void *)&D4,(void *)&D5};      

void child_process_code(int child_id) {
	process[child_id]();
}

int main() { 
    int i;
	pid_t pid;
    system("clear");

	S1 = sem_open ("S1", O_CREAT | O_EXCL, 0644, 1);
	S_D1_ends = sem_open ("S_D1_ends", O_CREAT | O_EXCL, 0644, 0);
	S_D2_ends = sem_open ("S_D2_ends", O_CREAT | O_EXCL, 0644, 0);
	S_D3_ends = sem_open ("S_D3_ends", O_CREAT | O_EXCL, 0644, 0);
	S_D4_ends = sem_open ("S_D4_ends", O_CREAT | O_EXCL, 0644, 0);

    for (i = 0; i < 5; i++){
        pid = fork ();
        if (pid < 0) {
            printf ("Fork error.\n");
        }
        else if (pid == 0) {
            break;
		}
    }
    if (pid==0){
		child_process_code(i);
        exit (0);
    } else {
        while (pid = waitpid (-1, NULL, 0)){
            if (errno == ECHILD)
                break;
        }
        printf ("\nParent: All children have exited.\n");
        sem_unlink ("S1");sem_close(S1);
		sem_unlink ("S_D1_ends");sem_close(S_D1_ends);
        sem_unlink ("S_D2_ends");sem_close(S_D2_ends);
		sem_unlink ("S_D3_ends");sem_close(S_D3_ends);
		sem_unlink ("S_D4_ends");sem_close(S_D4_ends);		
        exit (0);
    }
} 
