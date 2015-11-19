#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "index.h"

struct index_ {
    /* to be implemented */
};

int index_create(char* path) {
    /* to be implemented */
}

int index_save(index_t *index, char* path) {
    /* to be implemented */
}

index_t* index_open(char* path) {
    /* to be implemented */
}

int index_put(index_t *index, int key, long pos) {
    /* to be implemented */
}

void index_get(index_t *index, int key, long** offsets, int* noffsets) {
    /* to be implemented */
}

void index_close(index_t *index) {
    /* to be implemented */
}
