
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    int *pid;
    int N;
    
    N=3;
    pid = (int*)malloc(sizeof(int) * N);

    for(int i=0 ; i<N ; i++){
        pid[i] = fork();
        if(pid[i] == 0){
            printf("process son %d\n",i);
            sleep(10);
            exit(4);
        } 
    }
    
    printf("Wait for all processes\n");
    for(int i=0 ; i<N ; i++){
        int status;
        if (pid[i] > 0)
        {
            waitpid(pid[i], &status,0);
            printf("Process %i end with %i\n", i, WEXITSTATUS(status));
        }
    }
    free(pid);
    printf("end\n");

}

