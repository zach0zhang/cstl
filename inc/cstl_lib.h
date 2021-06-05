#ifndef _CSTL_LIB_H_
#define _CSTL_LIB_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum {
	CSTL_SUCCESS = 0,
	CSTL_ERROR,
	CSTL_ERROR_MEMORY,
	CSTL_ELEMENT_RETURN_ERROR,

	CSTL_VECTOR_NOT_INITIALIZED = 0x80000101,
	CSTL_VECTOR_INDEX_OUT_OF_BOUND,
	CSTL_VECTOR_INSERT_FAILED,
	
	CSTL_MAP_NOT_INITIALIZED = 0x80000201,
	CSTL_MAP_KEY_ALREADY_EXIST,
	CSTL_MAP_KEY_NOT_FOUND,
	
	CSTL_DEQUE_NOT_INITIALIZED = 0x80000301,
	CSTL_DEQUE_INSERT_FAILED
};

typedef void (*cstl_destory)(void*);
typedef int  (*cstl_compare)(void*, void*);

typedef int cstl_error;
typedef int cstl_bool;

#define cstl_true    1
#define cstl_false   0

struct cstl_object {
	void *data;
	size_t size;
};


struct cstl_iterator {
	struct cstl_object *(*get_next)(struct cstl_iterator*);
	void (*replace_value)(struct cstl_iterator*, void*, size_t);
	void *(*get_value)(void*);
	void *container;
	int  current;
	void *current_element;
};

#include "cstl_vector.h"
#include "cstl_rb.h"
#include "cstl_map.h"
#include "cstl_deque.h"
extern struct cstl_object* new_cstl_object(void *elem, size_t elem_size);
extern cstl_error get_data_cstl_object(struct cstl_object *my_object, void **elem);
extern void delete_cstl_object(struct cstl_object *my_object);
extern void replace_data_cstl_object(struct cstl_object *current_object, void *elem, size_t elem_size);


#ifdef __cplusplus
}
#endif
#endif
