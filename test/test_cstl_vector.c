#include <assert.h>
#include "cstl_lib.h"

static int compare_function(void *left, void *right)
{
	int *l = (int*)left;
	int *r = (int*)right;
	return *l == *r;
}

static void free_function(void *ptr)
{
	if (ptr)
		free(ptr);
}

static void test_with_int()
{
	int init_size = 20;
	int i = 0;
	int ret = CSTL_SUCCESS;
	void *elem = (void*)0;

	struct cstl_vector *my_vector = new_cstl_vector(init_size, compare_function, NULL);
	assert(cstl_true == empty_cstl_vector(my_vector));

	for (i = 0; i < init_size; i++)
		push_back_cstl_vector(my_vector, &i, sizeof(int));
	assert(cstl_false == empty_cstl_vector(my_vector));
	assert(init_size == size_cstl_vector(my_vector));	

	for (i = 0; i < init_size; i++) {
		ret = element_at_cstl_vector(my_vector, i, &elem);
		assert(ret == CSTL_SUCCESS);
		assert(*(int*)elem == i);
		//printf("after push_back_cstl_vector, my_vector->elements[%d]->data is %d\n", i, *(int*)elem);
		free(elem);	
	} 
	
	ret = front_cstl_vector(my_vector, &elem);
	assert(ret == CSTL_SUCCESS);
	assert(*(int*)elem == 0);
	//printf("after push_back_cstl_vector, the front number is %d\n", *(int*)elem);
	free(elem);

	ret = back_cstl_vector(my_vector, &elem);
    assert(ret == CSTL_SUCCESS);
	assert(*(int*)elem == init_size-1);
    //printf("after push_back_cstl_vector, the back number is %d\n", *(int*)elem);
	free(elem);

	remove_from_cstl_vector(my_vector, 0);
	assert(init_size - 1 == size_cstl_vector(my_vector));
	ret = element_at_cstl_vector(my_vector, 0, &elem);
	assert(*(int*)elem == 1);
	//printf("after remove_from_cstl_vector(0), my_vector->elements[0]->data is %d\n", *(int*)elem);
	free(elem);

	init_size = size_cstl_vector(my_vector);
	assert(init_size == 19);
	//printf("after remove_from_cstl_vector(0), init_size = %d\n", init_size);
	remove_from_cstl_vector(my_vector, init_size/2);
	assert(init_size -1 == size_cstl_vector(my_vector));
	ret = element_at_cstl_vector(my_vector, init_size/2, &elem);
	//printf("after remove_from_cstl_vector(19/2), my_vector->element[9]->data is %d\n", *(int*)elem);
	assert(*(int*)elem == init_size/2 + 2);
	free(elem);

	init_size = size_cstl_vector(my_vector);
	//printf("after remove_from_cstl_vector(19/2), init_size = %d\n", init_size);
	remove_from_cstl_vector(my_vector, init_size - 1);
	assert(init_size - 1 == size_cstl_vector(my_vector));
	ret = element_at_cstl_vector(my_vector, init_size - 2, &elem);
	//printf("after remove_from_cstl_vector(18-1), my_vector->element[16]->data is %d\n", *(int*)elem);
	assert(*(int*)elem == 18);
	free(elem);

	i = 888;
	insert_at_cstl_vector(my_vector, 10, &i, sizeof(int));
	ret = element_at_cstl_vector(my_vector, 10, &elem);
	//printf("after insert_at_cstl_vector(10, 888), my_vector->element[10]->data is %d\n", *(int*)elem);
	//printf("my_vector->element[10]->data is %d\n", *(int*)my_vector->elements[10]->data);
	assert(*(int*)elem == i);
	free(elem);	

	for (i = 0; i < init_size; i++) {
		ret = element_at_cstl_vector(my_vector, i, &elem);
		assert(ret == CSTL_SUCCESS);
		//printf("now, my_vector->elements[%d]->data is %d\n", i, *(int*)elem);
		free(elem);
        }
	delete_cstl_vector(my_vector);
}

static void test_with_pointers()
{
	int size = 10;
	int i = 0;
	int ret = CSTL_SUCCESS;
	void *elem = (void *)0;

	struct cstl_vector *my_vector = new_cstl_vector(8, compare_function, free_function);
	assert(cstl_true == empty_cstl_vector(my_vector));

	for (i = 0; i < size; i++) {
		int *v = (int *)malloc(sizeof(int));
		memcpy(v, &i, sizeof(int));
		push_back_cstl_vector(my_vector, v, sizeof(int *));
		free(v);
	}
	assert(cstl_false == empty_cstl_vector(my_vector));
	assert(size == size_cstl_vector(my_vector));

	for (i = 0; i <size; i++) {
		ret = element_at_cstl_vector(my_vector, i, &elem);
		assert(CSTL_SUCCESS == ret);
		assert(*(int*)elem == i);
		//printf("after push_back_cstl_vector, my_vector->elements[%d]->data = %d\n", i, *(int *)elem);
		free(elem);
	}

	ret = front_cstl_vector(my_vector, &elem);
	assert(CSTL_SUCCESS == ret);
	assert(*(int*)elem == 0);
	//printf("after push_back_cstl_vector, my_vector->elements[0]->data = %d\n", *(int *)elem);	
	free(elem);
	
	ret = back_cstl_vector(my_vector, &elem);
	assert(CSTL_SUCCESS == ret);
	assert(*(int*)elem == size-1);
	//printf("after push_bacl_cstl_vector, my_vector->elements[9]->data = %d\n", *(int *)elem);
	free(elem);	

	assert(capacity_cstl_vector(my_vector) >= size);
	//printf("after push_back_cstl_vector, my_vector->capacity = %d\n", capacity_cstl_vector(my_vector));
	
	ret = remove_from_cstl_vector(my_vector, 0);
	assert(ret == CSTL_SUCCESS);
	
	ret = remove_from_cstl_vector(my_vector, size - 2);
	assert(ret == CSTL_SUCCESS);

	size = size_cstl_vector(my_vector);
	i = 888;
	int *v = (int *)malloc(sizeof(int));
	memcpy(v, &i, sizeof(int));
	ret = insert_at_cstl_vector(my_vector, size / 2, v, sizeof(int));
	free(v);

	size = size_cstl_vector(my_vector);
	assert(size == 9);
	for (i = 0; i < size; i++) {
		ret = element_at_cstl_vector(my_vector, i, &elem);
		assert(CSTL_SUCCESS == ret);
		//printf("now, my_vector->elements[%d]->raw_data = %d\n", i, *(int *)elem);
		free(elem);

	}
	
	delete_cstl_vector(my_vector);
}

