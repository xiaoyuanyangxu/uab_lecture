#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>
#include <stdlib.h>
  
pthread_mutex_t m;
pthread_cond_t ocupat_cv;
pthread_cond_t lliure_cv;

int size = 10;
int consum = 0;
int producte = 0;
int buffer[10];

int ocupats() {
    int num =0;
    for (int i=0 ; i< size ; i++)
    {
        if (buffer[i] > 0) {
            num +=1;
        }
    }
    return num;
}

int lliures() {
    int num =0;
    for (int i=0 ; i< size ; i++)
    {
        if (buffer[i] == 0) {
            num +=1;
        }
    }
    return num;

}
  
void* producer(void* arg)
{
    while(1) {
        int l;
        pthread_mutex_lock(&m);
        while((l=lliures()) < 1) {
            pthread_cond_wait(&lliure_cv, &m); // This function do 2 things: 1) wait for lliure_cv 2) ulock m
        }
        int r = rand() %100;
        int p,c;
        buffer[producte] = r;
        producte = (producte + 1) % size;
        
        c = consum;
        p = producte;

        pthread_cond_signal(&ocupat_cv);
        pthread_mutex_unlock(&m);

        printf("Producer: %i Consum=%i Producte=%i Lliure=%i\n", r, c, p, l);
        int sleep_sec = rand() % 4;
        sleep(sleep_sec);
    }

    return 0;
}

void* consumer(void* arg)
{
    int consumer_id = *(int*)arg;
    while(1) {
        int o;
        pthread_mutex_lock(&m);
        
        while((o=ocupats()) < 1) {
            pthread_cond_wait(&ocupat_cv, &m); // This function do 2 things: 1) wait for ocupat_cv 2) ulock m
        }
        int v;
        v = buffer[consum];
        buffer[consum] =0;
        consum = (consum + 1) % size;

        pthread_cond_signal(&lliure_cv);
        pthread_mutex_unlock(&m);
        
        printf("\tConsumer %i: %i Ocupats=%i\n", consumer_id, v, o);
        int sleep_sec = rand() % 1;
        sleep(sleep_sec);
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
    pthread_cond_init(&ocupat_cv, NULL);
    pthread_cond_init(&lliure_cv, NULL);

    // Create 1 thread for producer
    pthread_create(&t1,NULL,producer,NULL);
    sleep(2);

    // Create 2 threads for consumers
    int pthread_id_1 = 0;
    pthread_create(&t2,NULL,consumer,&pthread_id_1);
    int pthread_id_2 = 1;
    pthread_create(&t2,NULL,consumer,&pthread_id_2);
    
    // Wait all 2 threads
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);
    
    return 0;
}

/* Posible OUTPUT
$./problema4_condition_var 

Producer: 7 Consum=0 Producte=1 Lliure=10
Producer: 73 Consum=0 Producte=2 Lliure=9
        Consumer 0: 7 Ocupats=2
        Consumer 1: 73 Ocupats=1
Producer: 44 Consum=2 Producte=3 Lliure=10
        Consumer 0: 44 Ocupats=1
Producer: 9 Consum=3 Producte=4 Lliure=10
Producer: 65 Consum=4 Producte=5 Lliure=10
Producer: 42 Consum=4 Producte=6 Lliure=9
        Consumer 1: 9 Ocupats=1
        Consumer 1: 42 Ocupats=1
        Consumer 0: 65 Ocupats=2
Producer: 40 Consum=6 Producte=7 Lliure=10
Producer: 3 Consum=6 Producte=8 Lliure=9
        Consumer 1: 40 Ocupats=2
        Consumer 1: 3 Ocupats=1
Producer: 60 Consum=8 Producte=9 Lliure=10
        Consumer 1: 60 Ocupats=1
Producer: 78 Consum=9 Producte=0 Lliure=10
Producer: 35 Consum=9 Producte=1 Lliure=9
        Consumer 0: 78 Ocupats=2
        Consumer 0: 35 Ocupats=1
*/