#ifndef _CSTL_VECTOR_H_
#define _CSTL_VECTOR_H_

#ifdef __cplusplus
extern "C" {
#endif


struct cstl_vector {
	int capacity;
	int index;
	struct cstl_object **elements;
	cstl_compare compare_fn;
	cstl_destory destruct_fn;
};

extern struct cstl_vector *new_cstl_vector(int init_size, cstl_compare fn_c, cstl_destory fn_d);
extern cstl_error push_back_cstl_vector(struct cstl_vector *my_vector, void *elem, size_t elem_size);
extern cstl_error element_at_cstl_vector(struct cstl_vector *my_vector, int index, void **elem);
extern cstl_error insert_at_cstl_vector(struct cstl_vector *my_vector, int index, void *elem, size_t elem_size);
extern int size_cstl_vector(struct cstl_vector *my_vector);
extern int capacity_cstl_vector(struct cstl_vector *my_vector);
extern cstl_bool empty_cstl_vector(struct cstl_vector *my_vector);
extern cstl_error front_cstl_vector(struct cstl_vector *my_vector, void **elem);
extern cstl_error back_cstl_vector(struct cstl_vector *my_vector, void **elem);
extern cstl_error remove_from_cstl_vector(struct cstl_vector *my_vector, int index);
extern cstl_error delete_cstl_vector(struct cstl_vector *my_vector);

extern struct cstl_iterator *new_iterator_cstl_vector(struct cstl_vector *my_vector);
extern void delete_iterator_cstl_vector(struct cstl_iterator *my_itr);


#ifdef __cpluplus
}
#endif

#endif
