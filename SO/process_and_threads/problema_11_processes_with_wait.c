
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

int main() {
    int pid, N;
    
    N=3;

    for(int i=0 ; i<N ; i++){
        pid = fork();

        if(pid == 0){
            printf("process son %d\n",i);
            sleep(10);
            exit(0);
        } 
    }

    printf("Wait for all processes\n");
    
    for(int i=0 ; i<N ; i++){
        wait(NULL);
    }
    printf("end\n");
}
