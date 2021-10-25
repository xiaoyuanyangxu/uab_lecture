#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
  
sem_t semaphore;
int vector[10];
  
void* sum(void* arg)
{
    int total = 0;
    //wait
    int ok = sem_wait(&semaphore);
    printf("Let sum %i\n", ok);
    if (ok < 0) {
        perror("there are some error\n");
    }

    //critical section
    for(int i=0 ; i<10 ; i++){
        total = total + vector[i];
        sleep(1);
    }
       
    //signal
    printf("Sum is done %i\n", total);
    sem_post(&semaphore);

    return 0;
}

void* reset(void* arg)
{
    //wait
    int ok = sem_wait(&semaphore);
    printf("\tLet do the reset %i\n", ok);

    //critical section
    for(int i=0 ; i<10 ; i++){
        vector[i] = 0;
    }
       
    //signal
    printf("\tReset is done\n");
    sem_post(&semaphore);
    return 0;
}
 

int main()
{
    pthread_t t1,t2;

    // Initialize the vector
    for(int i=0 ; i<10 ; i++) {
        vector[i] = 10;
    }

    // Initialize the semaphore
    sem_init(&semaphore, 0, 1);
    
    // Create the 2 threads
    pthread_create(&t1,NULL,sum,NULL);
    sleep(2);
    pthread_create(&t2,NULL,reset,NULL);
    
    // Wait all 2 threads
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    
    // Destroy the semaphore
    sem_destroy(&semaphore);
    return 0;
}