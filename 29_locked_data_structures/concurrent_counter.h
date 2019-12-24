#ifndef __CONCURRENT_COUNTER_H__
#define __CONCURRENT_COUNTER_H__

#include <pthread.h>

#include "mythreads.h"

typedef struct __counter_t {
    int val;
    pthread_mutex_t lock;
} counter_t;

void Counter_Init(counter_t* counter) {
    counter->val = 0;
    Pthread_mutex_init(&counter->lock, NULL);
}

void Counter_Increment(counter_t* counter) {
    Pthread_mutex_lock(&counter->lock);
    counter->val = counter->val + 1;
    Pthread_mutex_unlock(&counter->lock);
}

void Counter_Decrement(counter_t* counter) {
    Pthread_mutex_lock(&counter->lock);
    counter->val = counter->val - 1;
    Pthread_mutex_unlock(&counter->lock);
}

int Counter_Get(counter_t* counter) {
    Pthread_mutex_lock(&counter->lock);
    int rc = counter->val;
    Pthread_mutex_unlock(&counter->lock);
    return rc;
}

void Counter_Set(counter_t* counter, int val) {
    Pthread_mutex_lock(&counter->lock);
    counter->val = val;
    Pthread_mutex_unlock(&counter->lock);
}

#endif // __CONCURRENT_COUNTER_H__