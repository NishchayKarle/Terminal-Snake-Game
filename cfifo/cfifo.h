#pragma once

// A FIFO data structure which stores a specific type (coordinate_t)
//
// !!!! IT MUST BE IMPLEMENTED USING A LINKED LIST (FOR PEDAGOGICAL REASONS) !!!!!
//
// You can decide on the rest of the implementation details
// (doubly linked list or not, etc.)
//
// HINT (it is possible to implement all of the functions below _efficiently_
// using a singly linked list)


#include "coordinate.h"

#include <stdbool.h>

struct cfifo_impl_t;

typedef struct cfifo_impl_t * cfifo_handle;
typedef const struct cfifo_impl_t * const_cfifo_handle;

cfifo_handle cfifo_create();

void cfifo_destroy(cfifo_handle h);


// Your cfifo_size function should be O(1) -- i.e. the execution time
// cannot depend on the number of elements in the FIFO.
// You can assume the handle is valid.
unsigned int cfifo_size(const_cfifo_handle h);

// Returns true if size == 0, false otherwise
// The time complexity for this function should be O(1)
// You can assume the handle is valid.
bool cfifo_empty(const_cfifo_handle h);

// Add new coordinate to the FIFO structure
// The coordinate pointed to by c is copied into the FIFO;
//
// The time complexity for this function should be O(1)
// You can assume the handle is valid.
void cfifo_push(cfifo_handle h, const coordinate_t * c);

// Remove oldest (first added) coordinate from the FIFO structure and returns
// it.
//
// The time complexity for this function should be O(1)
// You can assume the handle is valid.
coordinate_t cfifo_pop(cfifo_handle h);


// If not empty: returns a pointer to the last added(pushed) coordinate
// If empty: returns NULL
// The caller must not free the returned pointer. Ownership of the memory
// remains with the cfifo.
//
// The time complexity for this function should be O(1)
// You can assume the handle is valid.
const coordinate_t * cfifo_head(const_cfifo_handle h);

typedef bool (*cfifo_visit_func_t)(const coordinate_t * c, void * userdata);

// Call the visit function (passing it a coordinate as well as the userdata
// pointer passed to this function) for each element in the FIFO.
//
// !! The visit function must visit the elements in the FIFO from OLDEST to NEWEST.
// !! (i.e. the first call to the visit function will be the element which would
// !!  be removed by a call to cfifo_pop)
//
// If the visit function returns false, visitation stops right away.
// The cfifo_visit function returns the number of elements visited
// (i.e. number of times the visit function was called).
//
// The time complexity for this function should be O(n)
// You can assume the handle is valid.
unsigned int cfifo_visit(const_cfifo_handle h, cfifo_visit_func_t visit,
        void * userdata);

