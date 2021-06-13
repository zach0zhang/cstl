#ifndef _CSTL_SET_H_
#define _CSTL_SET_H_

#ifdef __cplusplus
extern "C" {
#endif

struct cstl_set_node {
    struct rb_node node;
    struct cstl_object *key;
};

struct cstl_set {
    struct rb_root root;
    cstl_compare compare_fn;
    cstl_destory destruct_fn;
};

extern struct cstl_set *new_cstl_set(cstl_compare fn_c, cstl_destory fn_d);
extern cstl_error insert_cstl_set(struct cstl_set *my_set, void *key, size_t key_size);
extern cstl_error remove_cstl_set(struct cstl_set *my_set, void *key);
extern cstl_error delete_cstl_set(struct cstl_set *my_set);
extern cstl_bool exists_cstl_set(struct cstl_set *my_set, void *key);
extern cstl_bool find_cstl_set(struct cstl_set *my_set, void *key, void **outKey);

extern struct cstl_iterator *new_iterator_cstl_set(struct cstl_set *my_set);
extern void delete_iterator_cstl_set(struct cstl_iterator *my_itr);

#ifdef __cplusplus
}
#endif
#endif