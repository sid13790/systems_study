#ifndef __HAND_OVER_HAND_LL_H__
#define __HAND_OVER_HAND_LL_H__

#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "mythreads.h"

typedef struct __node_t {
    int key;
    struct __node_t* next;
    pthread_mutex_t lock;
} node_t;

typedef struct __list_t {
    node_t* head;
} list_t;

void List_Init(list_t* L) {
    L->head = NULL;
}

void List_Insert(list_t* L, int key) {
    node_t* _node = (node_t*)calloc(1, sizeof(node_t));

    if (_node == NULL) {
        perror("malloc");
        return;
    }

    Pthread_mutex_lock(&L->head->lock);
    _node->next = L->head;
    L->head = _node;
    Pthread_mutex_unlock(&L->head->next->lock);
}

int List_Lookup(list_t* L, int key) {
    Pthread_mutex_lock(&L->head->lock);
    node_t* curr = L->head;
    while (curr) {
        if (curr->key == key) {
            Pthread_mutex_unlock(&curr->lock);
            return 0;
        }
        if (curr->next != NULL) {
            Pthread_mutex_lock(&curr->next->lock);
        }
        Pthread_mutex_unlock(&curr->lock);
        curr = curr->next;
    }
    return -1;
}

#endif // __HAND_OVER_HAND_LL_H__