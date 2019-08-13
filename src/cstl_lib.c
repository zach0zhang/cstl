#include "cstl_lib.h"

struct cstl_object *new_cstl_object(void *elem, size_t elem_size)
{
	struct cstl_object *my_object = (struct cstl_object*)malloc(sizeof(struct cstl_object));
	if (!my_object)
		return (struct cstl_object*)0;

	my_object->size = elem_size;
	my_object->data = (void*)malloc(elem_size);
	if (!my_object->data) {
		free(my_object);
		return (struct cstl_object*)0;
	}

	memcpy(my_object->data, elem, elem_size);
	return my_object;
}

cstl_error get_data_cstl_object(struct cstl_object *my_object, void **elem)
{
	cstl_error ret = CSTL_SUCCESS;

	*elem = (void*)malloc(sizeof(my_object->size));
	if (! *elem)
		return CSTL_ELEMENT_RETURN_ERROR;
	
	memcpy(*elem, my_object->data, my_object->size);
	return ret;
}

void delete_cstl_object(struct cstl_object *my_object)
{
	if (my_object) {
		free(my_object->data);
		free(my_object);
	}
}

void replace_data_cstl_object(struct cstl_object *current_object, void *elem, size_t elem_size)
{
	free(current_object->data);
	current_object->data = (void*)malloc(elem_size);
	memcpy(current_object->data, elem, elem_size);

}
