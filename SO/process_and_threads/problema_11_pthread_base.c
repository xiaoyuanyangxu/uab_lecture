#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void *do_nothing(void *null){
   int i;
   i=0;
   printf("I'm a Thread\n");
   sleep(10);
   pthread_exit(NULL);
}

int main() {
    int t,rc;
    pthread_t tid[3];
    pthread_attr_t attr;
    pthread_attr_init(&attr);

    printf("Create 3 threads\n");
    for(int i=0;i<3;i++){
        rc=pthread_create(&tid[i], &attr, do_nothing, NULL);
    }

    printf("Join all 3 threads\n");
    for(int i=0;i<3;i++) {
        rc=pthread_join(tid[i],NULL);
    }

    pthread_exit(NULL);
}