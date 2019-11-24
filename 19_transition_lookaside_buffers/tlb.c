#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sched.h>


int main(int argc, char* argv[]) {

    /*
        set this process to run on only one CPU
    */
    cpu_set_t set;
    CPU_ZERO(&set);
    CPU_SET(0, &set);

    if (sched_setaffinity(getpid(), sizeof(cpu_set_t), &set) == -1) {
        printf("Set CPU affinity error\n");
        exit(1);
    }

    /*
        get the command line args
    */
    int NUMPAGES;
    int TRIALS;

    if (argc == 1) {
        NUMPAGES = 5;
        TRIALS = 100000;
    } else if (argc == 2) {
        NUMPAGES = atoi(argv[1]);
        TRIALS = 100000;
    } else {
        NUMPAGES = atoi(argv[1]);
        TRIALS = atoi(argv[2]);
    }


    /*
        calculate page size, and the jump, and initialize the array
        (jump is number of array spots to jump)
    */
    int PAGESIZE = getpagesize();
    int jump = PAGESIZE / sizeof(int);
    int *a = calloc(NUMPAGES * jump, sizeof(int));


    /*
        start the benchmark
    */
    struct timeval start;
    struct timeval end;

    gettimeofday(&start, NULL);

    for (size_t j = 0; j < TRIALS; j++) {
        for (int i = 0; i < NUMPAGES * jump; i += jump) {
            a[i] += 1;
        }
    }

    gettimeofday(&end, NULL);

    /*
        calculate time taken and print
    */
    int nloops = TRIALS * NUMPAGES;

    // this is the time taken in nanoseconds
    float time_taken = (float)((end.tv_sec * 1000000 + end.tv_usec) - (start.tv_sec * 1000000 + start.tv_usec)) * 1000 / nloops;

    printf("%f\n", time_taken);

    /*
        free resources
    */
    free(a);

    return 0;
}