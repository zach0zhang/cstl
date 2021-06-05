#ifndef _CSTL_DEQUE_H_
#define _CSTL_DEQUE_H_

#ifdef __cplusplus
extern "C" {
#endif

struct cstl_deque{
    int capacity;
    int index;
    int head;
    int tail;
    struct cstl_object **elements;
    cstl_compare compare_fn;
    cstl_destory destruct_fn;
};

extern struct cstl_deque *new_cstl_deque(int capacity, cstl_compare fn_c, cstl_destory fn_d);
extern cstl_error push_back_cstl_deque(struct cstl_deque *my_deque, void *elem, size_t elem_size);
extern cstl_error push_front_cstl_deque(struct cstl_deque *my_deque, void *elem, size_t elem_size);
extern cstl_error front_cstl_deque(struct cstl_deque *my_deque, void **elem);
extern cstl_error back_cstl_deque(struct cstl_deque *my_deque, void **elem);
extern cstl_error pop_back_cstl_deque(struct cstl_deque *my_deque);
extern cstl_error pop_front_cstl_deque(struct cstl_deque *my_deque);
extern cstl_bool empty_cstl_deque(struct cstl_deque *my_deque);
extern int size_cstl_deque(struct cstl_deque *my_deque);
extern cstl_error delete_cstl_deque(struct cstl_deque *my_deque);

extern struct cstl_iterator *new_iterator_cstl_deque(struct cstl_deque *my_deque);
extern void delete_iterator_cstl_deque(struct cstl_iterator *my_itr);


#ifdef __cplusplus
}
#endif
#endif