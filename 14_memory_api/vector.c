#include <stdlib.h>
#include <stdio.h>

typedef struct Vector {
    int size;  // number of elements
    int capacity; // capacity of list
    int* vec; // pointer to actual vector
} vector;

vector init(int capacity) {
    vector res = { 0 , capacity, calloc(capacity, sizeof(int))};
    return res;
}

void free_vector(vector* v) {
    free(v->vec);
    v->size = 0;
    v->capacity = 0;
}

void add(vector* v, int elem) {
    if (v->size == v->capacity) {
        v->vec = realloc(v->vec, v->capacity * 2 * sizeof(int));
    }
    (v->vec)[v->size++] = elem;
}

int get(vector* v, int idx) {
    if (idx >= v->size || idx < 0) {
        return -1;
    }
    return *(v->vec + idx);
}

int main(int argc, char* argv[]) {
    vector v = init(10);
    add(&v, 1);
    add(&v, 2);
    int test = get(&v, 1);
    printf("testing this: %d\n", test);

    free_vector(&v);

    return 0;
}