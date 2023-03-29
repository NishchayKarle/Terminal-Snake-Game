// cfifo unit test
//
#include "cfifo.h"
#include <unistd.h>
#include <assert.h>
#include <stdlib.h>
#include <CUnit/Basic.h>

static int init_suite()
{
    return 0;
}

static int clean_suite()
{
    return 0;
}

static void test_cfifo_create() {
    cfifo_handle c = cfifo_create();
    CU_ASSERT_EQUAL(c != NULL, true);
    cfifo_destroy(c);
}

static void test_cfifo_size() {
    cfifo_handle c = cfifo_create();

    CU_ASSERT_EQUAL(cfifo_size(c) == 0, true);

    coordinate_t * point1 = (coordinate_t *)malloc(sizeof(*point1));
    point1->x = 5;
    point1->y = 6;
    cfifo_push(c, point1);
    CU_ASSERT_EQUAL(cfifo_size(c) == 1, true);

    coordinate_t * point2 = (coordinate_t *)malloc(sizeof(*point2));
    point2->x = 5;
    point2->y = 6;
    cfifo_push(c, point2);
    CU_ASSERT_EQUAL(cfifo_size(c) == 2, true);

    coordinate_t * point3 = (coordinate_t *)malloc(sizeof(*point3));
    point3->x = 5;
    point3->y = 6;
    cfifo_push(c, point3);
    CU_ASSERT_EQUAL(cfifo_size(c) == 3, true);

    free(point1);
    free(point2);
    free(point3);
    cfifo_destroy(c);
}

static void test_cfifo_empty() {
    cfifo_handle c = cfifo_create();
    
    CU_ASSERT_EQUAL(cfifo_empty(c) == true, true);

    coordinate_t * point1 = (coordinate_t *)malloc(sizeof(*point1));
    point1->x = 5;
    point1->y = 6;
    cfifo_push(c, point1);
    CU_ASSERT_EQUAL(cfifo_empty(c) == false, true);

    free(point1);
    cfifo_destroy(c);
}

static void test_cfifo_push() {
    cfifo_handle c = cfifo_create();

    coordinate_t * point1 = (coordinate_t *)malloc(sizeof(*point1));
    point1->x = 5;
    point1->y = 6;
    cfifo_push(c, point1);
    const coordinate_t * head = cfifo_head(c);
    CU_ASSERT_EQUAL((head->x == point1->x && head->y == point1->y) == true, true);

    coordinate_t * point2 = (coordinate_t *)malloc(sizeof(*point2));
    point2->x = 5;
    point2->y = 6;
    cfifo_push(c, point2);
    head = cfifo_head(c);
    CU_ASSERT_EQUAL((head->x == point2->x && head->y == point2->y) == true, true);

    coordinate_t * point3 = (coordinate_t *)malloc(sizeof(*point3));
    point3->x = 5;
    point3->y = 6;
    cfifo_push(c, point3);
    head = cfifo_head(c);
    CU_ASSERT_EQUAL((head->x == point3->x && head->y == point3->y) == true, true);

    free(point1);
    free(point2);
    free(point3);
    cfifo_destroy(c);
}

static void test_cfifo_pop() {
    cfifo_handle c = cfifo_create();

    coordinate_t * point1 = (coordinate_t *)malloc(sizeof(*point1));
    point1->x = 5;
    point1->y = 6;
    coordinate_t * point2 = (coordinate_t *)malloc(sizeof(*point2));
    point2->x = 5;
    point2->y = 6;
    coordinate_t * point3 = (coordinate_t *)malloc(sizeof(*point3));
    point3->x = 5;
    point3->y = 6;

    cfifo_push(c, point1);
    cfifo_push(c, point2);
    cfifo_push(c, point3);

    const coordinate_t * head = cfifo_head(c);
    head = cfifo_head(c);
    CU_ASSERT_EQUAL((head->x == point3->x && head->y == point3->y) == true, true);
    cfifo_pop(c);
    CU_ASSERT_EQUAL((head->x == point2->x && head->y == point2->y) == true, true);
    cfifo_pop(c);
    CU_ASSERT_EQUAL((head->x == point1->x && head->y == point1->y) == true, true);
    cfifo_pop(c);

    free(point1);
    free(point2);
    free(point3);
    cfifo_destroy(c);
}

static void test_cfifo_head() {
    cfifo_handle c = cfifo_create();

    coordinate_t * point1 = (coordinate_t *)malloc(sizeof(*point1));
    point1->x = 5;
    point1->y = 6;
    coordinate_t * point2 = (coordinate_t *)malloc(sizeof(*point2));
    point2->x = 5;
    point2->y = 6;
    coordinate_t * point3 = (coordinate_t *)malloc(sizeof(*point3));
    point3->x = 5;
    point3->y = 6;

    const coordinate_t * head = cfifo_head(c);
    head = cfifo_head(c);
    CU_ASSERT_EQUAL(head == NULL, true);

    cfifo_push(c, point1);
    head = cfifo_head(c);
    CU_ASSERT_EQUAL((head->x == point1->x && head->y == point1->y) == true, true);

    cfifo_push(c, point2);
    head = cfifo_head(c);
    CU_ASSERT_EQUAL((head->x == point2->x && head->y == point2->y) == true, true);

    cfifo_push(c, point3);
    head = cfifo_head(c);
    CU_ASSERT_EQUAL((head->x == point3->x && head->y == point3->y) == true, true);

    free(point1);
    free(point2);
    free(point3);
    cfifo_destroy(c);
}

static void test_cfifo_visit() {

}

int main(int argc, char** args) {
    srand(getpid());
    CU_pSuite pSuite = NULL;
    if (CUE_SUCCESS != CU_initialize_registry())
        return CU_get_error();

    pSuite = CU_add_suite("cfifo unit test", init_suite, clean_suite);
    if (NULL == pSuite)
    {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if ( (NULL == CU_add_test(pSuite, "cfifo_create", test_cfifo_create)) || 
         (NULL == CU_add_test(pSuite, "cfifo_size", test_cfifo_size)) || 
         (NULL == CU_add_test(pSuite, "cfifo_empty", test_cfifo_empty)) ||
         (NULL == CU_add_test(pSuite, "cfifo_push", test_cfifo_push)) ||
         (NULL == CU_add_test(pSuite, "cfifo_pop", test_cfifo_pop)) || 
         (NULL == CU_add_test(pSuite, "cfifo_head", test_cfifo_head)) ||
         (NULL == CU_add_test(pSuite, "cfifo_visit", test_cfifo_visit))
        ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}