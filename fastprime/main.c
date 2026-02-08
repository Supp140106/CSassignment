#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>
#include <time.h>


int is_prime(int num) {
    if (num < 2) return 0;
    if (num == 2) return 1;
    if (num % 2 == 0) return 0;

    for (int i = 3; i * i <= num; i += 2)
        if (num % i == 0)
            return 0;

    return 1;
}

//
double run_parallel(int rl, int rh, int n) {
    int range = rh - rl + 1;
    int chunk = range / n;

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < n; i++) {
        pid_t pid = fork();

        if (pid == 0) {  // child
            int low = rl + i * chunk;
            int high = (i == n - 1) ? rh : low + chunk - 1;

            for (int num = low; num <= high; num++)
                is_prime(num);

            exit(0);
        }
    }

    for (int i = 0; i < n; i++)
        wait(NULL);

    clock_gettime(CLOCK_MONOTONIC, &end);

    return (end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec) / 1e9;
}


void write_primes(int rl, int rh) {
    FILE *fp = fopen("result.txt", "w");
    if (!fp) {
        perror("result.txt");
        return;
    }

    for (int i = rl; i <= rh; i++)
        if (is_prime(i))
            fprintf(fp, "%d\n", i);

    fclose(fp);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <rl> <rh>\n", argv[0]);
        return 1;
    }

    int rl = atoi(argv[1]);
    int rh = atoi(argv[2]);

    int max_cores = sysconf(_SC_NPROCESSORS_ONLN);
    printf("Detected logical CPUs: %d\n", max_cores);


    FILE *csv = fopen("results.csv", "a+");
    if (!csv) {
        perror("results.csv");
        return 1;
    }

    fseek(csv, 0, SEEK_END);
    if (ftell(csv) == 0) {
        fprintf(csv, "n,time_seconds\n");
    }

    for (int n = 1; n <= max_cores; n++) {
        double t = run_parallel(rl, rh, n);
        printf("n = %d | time = %.6f sec\n", n, t);
        fprintf(csv, "%d,%.6f\n", n, t);
        fflush(csv);
    }

    fclose(csv);


    write_primes(rl, rh);

    printf("Timing saved in results.csv\n");
    printf("Prime numbers saved in result.txt\n");

    return 0;
}
#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <math.h>
#include <time.h>


int is_prime(int num) {
    if (num < 2) return 0;
    if (num == 2) return 1;
    if (num % 2 == 0) return 0;

    for (int i = 3; i * i <= num; i += 2)
        if (num % i == 0)
            return 0;

    return 1;
}


double run_parallel(int rl, int rh, int n) {
    int range = rh - rl + 1;
    int chunk = range / n;

    struct timespec start, end;
    clock_gettime(CLOCK_MONOTONIC, &start);

    for (int i = 0; i < n; i++) {
        pid_t pid = fork();

        if (pid == 0) {  // child
            int low = rl + i * chunk;
            int high = (i == n - 1) ? rh : low + chunk - 1;

            for (int num = low; num <= high; num++)
                is_prime(num);

            exit(0);
        }
    }

    for (int i = 0; i < n; i++)
        wait(NULL);

    clock_gettime(CLOCK_MONOTONIC, &end);

    return (end.tv_sec - start.tv_sec) +
           (end.tv_nsec - start.tv_nsec) / 1e9;
}


void write_primes(int rl, int rh) {
    FILE *fp = fopen("result.txt", "w");
    if (!fp) {
        perror("result.txt");
        return;
    }

    for (int i = rl; i <= rh; i++)
        if (is_prime(i))
            fprintf(fp, "%d\n", i);

    fclose(fp);
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <rl> <rh>\n", argv[0]);
        return 1;
    }

    int rl = atoi(argv[1]);
    int rh = atoi(argv[2]);

    int max_cores = sysconf(_SC_NPROCESSORS_ONLN);
    printf("Detected logical CPUs: %d\n", max_cores);


    FILE *csv = fopen("results.csv", "a+");
    if (!csv) {
        perror("results.csv");
        return 1;
    }

    fseek(csv, 0, SEEK_END);
    if (ftell(csv) == 0) {
        fprintf(csv, "n,time_seconds\n");
    }

    for (int n = 1; n <= max_cores; n++) {
        double t = run_parallel(rl, rh, n);
        printf("n = %d | time = %.6f sec\n", n, t);
        fprintf(csv, "%d,%.6f\n", n, t);
        fflush(csv);
    }

    fclose(csv);


    write_primes(rl, rh);

    printf("Timing saved in results.csv\n");
    printf("Prime numbers saved in result.txt\n");

    return 0;
}

