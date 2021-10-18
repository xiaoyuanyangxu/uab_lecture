
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

void *do_nothing(void *param){
   int i;
   char * result = (char*)malloc(sizeof(char) * 100);

   i= *(int*)param;
   sprintf(result, "Thread %i\n", i);
   printf("%s", result);

   sleep(10);
   
   pthread_exit(result);
   
}

int main() {
    int t,rc;
    pthread_t tid[3];
    pthread_attr_t attr;
    int thread_param[3];
    char* thread_result[3];

    pthread_attr_init(&attr);

    printf("Create 3 threads\n");
    for(int i=0;i<3;i++){
        thread_param[i] = i;
        rc=pthread_create(&tid[i], 
                        &attr, do_nothing, 
                        &(thread_param[i]));
    }
    
    printf("Join all 3 threads\n");
    for(int i=0;i<3;i++) {
        char * result;
        rc=pthread_join(tid[i],(void**)&result);
        printf("Result %i: %s", i, result);
        free(result);
    }

    pthread_exit(NULL);
}
