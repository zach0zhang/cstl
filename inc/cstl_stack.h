#ifndef _CSTL_STACK_H_
#define _CSTL_STACK_H_

#ifdef __cplusplus
extern "C" {
#endif

struct cstl_stack {
    struct cstl_deque *deque;
};

extern struct cstl_stack *new_cstl_stack(int capacity, cstl_compare fn_c, cstl_destory fn_d);
extern cstl_error push_cstl_stack(struct cstl_stack *my_stack, void *elem, size_t elem_size);
extern cstl_error top_cstl_stack(struct cstl_stack *my_stack, void **elem);
extern cstl_error pop_cstl_stack(struct cstl_stack *my_stack);
extern cstl_bool empty_cstl_stack(struct cstl_stack *my_stack);
extern int size_cstl_stack(struct cstl_stack *my_stack);
extern cstl_error delete_cstl_stack(struct cstl_stack *my_stack);

#ifdef __cplusplus
}
#endif
#endif