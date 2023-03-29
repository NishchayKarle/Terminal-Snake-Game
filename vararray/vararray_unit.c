// A cunit test for vararray.
//
// Make sure to have a separate test function FOR EACH FUNCTION IN VARARRAY.H
//
#include "vararray.h"
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <stdint.h>
#include <CUnit/Basic.h>

static int init_suite()
{
    return 0;
}

static int clean_suite()
{
    return 0;
}

void test_destroy_func(void * element, void * user) {
    free(element);
    return;
}

bool test_visit_func_1(unsigned int idx, void * data, void * visitdata) {
    return false;
}

bool test_visit_func_2(unsigned int idx, void * data, void * visitdata) {
    return true;
}

static void test_vararray_create_with_destroy() {
    char * a = (char *)malloc(sizeof(*a));
    *a = 'a';
    vararray_handle v = vararray_create_with_destroy(NULL, a);
    CU_ASSERT_EQUAL(v == VARARRAY_INVALID_HANDLE, true);
    vararray_handle v1 = vararray_create_with_destroy(&test_destroy_func, a);
    CU_ASSERT_EQUAL(v1 != VARARRAY_INVALID_HANDLE, true);
    free(a);
    a = NULL;
    vararray_destroy(v1);
}

static void test_vararray_create() {
    vararray_handle v = vararray_create();
    CU_ASSERT_EQUAL(v != NULL, true);
    vararray_destroy(v);
}

static void test_vararray_size() {
    char * a = (char *)malloc(sizeof(*a));
    *a = 'a';
    vararray_handle v = vararray_create_with_destroy(&test_destroy_func, a);
    unsigned int size = vararray_size(v);
    CU_ASSERT_EQUAL(size == 0, true);

    int val = 5, val2 = 6;
    vararray_push_back(v, &val);
    size = vararray_size(v);
    CU_ASSERT_EQUAL(size == 1, true);

    vararray_push_back(v, &val2);
    size = vararray_size(v);
    CU_ASSERT_EQUAL(size == 2, true);

    char val3 = 'a', val4 = 'b';
    vararray_push_back(v, &val3);
    size = vararray_size(v);
    CU_ASSERT_EQUAL(size == 3, true);

    vararray_push_back(v, &val4);
    size = vararray_size(v);
    CU_ASSERT_EQUAL(size == 4, true);

    free(a);
    a = NULL;
    vararray_destroy(v);
}

static void test_vararray_push_back() {
    char * a = (char *)malloc(sizeof(*a));
    *a = 'a';
    vararray_handle v = vararray_create_with_destroy(&test_destroy_func, a);

    int val = 5, val2 = 6;
    vararray_push_back(v, (void*)(intptr_t)val);
    CU_ASSERT_EQUAL((int)(intptr_t)vararray_get(v, 0) == val, true);

    vararray_push_back(v, (void*)(intptr_t)val2);
    CU_ASSERT_EQUAL((int)(intptr_t)vararray_get(v, 1) == val2, true);

    char val3 = 'a';
    vararray_push_back(v, (void*)(intptr_t)val3);
    CU_ASSERT_EQUAL((char)(intptr_t)vararray_get(v, 2) == val3, true);

    free(a);
    a = NULL;
    vararray_destroy(v);
}

static void test_vararray_get() {
    char * a = (char *)malloc(sizeof(*a));
    *a = 'a';
    vararray_handle v = vararray_create_with_destroy(&test_destroy_func, a);

    int val = 5, val2 = 6;
    vararray_push_back(v, (void*)(intptr_t)val);
    CU_ASSERT_EQUAL((int)(intptr_t)vararray_get(v, 0) == val, true);

    vararray_push_back(v, (void*)(intptr_t)val2);
    CU_ASSERT_EQUAL((int)(intptr_t)vararray_get(v, 1) == val2, true);

    char val3 = 'a';
    vararray_push_back(v, (void*)(intptr_t)val3);
    CU_ASSERT_EQUAL((char)(intptr_t)vararray_get(v, 2) == val3, true);

    free(a);
    a = NULL;
    vararray_destroy(v);
}

static void test_vararray_set() {
    char * a = (char *)malloc(sizeof(*a));
    *a = 'a';
    vararray_handle v = vararray_create_with_destroy(&test_destroy_func, a);

    int val = 5, val2 = 6;
    vararray_push_back(v, (void*)(intptr_t)val);
    vararray_set(v, 0, (void*)(intptr_t)val2);
    CU_ASSERT_EQUAL((int)(intptr_t)vararray_get(v, 0) == val2, true);

    vararray_push_back(v, (void*)(intptr_t)val2);
    vararray_set(v, 1, (void*)(intptr_t)val);
    CU_ASSERT_EQUAL((int)(intptr_t)vararray_get(v, 1) == val, true);

    char val3 = 'a', val4 = 'b';
    vararray_push_back(v, (void*)(intptr_t)val3);
    vararray_set(v, 2, (void*)(intptr_t)val4);
    CU_ASSERT_EQUAL((char)(intptr_t)vararray_get(v, 2) == val4, true);

    free(a);
    a = NULL;
    vararray_destroy(v);
}

static void test_vararray_visit() {
    char * a = (char *)malloc(sizeof(*a));
    *a = 'a';

    vararray_handle v = vararray_create_with_destroy(&test_destroy_func, a);

    int val = 5, val2 = 6;
    char val3 = 'a';
    vararray_push_back(v, (void*)(intptr_t)val);
    CU_ASSERT_EQUAL(vararray_visit(v, &test_visit_func_1, a) == 0, true);

    vararray_push_back(v, (void*)(intptr_t)val2);
    CU_ASSERT_EQUAL(vararray_visit(v, &test_visit_func_2, a) == 2, true);

    vararray_push_back(v, (void*)(intptr_t)val3);
    CU_ASSERT_EQUAL(vararray_visit(v, &test_visit_func_2, a) == 3, true);

    free(a);
    a = NULL;
    vararray_destroy(v);
}

int main(int argc, char** args) {
    srand(getpid());
    CU_pSuite pSuite = NULL;
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    pSuite = CU_add_suite("vararray unit test", init_suite, clean_suite);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if ( (NULL == CU_add_test(pSuite, "vararray_create_with_destroy", 
            test_vararray_create_with_destroy)) || 
        (NULL == CU_add_test(pSuite, "vararray_create", 
            test_vararray_create)) || 
        (NULL == CU_add_test(pSuite, "vararray_size", 
            test_vararray_size)) || 
        (NULL == CU_add_test(pSuite, "vararray_push_back",
            test_vararray_push_back)) ||
        (NULL == CU_add_test(pSuite, "vararray_get",
            test_vararray_get)) || 
        (NULL == CU_add_test(pSuite, "vararray_set",
            test_vararray_set)) || 
        (NULL == CU_add_test(pSuite, "vararray_visit",
            test_vararray_visit))
        ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}