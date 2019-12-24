#ifndef __BETTER_CONCURRENT_COUNTER_H__
#define __BETTER_CONCURRENT_COUNTER_H__

#include <pthread.h>

#include "mythreads.h"

#define NUM_CPUS 4

typedef struct __counter_t {
    int global;
    pthread_mutex_t glock;
    int locals[NUM_CPUS];
    pthread_mutex_t llock[NUM_CPUS];
    int threshold;
} counter_t;

void Counter_Init(counter_t* c, int threshold) {
    c->threshold = threshold;
    c->global = 0;
    Pthread_mutex_init(&c->glock, NULL);

    for (int i = 0; i < NUM_CPUS; i++) {
        c->locals[i] = 0;
        Pthread_mutex_init(&c->llock[i], NULL);
    }
}

void Counter_Increment(counter_t* c, int thread_id) {

    int cpu = thread_id % NUM_CPUS;

    Pthread_mutex_lock(&c->llock[cpu]);
    
    c->locals[cpu]++;

    if (c->locals[cpu] >= c->threshold) {
        Pthread_mutex_lock(&c->glock);
        c->global += c->locals[cpu];
        Pthread_mutex_unlock(&c->glock);
        c->locals[cpu] = 0; 
    }

    Pthread_mutex_unlock(&c->llock[cpu]);
}

int Counter_Get(counter_t* c) {
    Pthread_mutex_lock(&c->glock);
    int rc = c->global;
    Pthread_mutex_unlock(&c->glock);
    return rc;
}

void Counter_Flush(counter_t* c) {

    for (int i = 0; i < NUM_CPUS; i++) {
        Pthread_mutex_lock(&c->llock[i]);
        c->locals[i] = 0;
        Pthread_mutex_unlock(&c->llock[i]);
    }

    Pthread_mutex_lock(&c->glock);
    c->global = 0;
    Pthread_mutex_unlock(&c->glock);
}


#endif // __BETTER_CONCURRENT_COUNTER_H__