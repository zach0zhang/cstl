#include <assert.h>
#include "cstl_lib.h"

char *char_value[] = {  "A","B","C","D","E","F","G","H","I","J","K","L","M",
                        "N","O","P","Q","R","S","T","U","V","W","X","Y","Z"};
int int_value[] = { 1,2,3,4,5,6,7,8,9,10,
                      11,12,13,14,15,16,17,18,19,20,
                      21,22,23,24,25,26};

static int
compare_function ( void* left, void* right ) {
    return strcmp ( (const char *)left, (const char *) right );
}

static void insert_all(struct cstl_map *my_map)
{
	int size = sizeof(char_value) / sizeof(char_value[0]);
	int i = 0;
	for (i = 0; i < size; i++) {
		char *key = strdup(char_value[i]);
		int key_length = (int)strlen(key) + 1;
		int value = int_value[i];
		printf("Inserting [%s -> %d]\n", key, value);
		insert_cstl_map(my_map, key, key_length, &value, sizeof(int));
		free(key);
	}
}
static void insert_some(struct cstl_map *my_map)
{
	char *new_char_value[] = { "A", "G", "N", "Z" };
	int size = 4;
	int i = 0;
	for (i = 0; i < size; i++) {
		char *key = strdup(new_char_value[i]);
		int key_length = (int)strlen(key) + 1;
		int value = i;
		printf("Inserting [%s -> %d]\n", key, value);
		insert_cstl_map(my_map, key, key_length, &value, sizeof(int));
		free(key);
	}
}
static void check_all(struct cstl_map *my_map)
{
	int size = sizeof(char_value) / sizeof(char_value[0]);
	int i = 0;
	for (i = 0; i < size; i++) {
		void *value;
		assert(cstl_true == exists_cstl_map(my_map, char_value[i]));
		assert(cstl_true == find_cstl_map(my_map, char_value[i], &value));
		printf("Result [%s, %d]\n", char_value[i], *(int *)value);
		//assert(*(int *)value == int_value[i]);
		free(value);
	}
}

static void remove_some(struct cstl_map *my_map)
{
	assert(cstl_true == exists_cstl_map(my_map, "A"));
	assert(CSTL_SUCCESS == remove_cstl_map(my_map, "A"));
	assert(cstl_false == exists_cstl_map(my_map, "A"));

	assert(cstl_true == exists_cstl_map(my_map, "G"));
	assert(CSTL_SUCCESS == remove_cstl_map(my_map, "G"));
	assert(cstl_false == exists_cstl_map(my_map, "G"));

	assert(cstl_true == exists_cstl_map(my_map, "N"));
	assert(CSTL_SUCCESS == remove_cstl_map(my_map, "N"));
	assert(cstl_false == exists_cstl_map(my_map, "N"));

	assert(cstl_true == exists_cstl_map(my_map, "Z"));
	assert(CSTL_SUCCESS == remove_cstl_map(my_map, "Z"));
	assert(cstl_false == exists_cstl_map(my_map, "Z"));

}

static void print_using_iterator(struct cstl_map *my_map)
{
	struct cstl_iterator *my_itr = new_iterator_cstl_map(my_map);
	struct cstl_object *my_object;
	
	my_object = my_itr->get_next(my_itr);
	while (my_object) {
		void *value = my_itr->get_value(my_object);
		printf("%d\n", *(int *)value);
		free(value);
		my_object = my_itr->get_next(my_itr);
	}

	delete_iterator_cstl_map(my_itr);
}

static void replace_using_iterator(struct cstl_map *my_map)
{
	struct cstl_iterator *my_itr = new_iterator_cstl_map(my_map);
	struct cstl_object *my_object;
	
	my_object = my_itr->get_next(my_itr);
	while (my_object) {
		void *value = my_itr->get_value(my_object);
		int new_value = *(int *)value * 2;
		my_itr->replace_value(my_itr, &new_value, sizeof(new_value));
		free(value);
		my_object = my_itr->get_next(my_itr);
	}
	
	delete_iterator_cstl_map(my_itr);
}

static test_iterators()
{
	struct cstl_map *my_map = new_cstl_map(compare_function, NULL, NULL);
	insert_all(my_map);
	print_using_iterator(my_map);
	replace_using_iterator(my_map);
	print_using_iterator(my_map);
	delete_cstl_map(my_map);
}

void test_cstl_map()
{
	printf("******************start test_cstl_map**********************\n");
	struct cstl_map *my_map = new_cstl_map(compare_function, NULL, NULL);
	insert_all(my_map);
	check_all(my_map);
	remove_some(my_map);
	insert_some(my_map);
	check_all(my_map);
	assert(CSTL_SUCCESS == delete_cstl_map(my_map));
	test_iterators();
	printf("******************finished test_cstl_map*******************\n");
}


