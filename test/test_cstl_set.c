#include <assert.h>
#include "cstl_lib.h"

static int
compare_function (void *left, void *right){
    return strcmp ((const char *)left, (const char *)right);
}

typedef struct {
    int num;
    char name[10];
}test_set_struct_t;

static char *char_value[] = {"zero", "one", "two", "three", "four", "five",
                    "six", "seven", "eight", "nine", "ten"};
static int int_value[] = {0,1,2,3,4,5,6,7,8,9,10};

static void test_with_pointers_and_iterator()
{
    void *elem = (void *)0;
    test_set_struct_t test_struct[11];
    int size = sizeof(test_struct)/sizeof(test_set_struct_t);
    for (int i = 0; i < sizeof(test_struct)/sizeof(test_set_struct_t); i++) {
        test_struct[i].num = int_value[i];
        snprintf(test_struct[i].name, sizeof(test_struct[i].name), "%s", char_value[i]); 
    }

    struct cstl_set *my_set = new_cstl_set(compare_function, NULL);
    assert(my_set);

    for (int i = 0; i < size; i++)
        assert(CSTL_SUCCESS == insert_cstl_set(my_set, (void *)&test_struct[i], sizeof(test_set_struct_t)));

    for (int i = 0; i < size; i++) {
        assert(cstl_true == exists_cstl_set(my_set, (void *)&test_struct[i]));
        assert(cstl_true == find_cstl_set(my_set, (void *)&test_struct[i], &elem));
        assert(((test_set_struct_t *)elem)->num == int_value[i]);
        assert(strcmp(((test_set_struct_t *)elem)->name, char_value[i]) == 0);
        free(elem);
    }

    test_set_struct_t notExistStruct = {11, "eleven"};
    assert(cstl_false == exists_cstl_set(my_set, (void *)&notExistStruct));

    assert(cstl_true == exists_cstl_set(my_set, (void *)&test_struct[5]));
    assert(CSTL_SUCCESS == remove_cstl_set(my_set, (void *)&test_struct[5]));
    assert(cstl_false == exists_cstl_set(my_set, (void *)&test_struct[5]));

    memcpy(&test_struct[5], &notExistStruct, sizeof(test_set_struct_t));

    assert(CSTL_SUCCESS == insert_cstl_set(my_set, (void *)&notExistStruct, sizeof(test_set_struct_t)));

    struct cstl_iterator *my_itr = new_iterator_cstl_set(my_set);
    struct cstl_object *my_object;

    my_object = my_itr->get_next(my_itr);
    while (my_object) {
        void *value = my_itr->get_value(my_object);
        //printf("%d; %s\n", ((test_set_struct_t *)value)->num, ((test_set_struct_t *)value)->name);
        free(value);
        my_object = my_itr->get_next(my_itr);
    }

    delete_iterator_cstl_set(my_itr);


    assert(CSTL_SUCCESS == delete_cstl_set(my_set));
}



void test_cstl_set()
{
    printf("******************start test_cstl_set**********************\n");
    printf("!!test with struct pointers and iterator:!!\n");
    test_with_pointers_and_iterator();
    printf("******************finished test_cstl_deque****************\n");
}