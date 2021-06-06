#include "cstl_lib.h"

#define CHECK_STACK(stack)     do {             \
    if (!stack)                                 \
        return CSTL_STACK_NOT_INITIALIZED;      \
    } while (0)

struct cstl_stack *new_cstl_stack(int capacity, cstl_compare fn_c, cstl_destory fn_d)
{
    struct cstl_stack *my_stack = (struct cstl_stack *)malloc(sizeof(struct cstl_stack));
    if (!my_stack)
        return (struct cstl_stack*)0;
    
    struct cstl_deque *my_deque = new_cstl_deque(capacity, fn_c, fn_d);
    if (!my_deque) {
        free(my_stack);
        return (struct cstl_stack*)0;
    }

    my_stack->deque = my_deque;
    return my_stack;
}
cstl_error push_cstl_stack(struct cstl_stack *my_stack, void *elem, size_t elem_size)
{
    cstl_error ret = CSTL_SUCCESS;

    CHECK_STACK(my_stack);

    ret = push_front_cstl_deque(my_stack->deque, elem, elem_size);
    if (ret < 0)
        return CSTL_STACK_INSERT_FAILED;

    return ret;
}
cstl_error top_cstl_stack(struct cstl_stack *my_stack, void **elem)
{
    CHECK_STACK(my_stack);

    if (empty_cstl_stack(my_stack))
        return CSTL_STACK_IS_EMPTY;

    return front_cstl_deque(my_stack->deque, elem);   
}

cstl_error pop_cstl_stack(struct cstl_stack *my_stack)
{
    CHECK_STACK(my_stack);

    if (empty_cstl_stack(my_stack))
        return CSTL_STACK_IS_EMPTY;

    return pop_front_cstl_deque(my_stack->deque);
}

cstl_bool empty_cstl_stack(struct cstl_stack *my_stack)
{
    if (!my_stack)
        return cstl_true;

    return empty_cstl_deque(my_stack->deque);
}

int size_cstl_stack(struct cstl_stack *my_stack)
{
    CHECK_STACK(my_stack);

    return size_cstl_deque(my_stack->deque);
}

cstl_error delete_cstl_stack(struct cstl_stack *my_stack)
{
    if (!my_stack)
        return CSTL_SUCCESS;

    delete_cstl_deque(my_stack->deque);
    free(my_stack);

    return CSTL_SUCCESS;
}