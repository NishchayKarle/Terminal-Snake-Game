#include "cfifo.h"
#include <stdlib.h>

// COMPLETE ME
struct Node {
    coordinate_t * point;
    struct Node * next;
};
typedef struct Node * node_handle;

node_handle create_node(const coordinate_t * c) {
    node_handle node = (node_handle)malloc(sizeof(struct Node));
    node->point = (coordinate_t *)malloc(sizeof(coordinate_t));
    node->point->x = c->x;
    node->point->y = c->y;
    node->next = NULL;
    return node;
}

struct cfifo_impl_t {
    unsigned int size;
    node_handle head;
    node_handle tail;
};

cfifo_handle cfifo_create() {
    cfifo_handle h = (cfifo_handle)malloc(sizeof(struct cfifo_impl_t));
    h->size = 0;
    h->head = NULL;
    h->tail = NULL;
    return h;
}

void cfifo_destroy(cfifo_handle h) {
    if (h->head) {
        node_handle temp = h->head;
        for (unsigned int i = 0; i < h->size; i++) {
            node_handle temp2 = temp->next;
            free(temp->point);
            temp->point = NULL;
            temp->next = NULL;
            free(temp);
            temp = temp2;
        }
        h->head = NULL;
        h->tail = NULL;
    }
    free(h);
    h = NULL;
}

unsigned int cfifo_size(const_cfifo_handle h) {
    return h->size;
}

bool cfifo_empty(const_cfifo_handle h) {
    return h->size == 0 ? true : false;
}

void cfifo_push(cfifo_handle h, const coordinate_t * c) {
    node_handle node = create_node(c);
    h->size++;
    if (!h->head) {
        h->head = node;
        h->tail = node;
        return;
    }

    h->tail->next = node;
    h->tail = node;
    return;
}

coordinate_t cfifo_pop(cfifo_handle h) {
    node_handle temp = h->head;
    coordinate_t c = *(temp->point);

    h->head = h->head->next;
    h->size--;

    temp->next = NULL;
    free(temp->point);
    temp->point = NULL;
    free(temp);
    return c;
}

const coordinate_t * cfifo_head(const_cfifo_handle h) {
    if (!h->head)
        return NULL;
    
    return h->tail->point;
}

unsigned int cfifo_visit(const_cfifo_handle h, cfifo_visit_func_t visit,
        void * userdata) {
            unsigned int i = 0;
            node_handle temp = h->head;
            while (i < h->size) {
                i++;
                if (!visit(temp->point, userdata))
                    break;
                temp = temp->next;
            }
            return i;
}