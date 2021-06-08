#include <assert.h>
#include <stdlib.h>
#include <time.h>
#include "cstl_lib.h"


static void test_with_int()
{
    int capacity = 1;
    int size = 100;
    void *elem = (void *)0;

    struct cstl_queue *my_queue = new_cstl_queue(capacity, NULL, NULL, cstl_true);

    assert(cstl_true == empty_cstl_queue(my_queue));
    assert(0 == size_cstl_queue(my_queue));

    for (int i = 0; i < size; i++)
        assert(CSTL_SUCCESS == push_cstl_queue(my_queue, &i, sizeof(int)));

    assert(cstl_false == empty_cstl_queue(my_queue));
    assert(size == size_cstl_queue(my_queue));

    for (int i = 0; i < size; i++) {
        assert(CSTL_SUCCESS == top_cstl_queue(my_queue, &elem));
        assert(*(int *)elem == i);
        free(elem);
        assert(CSTL_SUCCESS == pop_cstl_queue(my_queue));
    }

    assert(cstl_true == empty_cstl_queue(my_queue));
    assert(0 == size_cstl_queue(my_queue));

    assert(CSTL_SUCCESS == delete_cstl_queue(my_queue));

    my_queue = new_cstl_queue(capacity, NULL, NULL, cstl_true); // test stability
    time_t t;
    srand((unsigned) time(&t));
    for (int i = 0; i < size; i++) {
        for (int j = 0; j < rand() % 1000 + 1; j++) {
            push_cstl_queue(my_queue, &j, sizeof(int));
        }
        for (int j = 0; j < rand() % 1000 + 1; j++) {
            top_cstl_queue(my_queue, &elem);
            if (elem) {
                free(elem);
                elem = (void *)0;
            }
            pop_cstl_queue(my_queue);
        }
    }

    assert(CSTL_SUCCESS == delete_cstl_queue(my_queue));
}

typedef struct {
    int num;
    char name[10];
}test_queue_struct_t;

static char *char_value[] = {"zero", "one", "two", "three", "four", "five",
                    "six", "seven", "eight", "nine", "ten"};
static int int_value[] = {0,1,2,3,4,5,6,7,8,9,10};

static void test_with_pointers()
{
    int size = 1;
    void *elem = (void *)0;
    test_queue_struct_t test_struct[11];
    for (int i = 0; i < sizeof(test_struct)/sizeof(test_queue_struct_t); i++) {
        test_struct[i].num = int_value[i];
        snprintf(test_struct[i].name, sizeof(test_struct[i].name), "%s", char_value[i]); 
    }

    struct cstl_queue *my_queue = new_cstl_queue(size, NULL, NULL, cstl_true);

    assert(cstl_true == empty_cstl_queue(my_queue));
    assert(0 == size_cstl_queue(my_queue));

    size = sizeof(test_struct)/sizeof(test_queue_struct_t);

    for (int i = 0; i < size; i++)
        assert(CSTL_SUCCESS == push_cstl_queue(my_queue, (void *)&test_struct[i], sizeof(test_queue_struct_t)));

    assert(cstl_false == empty_cstl_queue(my_queue));
    assert(size == size_cstl_queue(my_queue));

    for (int i = 0; i < size; i++) {
        assert(CSTL_SUCCESS == top_cstl_queue(my_queue, &elem));
        assert(((test_queue_struct_t *)elem)->num == int_value[i]);
        assert(strcmp(((test_queue_struct_t *)elem)->name, char_value[i]) == 0);
        assert(CSTL_SUCCESS == pop_cstl_queue(my_queue));
        free(elem);
    }

    assert(cstl_true == empty_cstl_queue(my_queue));
    assert(0 == size_cstl_queue(my_queue));

    assert(CSTL_SUCCESS == delete_cstl_queue(my_queue));
}

static void test_without_auto_grow()
{
    int capacity = 1024;
    void *elem = (void *)0;
    struct cstl_queue *my_queue = new_cstl_queue(1024, NULL, NULL, cstl_false);

    assert(cstl_true == empty_cstl_queue(my_queue));
    assert(0 == size_cstl_queue(my_queue));
    assert(cstl_false == full_cstl_queue(my_queue));

    for (int i = 0; i < capacity; i++)
        assert(CSTL_SUCCESS == push_cstl_queue(my_queue, &i, sizeof(int)));

    assert(cstl_true == full_cstl_queue(my_queue));

    assert(CSTL_QUEUE_IS_FULL == push_cstl_queue(my_queue, &capacity, sizeof(int)));

    assert(cstl_false == empty_cstl_queue(my_queue));
    assert(capacity == size_cstl_queue(my_queue));

    for (int i = 0; i < capacity; i++) {
        assert(CSTL_SUCCESS == top_cstl_queue(my_queue, &elem));
        assert(*(int *)elem == i);
        free(elem);
        assert(CSTL_SUCCESS == pop_cstl_queue(my_queue));
    }

    assert(cstl_true == empty_cstl_queue(my_queue));
    assert(0 == size_cstl_queue(my_queue));

    assert(CSTL_SUCCESS == delete_cstl_queue(my_queue));
}

void test_cstl_queue()
{
	printf("!!test with int:!!\n");
	test_with_int();
	printf("!!test with struct pointers:!!\n");
	test_with_pointers();
	printf("!!test queue without auto grow:!!\n");
	test_without_auto_grow();
	printf("******************finished test_cstl_queue****************\n");
}