static void test_with_strings()
{
	int size = 10;
	char *input_vector[11];
	int i = 0;
	int ret = CSTL_SUCCESS;
	void *elem = (void *)0;

	struct cstl_vector *my_vector = new_cstl_vector(8, compare_function, free_function);
	assert(cstl_true == empty_cstl_vector(my_vector));

	for (i = 0; i < size; i++) {
		input_vector[i] = (char *)malloc(sizeof(char) * 9);
		sprintf(input_vector[i], "STRING_%d", i);
		//printf("%s\n", input_vector[i]);
	}

	for (i = 0; i < size; i++) {
		/*
		char *v = strdup(input_vector[i]);
		push_back_cstl_vector(my_vector, v, strlen(v) + 1);
		free(v);
		*/
		push_back_cstl_vector(my_vector, input_vector[i], sizeof(input_vector[i]));
	}
	assert(cstl_false == empty_cstl_vector(my_vector));
	assert(size == size_cstl_vector(my_vector));
	
	for (i = 0; i < size; i++) {
		ret = element_at_cstl_vector(my_vector, i, &elem);
		assert(ret == CSTL_SUCCESS);
		assert(strcmp((char *)elem, input_vector[i]) == 0);
		//printf("after push_back_cstl_vector, my_vector->elements[%d]->raw_data = %s\n", i, (char *)elem);
		free(elem);
	}

	ret = front_cstl_vector(my_vector, &elem);
	assert(CSTL_SUCCESS == ret);
	//printf("after push_back_cstl_vector, my_vector->elements[0]->raw_data = %s\n", (char *)elem);	
	free(elem);
	
	ret = back_cstl_vector(my_vector, &elem);
	assert(CSTL_SUCCESS == ret);
	//printf("after push_bacl_cstl_vector, my_vector->elements[9]->raw_data = %s\n", (char *)elem);
	free(elem);	

	//printf("after push_back_cstl_vector, my_vector->capacity = %d\n", capacity_cstl_vector(my_vector));
	
	ret = remove_from_cstl_vector(my_vector, 0);
	assert(ret == CSTL_SUCCESS);
	
	ret = remove_from_cstl_vector(my_vector, size - 2);
	assert(ret == CSTL_SUCCESS);

	size = size_cstl_vector(my_vector);
	char *s = "hhhhhhhhhh";
	ret = insert_at_cstl_vector(my_vector, size / 2, s, strlen(s) + 1);
	
	size = size_cstl_vector(my_vector);
	assert(size == 9);
	for (i = 0; i < size; i++) {
		ret = element_at_cstl_vector(my_vector, i, &elem);
		assert(CSTL_SUCCESS == ret);
		//printf("now, my_vector->elements[%d]->raw_data = %s\n", i, (char *)elem);
		free(elem);
	}

	for (i = 0; i < size + 1; i++)
		free(input_vector[i]);
	delete_cstl_vector(my_vector);
}

static void print_using_iterators(struct cstl_vector *my_vector)
{
	struct cstl_iterator *my_itr;
	struct cstl_object *element;

	my_itr = new_iterator_cstl_vector(my_vector);
	element = my_itr->get_next(my_itr);
	while (element) {
		void *value = my_itr->get_value(element);
		//printf("%d\n", *(int *)value);
		free(value);
		element = my_itr->get_next(my_itr);
	}

	delete_iterator_cstl_vector(my_itr);
}

static void replace_values_using_iterators(struct cstl_vector *my_vector)
{
	struct cstl_iterator *my_itr = new_iterator_cstl_vector(my_vector);
	
	struct cstl_object *element = my_itr->get_next(my_itr);
	while	(element) {
		void *old_value = my_itr->get_value(element);
		int new_value = *(int *)old_value * 2;
		my_itr->replace_value(my_itr, &new_value, sizeof(int));
		free(old_value);
		element = my_itr->get_next(my_itr);
	}
	delete_iterator_cstl_vector(my_itr);
}

static void test_with_iterator()
{
	int size = 10;
	int i = 0;
	struct cstl_vector *my_vector = new_cstl_vector(8, compare_function, NULL);
	for (i = 0; i < size; i++)
		push_back_cstl_vector(my_vector, &i, sizeof(int));
	
	print_using_iterators(my_vector);
	replace_values_using_iterators(my_vector);
	print_using_iterators(my_vector);
	
	delete_cstl_vector(my_vector);
}
void test_cstl_vector()
{
	printf("!!test with int:!!\n");
	test_with_int();
	printf("!!test with pointers:!!\n");
	test_with_pointers();
	printf("!!test with strings:!!\n");
	test_with_strings();
	printf("!!test with iterator:!!\n");
	test_with_iterator();
	printf("******************finished test_cstl_vector****************\n");
}


