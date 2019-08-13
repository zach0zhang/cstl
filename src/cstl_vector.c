#include "cstl_lib.h"

static struct cstl_vector *vector_check_and_grow(struct cstl_vector *my_vector)
{
	if (my_vector->index >= my_vector->capacity) {
		my_vector->capacity *= 2;
		my_vector->elements = (struct cstl_object**)realloc(my_vector->elements, my_vector->capacity * sizeof(struct cstl_object*));
	}
	return my_vector;
}

struct cstl_vector *new_cstl_vector(int init_size, cstl_compare fn_c, cstl_destory fn_d)
{
	struct cstl_vector *my_vector = (struct cstl_vector*)malloc(sizeof(struct cstl_vector));
	if (!my_vector)
		return (struct cstl_vector*)0;

	my_vector->capacity = init_size < 8 ? 8 : init_size;
	my_vector->elements = (struct cstl_object**)malloc(my_vector->capacity * sizeof(struct cstl_object*));
	if (!my_vector->elements) {
		free(my_vector);
		return (struct cstl_vector*)0;
	}

	my_vector->compare_fn = fn_c;
	my_vector->destruct_fn = fn_d;
	my_vector->index = 0;

	return my_vector;

}

static cstl_error insert_cstl_vector(struct cstl_vector *my_vector, int index, void *elem, size_t elem_size)
{
	cstl_error ret = CSTL_SUCCESS;
	struct cstl_object *my_object = new_cstl_object(elem, elem_size);
	if (!my_object)
		return CSTL_VECTOR_INSERT_FAILED;

	my_vector->elements[index] = my_object;
	my_vector->index++;
	return ret;
}

cstl_error push_back_cstl_vector(struct cstl_vector *my_vector, void *elem, size_t elem_size)
{
	cstl_error ret = CSTL_SUCCESS;
	if (!my_vector)
		return CSTL_VECTOR_NOT_INITIALIZED;

	vector_check_and_grow(my_vector);

	ret = insert_cstl_vector(my_vector, my_vector->index, elem, elem_size);

	return ret;
}

cstl_error element_at_cstl_vector(struct cstl_vector *my_vector, int index, void **elem)
{
	cstl_error ret = CSTL_SUCCESS;

	if (!my_vector)
		return CSTL_VECTOR_NOT_INITIALIZED;
	
	if (index < 0 || index >= my_vector->capacity)
		return CSTL_VECTOR_INDEX_OUT_OF_BOUND;

	ret = get_data_cstl_object(my_vector->elements[index], elem);
	
	return ret;
}

cstl_error insert_at_cstl_vector(struct cstl_vector *my_vector, int index, void *elem, size_t elem_size)
{
	cstl_error ret = CSTL_SUCCESS;
	if (!my_vector)
		return CSTL_VECTOR_NOT_INITIALIZED;

	if (index < 0 || index > my_vector->capacity)
		return CSTL_VECTOR_INDEX_OUT_OF_BOUND;

	vector_check_and_grow(my_vector);

	memmove(&(my_vector->elements[index + 1]), &(my_vector->elements[index]), (my_vector->index - index) * sizeof(struct cstl_object*));

	ret = insert_cstl_vector(my_vector, index, elem, elem_size);

	return ret;
}

int size_cstl_vector(struct cstl_vector *my_vector)
{
	return (my_vector == (struct cstl_vector*)0) ? 0 : my_vector->index;
}

int capacity_cstl_vector(struct cstl_vector *my_vector)
{
	return (my_vector == (struct cstl_vector*)0) ? 0 : my_vector->capacity;
}

cstl_bool empty_cstl_vector(struct cstl_vector *my_vector)
{
	return (my_vector == (struct cstl_vector*)0) ? 0 : (my_vector->index == 0) ? cstl_true : cstl_false;
}

cstl_error front_cstl_vector(struct cstl_vector *my_vector, void **elem)
{
	return element_at_cstl_vector(my_vector, 0, elem);
}

cstl_error back_cstl_vector(struct cstl_vector *my_vector, void **elem)
{
	return element_at_cstl_vector(my_vector, my_vector->index - 1, elem);
}

cstl_error remove_from_cstl_vector(struct cstl_vector *my_vector, int index)
{
	cstl_error ret = CSTL_SUCCESS;
	if (!my_vector)
		return ret;

	if (index < 0 || index >= my_vector->index)
		return CSTL_VECTOR_INDEX_OUT_OF_BOUND;

	if (my_vector->destruct_fn) {
		void *elem;
		if (CSTL_SUCCESS == element_at_cstl_vector(my_vector, index, &elem))
			my_vector->destruct_fn(elem);
	}

	delete_cstl_object(my_vector->elements[index]);
	memmove(&(my_vector->elements[index]), &(my_vector->elements[index + 1]), (my_vector->index - index - 1) * sizeof(struct cstl_object*));

	my_vector->index--;

	return ret;
}

cstl_error delete_cstl_vector(struct cstl_vector *my_vector)
{
	cstl_error ret = CSTL_SUCCESS;
	int i = 0;

	if (!my_vector)
		return ret;

	if (my_vector->destruct_fn) {
		for (i = 0; i < my_vector->index; i++) {
			void *elem;
			if (CSTL_SUCCESS == element_at_cstl_vector(my_vector, i, &elem))
				my_vector->destruct_fn(elem);
		}
	}

	for (i = 0; i < my_vector->index; i++)
		delete_cstl_object(my_vector->elements[i]);

	free(my_vector->elements);
	free(my_vector);
	
	return ret;
}

static struct cstl_object *get_next_cstl_vector(struct cstl_iterator *my_itr)
{
	struct cstl_vector *my_vector = (struct cstl_vector*)my_itr->container;
	
	if (my_itr->current >= size_cstl_vector(my_vector))
		return (struct cstl_object*)0;

	my_itr->current_element = my_vector->elements[my_itr->current++];
	return my_itr->current_element;
}

static void replace_value_cstl_vector(struct cstl_iterator *my_itr, void *elem, size_t elem_size)
{
	struct cstl_vector *my_vector = (struct cstl_vector*)my_itr->container;

	if (my_vector->destruct_fn) {
		void *old_element;
		get_data_cstl_object(my_itr->current_element, &old_element);
		my_vector->destruct_fn(old_element);
	}
	replace_data_cstl_object(my_itr->current_element, elem, elem_size);
}

static void *get_value_cstl_vector(void *my_object)
{
	void *elem;
	get_data_cstl_object(my_object, &elem);
	return elem;
}

struct cstl_iterator *new_iterator_cstl_vector(struct cstl_vector *my_vector)
{
	struct cstl_iterator *my_itr = (struct cstl_iterator*)malloc(sizeof(struct cstl_iterator));
	my_itr->get_next = get_next_cstl_vector;
	my_itr->replace_value = replace_value_cstl_vector;
	my_itr->get_value = get_value_cstl_vector;
	my_itr->container = my_vector;
	my_itr->current = 0;
	return my_itr;
}

void delete_iterator_cstl_vector(struct cstl_iterator *my_itr)
{
	free(my_itr);
}
