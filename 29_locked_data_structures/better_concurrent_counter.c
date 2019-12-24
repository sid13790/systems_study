#define _GNU_SOURCE

#include <stdio.h>
#include <sched.h>
#include "mythreads.h"
#include "better_concurrent_counter.h"

int COUNT = 10000000;

void* increment_counter(void* counter) {
    int thread_id = sched_getcpu();
    for (int i = 0; i < COUNT; i++) {
        Counter_Increment((counter_t*)counter, thread_id);
    }
    return NULL;
}

int main() {

    counter_t counter;

    int threshold = 1000;


    Counter_Init(&counter, threshold);

    for (int i = 1; i <= 5; i++) {
        Counter_Flush(&counter);

        pthread_t threads[i];

        double start_time = Time_GetSeconds();

        for (int j = 0; j < i; j++) {
            Pthread_create(&threads[j], NULL, &increment_counter, &counter);
        }

        for (int j = 0; j < i; j++) {
            Pthread_join(threads[j], NULL);
        }

        double end_time = Time_GetSeconds();

        printf("it took: %f seconds to run %d threads\n", end_time - start_time, i);
    }


    return 0;
}