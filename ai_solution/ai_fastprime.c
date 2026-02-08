#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/wait.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <stdbool.h>

bool is_prime(int n) {
    if (n < 2) return false;
    if (n == 2) return true;
    if (n % 2 == 0) return false;
    for (int i = 3; i * i <= n; i += 2) {
        if (n % i == 0) return false;
    }
    return true;
}

void write_chunk(int start, int end, int fd) {
    char buffer[4096];
    buffer[0] = '\0'; 
    char temp[32];

    for (int i = start; i <= end; i++) {
        if (is_prime(i)) {
            sprintf(temp, "%d\n", i);
            
            
            if (strlen(buffer) + strlen(temp) >= sizeof(buffer)) {
                write(fd, buffer, strlen(buffer));
                buffer[0] = '\0';
            }
            strcat(buffer, temp);
        }
    }
   
    if (strlen(buffer) > 0) {
        write(fd, buffer, strlen(buffer));
    }
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <rl> <rh>\n", argv[0]);
        return 1;
    }

    int rl = atoi(argv[1]);
    int rh = atoi(argv[2]);
    
   
    int n_cores = sysconf(_SC_NPROCESSORS_ONLN);
    if (n_cores < 1) n_cores = 1;

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    int fd = open("prime.txt", O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (fd < 0) {
        perror("Error opening file");
        return 1;
    }

    int range = rh - rl + 1;
    int chunk = range / n_cores;

    for (int i = 0; i < n_cores; i++) {
        pid_t pid = fork();

        if (pid == 0) { 
            int s = rl + i * chunk;
            int e = (i == n_cores - 1) ? rh : (s + chunk - 1);
            write_chunk(s, e, fd);
            close(fd); 
            exit(0);
        } else if (pid < 0) {
            perror("Fork failed");
        }
    }

 
    for (int i = 0; i < n_cores; i++) {
        wait(NULL);
    }

    close(fd);

    clock_gettime(CLOCK_MONOTONIC, &end);
    double time_taken = (end.tv_sec - start.tv_sec) + (end.tv_nsec - start.tv_nsec) / 1e9;

    printf("AI Solution (C Language) Time: %.6f s using %d processes.\n", time_taken, n_cores);

    return 0;
}