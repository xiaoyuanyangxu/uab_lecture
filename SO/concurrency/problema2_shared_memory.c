#include <stdio.h>
#include <stdlib.h>
#include <sys/mman.h>
#include <string.h>
#include <unistd.h>

void* create_shared_memory(size_t size) {
    int protection = PROT_READ | PROT_WRITE;
    int visibility = MAP_SHARED | MAP_ANONYMOUS;
    return mmap(NULL, size, protection, visibility, -1, 0);
}

int main() {
    int var1 = 0;
    int var2 = 1;

    int* torn = create_shared_memory(sizeof(int));
    memcpy(torn, &var1, sizeof(var1));

    int pid = fork();
    if (pid == 0) {
        var1 = 1;
        var2 = 0;

        while(*torn != var1){};  // You should open a TOP to see the CPU at 100%

        printf("\tSON: I'm in CRITICAL SECTION\n");
        sleep(5);
        printf("\tSON: I'm DONE\n");
        
        memcpy(torn, &var2, sizeof(var2));
        printf("\tSON: end\n");
        exit(0);
  
  } else {
        while(*torn != var1); 
        
        printf("FATHER: I'm in CRITICAL SECTION\n");
        sleep(5);
        printf("FATHER: I'm DONE\n");
        

        memcpy(torn, &var2, sizeof(var2));

        wait(NULL);
        printf("FATHER: end\n");
        exit(0);
  }
}

/* OUTPUT EXAMPLE:
FATHER: I'm in CRITICAL SECTION
FATHER: I'm DONE
        SON: I'm in CRITICAL SECTION
        SON: I'm DONE
        SON: end
FATHER: end
*/
