#include <assert.h>
#include <signal.h>
#include <stdbool.h> /* false */
#include <stdio.h> /* perror */
#include <stdlib.h> /* EXIT_SUCCESS, EXIT_FAILURE */
#include <sys/wait.h> /* wait, sleep */
#include <unistd.h> /* fork, write */
#include <fcntl.h>
#include <string.h>


int open_file(char * file_name, int flags, int mode) {
    return open(file_name, flags, mode);
}

void close_file(int fd) {
    close(fd);
}

void write_file(int fd, char * body, int num_bytes) {
    write(fd, body, num_bytes);
}

void read_file(int fd, char * buffer, int num_bytes) {
    read(fd, buffer, num_bytes);
}

void lector_signal_handler(int sig) {
    int fd; 
    char buffer[100];
    printf("\n\tLECTOR: got a Signal %i\n", sig);

    if (sig == SIGUSR1) {   
        // Open file to read     
        fd = open_file("test.txt", O_RDONLY, S_IRWXU);
        if (fd < 0) {
            printf("\tLECTOR: error in opening file\n");
        }else{
            // Read the file
            read_file(fd, buffer, 100);
            printf("\tLECTOR: got %s\n", buffer);
            
            // Close the file
            close_file(fd);
        }
    }
}

int main() {
    pid_t pid;
    int fd;
    char buffer[100];

    // Set buffer to zero
    memset(buffer, 0, sizeof(buffer));

    // Configure the signal
    signal(SIGUSR1, lector_signal_handler);

    pid = fork();
    if (pid == -1) {
        printf("Error in fork\n");
    } else {
        if (pid == 0) {
            printf("\tLECTOR: Who am I?\n");
            for (int i=0 ; i < 10 ; i++) {
                printf("\tLECTOR: sleep %i\n", i);
                sleep(1);
            }
            exit(EXIT_SUCCESS);
        }

        printf("ESCRITOR: I'm the father and have a son %i\n", pid);
        sleep(2);

        // Open the file to write
        fd = open_file("test.txt", O_WRONLY | O_CREAT, S_IRWXU);
        if (fd < 0) {
            printf("ESCRITOR: Error in opening file\n");
            exit(EXIT_FAILURE);
        }
        // Write the content
        sprintf(buffer, "SON %i, I'm your FATHER\n", pid);
        write_file(fd, buffer, 100);
        // Close the file
        close_file(fd);

        // Send the SIGNAL
        kill(pid, SIGUSR1);

        // Wait the son
        wait(NULL);
        printf("ESCRITOR: Father done\n");
    }
    return EXIT_SUCCESS;
}

/* OUTPUT EXAMPLE
ESCRITOR: I'm the father and have a son 29497
        LECTOR: Who am I?
        LECTOR: sleep 0
        LECTOR: sleep 1
        LECTOR: sleep 2

        LECTOR: got a Signal 30
        LECTOR: got SON 29497, I'm your FATHER

        LECTOR: sleep 3
        LECTOR: sleep 4
        LECTOR: sleep 5
        LECTOR: sleep 6
        LECTOR: sleep 7
        LECTOR: sleep 8
        LECTOR: sleep 9
ESCRITOR: Father done
*/