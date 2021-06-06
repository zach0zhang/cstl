#include <assert.h>
#include "cstl_lib.h"

static void test_with_int()
{
    int size = 10;
    void *elem = (void *)0;

    struct cstl_stack *my_stack = new_cstl_stack(size, NULL, NULL);
    assert(cstl_true == empty_cstl_stack(my_stack));
    assert(0 == size_cstl_stack(my_stack));



    for (int i = 0; i < size; i++)
        push_cstl_stack(my_stack, &i, sizeof(int));

    assert(cstl_false == empty_cstl_stack(my_stack));
    assert(size == size_cstl_stack(my_stack));

    for (int i = size - 1; i >= 0; i--) {
        assert(CSTL_SUCCESS == top_cstl_stack(my_stack, &elem));
        assert(*(int *)elem == i);
        free(elem);
        assert(CSTL_SUCCESS == pop_cstl_stack(my_stack));
    }

    assert(cstl_true == empty_cstl_stack(my_stack));
    assert(0 == size_cstl_stack(my_stack));

    assert(CSTL_SUCCESS == delete_cstl_stack(my_stack));
}

typedef struct {
    int num;
    char name[10];
}test_statck_struct_t;

static char *char_value[] = {"zero", "one", "two", "three", "four", "five",
                    "six", "seven", "eight", "nine", "ten"};
static int int_value[] = {0,1,2,3,4,5,6,7,8,9,10};

static void test_with_pointers()
{
    int size = 1;
    void *elem = (void *)0;
    test_statck_struct_t test_struct[11];
    for (int i = 0; i < sizeof(test_struct)/sizeof(test_statck_struct_t); i++) {
        test_struct[i].num = int_value[i];
        snprintf(test_struct[i].name, sizeof(test_struct[i].name), "%s", char_value[i]); 
    }

    struct cstl_stack *my_stack = new_cstl_stack(size, NULL, NULL);
    assert(cstl_true == empty_cstl_stack(my_stack));

    size = sizeof(test_struct)/sizeof(test_statck_struct_t);
    for (int i = 0; i < size; i++)
        assert(CSTL_SUCCESS == push_cstl_stack(my_stack, (void *)&test_struct[i], sizeof(test_statck_struct_t)));

    assert(cstl_false == empty_cstl_stack(my_stack));
    assert(size == size_cstl_stack(my_stack));

    for (int i = size - 1; i >=0; i--) {
        assert(CSTL_SUCCESS == top_cstl_stack(my_stack, &elem));
        assert(((test_statck_struct_t *)elem)->num == int_value[i]);
        assert(strcmp(((test_statck_struct_t *)elem)->name, char_value[i]) == 0);
        assert(CSTL_SUCCESS == pop_cstl_stack(my_stack));
        free(elem);
    }

    assert(cstl_true == empty_cstl_stack(my_stack));
    assert(CSTL_SUCCESS == delete_cstl_stack(my_stack));
}

void test_cstl_stack()
{
    printf("!!test with int:!!\n");
	test_with_int();
	printf("!!test with struct pointers:!!\n");
	test_with_pointers();
	printf("******************finished test_cstl_stack****************\n");
}