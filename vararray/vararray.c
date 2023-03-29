#include "vararray.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <assert.h>

// COMPLETE ME
struct vararray_t {
    unsigned int size;
    void * user;
    void ** data;
    void (*destroy)(void * element, void * user);
};

vararray_handle vararray_create_with_destroy(vararray_destroy_func_t func,
        void * userdata) {
            if (!func)
                return VARARRAY_INVALID_HANDLE;
            
            vararray_handle v = vararray_create();
            v->destroy = func;
            v->user = userdata;
            return v;
}

vararray_handle vararray_create() {
    vararray_handle v = (vararray_handle)malloc(sizeof(struct vararray_t));
    if (!v) 
        return NULL;
    v->size = 0;
    v->user = NULL;
    v->data = NULL;
    v->destroy = NULL;
    return v;
}

void vararray_clear(vararray_handle h) {
    if (!h->destroy)
        for (unsigned int i = 0; i < h->size; i++)
            h->destroy(h->data[i], h->user);

    if (h->data)
        free(h->data);
    h->data = NULL;
    return;
}

unsigned int vararray_size(const_vararray_handle h) {
    return h->size;
}

void vararray_push_back(vararray_handle h, void * element) {
    h->data = (void**)realloc(h->data, sizeof(void *) * (h->size + 1));
    h->data[h->size] = element;
    h->size++;
}

void * vararray_get(const_vararray_handle h, unsigned int idx) {
    if (idx < 0 || idx >= vararray_size(h))
        return NULL;
    
    return h->data[idx];
}

bool vararray_set(vararray_handle h, unsigned int idx, void * element) {
    if (idx < 0 || idx >= vararray_size(h))
        return false;
    
    h->data[idx] = element;
    return true;
}

unsigned int vararray_visit(const_vararray_handle h,
        vararray_visit_func_t visitfunc, void * visitdata) {
            assert(visitfunc != NULL);
            if (vararray_size(h) == 0)
                return 0;
            
            unsigned int i = 0;
            while (i < h->size && visitfunc(i, h->data[i], visitdata))
                i++;
            
            return i;
}

void vararray_destroy(vararray_handle h) {
    vararray_clear(h);
    free(h);
    h = NULL;
}

// int main() {
//     vararray_handle t = (vararray_handle)malloc(sizeof(struct vararray_t));
//     t->data = (void**)malloc(sizeof(void*)*5);
//     for (int i = 0; i < 5; i++)
//         vararray_push_back(t, (void*)(intptr_t)i);
//     t->size = 5;
//     for (int i = 0; i < 5; i++)
//         printf("%ld ", (intptr_t)t->data[i]);
//     printf("\n");

//     t->data = (void**)realloc(t->data, sizeof(void*) * t->size+1);
//     t->size++;
//     t->data[5] = (void *)5;
//     for (int i = 0; i < t->size; i++)
//         printf("%ld ", (intptr_t)t->data[i]);
//     printf("\n");
//     return 0;
// }