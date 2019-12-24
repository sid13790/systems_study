#define _GNU_SOURCE

#include <stdio.h>
#include <sched.h>
#include "mythreads.h"
#include "better_concurrent_counter.h"

int COUNT = 10000000;

typedef struct __myarg_t {
    counter_t* c;
    int threads;
} myarg_t;

void* increment_counter(void* myargs) {

    myarg_t* m = (myarg_t*) myargs;

    int thread_id = sched_getcpu();
    for (int i = 0; i < COUNT / m->threads; i++) {
        Counter_Increment((counter_t*)m->c, thread_id);
    }
    return NULL;
}

int main() {

    counter_t counter;

    int threshold = 10000;


    Counter_Init(&counter, threshold);

    for (int i = 1; i <= 5; i++) {
        Counter_Flush(&counter);

        pthread_t threads[i];

        double start_time = Time_GetSeconds();

        myarg_t args;
        args.c = &counter;
        args.threads = i;

        for (int j = 0; j < i; j++) {
            Pthread_create(&threads[j], NULL, &increment_counter, &args);
        }

        for (int j = 0; j < i; j++) {
            Pthread_join(threads[j], NULL);
        }

        double end_time = Time_GetSeconds();

        printf("it took: %f seconds to run %d threads\n", end_time - start_time, i);
    }


    return 0;
}