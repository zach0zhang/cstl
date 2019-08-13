#ifndef _CSTL_MAP_H_
#define _CSTL_MAP_H_

#ifdef __cpluscplus
extern "C" {
#endif


struct cstl_map_node {
	struct rb_node node;
	struct cstl_object *key;
	struct cstl_object *value;
};

struct cstl_map {
	struct rb_root root;
	cstl_compare compare_function;
	cstl_destory destruct_key_function;
	cstl_destory destruct_value_function;
};

extern struct cstl_map *new_cstl_map(cstl_compare c_fn, cstl_destory d_k_fn, cstl_destory d_v_fn);
extern cstl_error insert_cstl_map(struct cstl_map *my_map, void *key, size_t key_size, void *value, size_t value_size);
extern cstl_error remove_cstl_map(struct cstl_map *my_map, void *key);
extern cstl_error delete_cstl_map(struct cstl_map *my_map);
extern cstl_bool exists_cstl_map(struct cstl_map *my_map, void *key);
extern cstl_bool find_cstl_map(struct cstl_map *my_map, void *key, void **value);

extern struct cstl_iterator *new_iterator_cstl_map(struct cstl_map *my_map);
extern void delete_iterator_cstl_map(struct cstl_iterator *my_itr);
#ifdef __cpluscplus
}
#endif

#endif
