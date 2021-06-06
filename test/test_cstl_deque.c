#include <assert.h>
#include "cstl_lib.h"

static void test_with_int()
{
    int capacity = 20;
    void *elem = (void *)0;

    struct cstl_deque *my_deque = new_cstl_deque(capacity, NULL, NULL);
    assert(cstl_true == empty_cstl_deque(my_deque));
    assert(0 == size_cstl_deque(my_deque));

    for (int i = 0; i < capacity; i++)
        push_front_cstl_deque(my_deque, &i, sizeof(int));

    assert(cstl_false == empty_cstl_deque(my_deque));
    assert(capacity == size_cstl_deque(my_deque));

    for (int i = 0; i < capacity; i++) {
        assert(CSTL_SUCCESS == back_cstl_deque(my_deque, &elem));
        assert(*(int *)elem == i);
        free(elem);
        assert(CSTL_SUCCESS == pop_back_cstl_deque(my_deque));
    }

    assert(cstl_true == empty_cstl_deque(my_deque));
    assert(0 == size_cstl_deque(my_deque));

        for (int i = 0; i < capacity; i++)
        push_back_cstl_deque(my_deque, &i, sizeof(int));

    assert(cstl_false == empty_cstl_deque(my_deque));
    assert(capacity == size_cstl_deque(my_deque));

    for (int i = 0; i < capacity; i++) {
        assert(CSTL_SUCCESS == front_cstl_deque(my_deque, &elem));
        assert(*(int *)elem == i);
        assert(CSTL_SUCCESS == pop_front_cstl_deque(my_deque));
        free(elem);
    }

    assert(cstl_true == empty_cstl_deque(my_deque));
    assert(0 == size_cstl_deque(my_deque));

    assert(CSTL_SUCCESS == delete_cstl_deque(my_deque));
}

typedef struct {
    int num;
    char name[10];
}test_deque_struct_t;

static char *char_value[] = {"zero", "one", "two", "three", "four", "five",
                    "six", "seven", "eight", "nine", "ten"};
static int int_value[] = {0,1,2,3,4,5,6,7,8,9,10};

static void test_with_pointers()
{
    int size = 1;
    void *elem = (void *)0;
    test_deque_struct_t test_struct[11];
    for (int i = 0; i < sizeof(test_struct)/sizeof(test_deque_struct_t); i++) {
        test_struct[i].num = int_value[i];
        snprintf(test_struct[i].name, sizeof(test_struct[i].name), "%s", char_value[i]); 
    }

    struct cstl_deque *my_deque = new_cstl_deque(size, NULL, NULL); // auto grow
    assert(cstl_true == empty_cstl_deque(my_deque));

    size = sizeof(test_struct)/sizeof(test_deque_struct_t);
    for (int i = 0; i < size; i++)
        assert(CSTL_SUCCESS == push_front_cstl_deque(my_deque, (void *)&test_struct[i], sizeof(test_deque_struct_t)));

    assert(cstl_false == empty_cstl_deque(my_deque));
    assert(size == size_cstl_deque(my_deque));

    for (int i = 0; i < size; i++) {
        assert(CSTL_SUCCESS == back_cstl_deque(my_deque, &elem));
        assert(((test_deque_struct_t *)elem)->num == int_value[i]);
        assert(strcmp(((test_deque_struct_t *)elem)->name, char_value[i]) == 0);
        assert(CSTL_SUCCESS == pop_back_cstl_deque(my_deque));
        free(elem);
    }

    assert(cstl_true == empty_cstl_deque(my_deque));
    assert(CSTL_SUCCESS == delete_cstl_deque(my_deque));
}


static void test_with_iterator()
{
    int size = 10;
    char *input_string[10];
    struct cstl_iterator *my_itr;
    struct cstl_object *element;

    for (int i = 0; i < size; i++) {
		input_string[i] = (char *)malloc(sizeof(char) * 10);
		sprintf(input_string[i], "STRING_%d", i);
		//printf("%s\n", input_string[i]);
	}
    struct cstl_deque *my_deque = new_cstl_deque(8, NULL, NULL);
    for (int i = 0; i < size; i++)
        push_back_cstl_deque(my_deque, input_string[i], strlen(input_string[i]) + 1);

    my_itr = new_iterator_cstl_deque(my_deque);
    element = my_itr->get_next(my_itr);
    while (element) {
        void *value = my_itr->get_value(element);
        //printf("%s\n", (char *)value);
        char *new_value = "re";
        my_itr->replace_value(my_itr, new_value, strlen(new_value) + 1);
        free(value);
        element = my_itr->get_next(my_itr);
    }
    delete_iterator_cstl_deque(my_itr);
    
    my_itr = new_iterator_cstl_deque(my_deque);
    element = my_itr->get_next(my_itr);
    while (element) {
        void *value = my_itr->get_value(element);
        assert(strcmp((char *)value, "re") == 0);
        //printf("%s\n", (char *)value);
        free(value);
        element = my_itr->get_next(my_itr);
    }
    delete_iterator_cstl_deque(my_itr);

    for(int i = 0; i < size; i++)
        free(input_string[i]);
    assert(CSTL_SUCCESS == delete_cstl_deque(my_deque));
}

void test_cstl_deque()
{
	printf("!!test with int:!!\n");
	test_with_int();
	printf("!!test with struct pointers:!!\n");
	test_with_pointers();
	printf("!!test with iterator:!!\n");
	test_with_iterator();
	printf("******************finished test_cstl_deque****************\n");
}