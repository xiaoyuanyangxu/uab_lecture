#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
  
pthread_mutex_t m;
int size = 10;
int consum = 0;
int producte = 0;
int buffer[10];

int ocupats() {
    int num =0;
    pthread_mutex_lock(&m);
    for (int i=0 ; i< size ; i++)
    {
        if (buffer[i] > 0) {
            num +=1;
        }
    }
    pthread_mutex_unlock(&m);
    return num;
}

int lliures() {
    int num =0;
    pthread_mutex_lock(&m);
    for (int i=0 ; i< size ; i++)
    {
        if (buffer[i] == 0) {
            num +=1;
        }
    }
    pthread_mutex_unlock(&m);
    //sleep(3);
    return num;

}
  
void* producer(void* arg)
{
    while(1) {
        int l;
        while((l=lliures()) < 1) {
            sleep(1);
        }
        int r = rand() %100;
        int p,c;

        pthread_mutex_lock(&m);
        buffer[producte] = r;
        producte = (producte + 1) % size;
        
        c = consum;
        p = producte;
        pthread_mutex_unlock(&m);
        printf("Producer: %i Consum=%i Producte=%i Lliure=%i\n", r, c, p, l);
        //sleep(2);
    }

    return 0;
}

void* consumer(void* arg)
{
    while(1) {
        int o;
        while((o=ocupats()) < 1) {
            sleep(1);
        }
        int v;
        pthread_mutex_lock(&m);
        v = buffer[consum];
        buffer[consum] =0;
        consum = (consum + 1) % size;
        pthread_mutex_unlock(&m);
        printf("\tConsumer: %i Ocupats=%i\n", v, o);
        sleep(2);
    }
    return 0;
}
 

int main()
{
    pthread_t t1,t2;
    // Initialize the vector
    for(int i=0 ; i<size ; i++) {
        buffer[i] = 0;
    }

    pthread_mutex_init(&m, NULL);
    
    // Create the 2 threads
    pthread_create(&t1,NULL,producer,NULL);
    sleep(2);
    pthread_create(&t2,NULL,consumer,NULL);
    
    // Wait all 2 threads
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    
    return 0;
}