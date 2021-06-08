#ifndef _CSTL_QUEUE_H_
#define _CSTL_QUEUE_H_

#ifdef __cplusplus
extern "C" {
#endif

struct cstl_queue {
    cstl_bool auto_grow;
    int capacity;
    int num;
    int rx;
    int tx;
    struct cstl_object **elements;
    cstl_compare compare_fn;
    cstl_destory destruct_fn;
};

extern struct cstl_queue *new_cstl_queue(int capacity, cstl_compare fn_c, cstl_destory fn_d, cstl_bool grow_auto);
extern cstl_error push_cstl_queue(struct cstl_queue *my_queue, void *elem, size_t elem_size);
extern cstl_error top_cstl_queue(struct cstl_queue *my_queue, void **elem);
extern cstl_error pop_cstl_queue(struct cstl_queue *my_queue);
extern cstl_bool full_cstl_queue(struct cstl_queue *my_queue);
extern cstl_bool empty_cstl_queue(struct cstl_queue *my_queue);
extern int size_cstl_queue(struct cstl_queue *my_queue);
extern cstl_error delete_cstl_queue(struct cstl_queue *my_queue);



#ifdef __cplusplus
}
#endif
#endif