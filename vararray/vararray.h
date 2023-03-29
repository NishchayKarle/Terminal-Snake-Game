#pragma once

#include <stdbool.h>

// A convenient array abstraction holding void * pointers

struct vararray_t;

typedef struct vararray_t * vararray_handle;

typedef const struct vararray_t * const_vararray_handle;

#define VARARRAY_INVALID_HANDLE ((vararray_handle) 0)

typedef bool (*vararray_visit_func_t)(unsigned int idx, void * data,
        void * visitdata);

typedef void (*vararray_destroy_func_t)(void * element, void * user);


// Create a new vararray; func should not be NULL and point to a function
// which will be called whenever an element of the vararray is removed.
// userdata is not used by the vararray implementation but is passed through
// to the destroy func unmodified whenever that function is called.
//
// On error (such as func == NULL) returns VARARRAY_INVALID_HANDLE,
// otherwise returs a valid vararray handle.
vararray_handle vararray_create_with_destroy(vararray_destroy_func_t func,
        void * userdata);

// Same as create_with_cleanup creates a varray without destroy function;
// (acts as if the vararray was created with a no-operation (i.e. do nothing)
// destroy function.
vararray_handle vararray_create();


// Removes all the elements from the array; Calls the user-provided destroy
// function for each element in the vararray (if the provided function pointer 
// during create was not NULL).
void vararray_clear(vararray_handle h);

// returns the number of elements in the array
unsigned int vararray_size(const_vararray_handle h);

// Add an element to the back of the array
void vararray_push_back(vararray_handle h, void * element);

// Access element I (0 <= I < vararray_size())
// Your implementation should be able to access any index in O(1) time.
void * vararray_get(const_vararray_handle h, unsigned int idx);

// Replaces the element at index idx. The destroy function is NOT invoked
// for the element being replaced.
//
// Returns false if the index was out of bounds, true otherwise
//
// Your implementation should set/get any index in O(1) time.
bool vararray_set(vararray_handle h, unsigned int idx, void * element);


// Calls visitfunc (which is not allowed to be NULL) for each element
// in the vararray *in order* of their index (i.e. starting with index 0,
// followed by index 1, etc. up to index vararray_size()-1)
// AS LONG AS visitfunc returns true.
//
// The return value is the first index for which visitfunc returned false,
// or varray_size() if visitfunc returned true for each element in the
// vararray.
// (Or, put another way, the return value is the number of times the
// visit function was called)
//
// If the vararray is empty, varray_visit returns 0.
//
// The visitdata value is not used by varray_visit and passed through
// unmodified to the visitfunc.
unsigned int vararray_visit(const_vararray_handle h,
        vararray_visit_func_t visitfunc, void * visitdata);

// Destroy vararray. If a destroy function was provided at create time,
// calls the destroy function on each element in unspecified order.
//
// The vararray_handle is no longer valid after this call returns and should
// no longer be used.
void vararray_destroy(vararray_handle h);

