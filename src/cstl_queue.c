#include "cstl_lib.h"

#define CHECK_QUEUE(queue)     do {             \
    if (!queue)                                 \
        return CSTL_QUEUE_NOT_INITIALIZED;      \
    } while (0)

static inline void add_tx(struct cstl_queue *my_queue) {
    my_queue->tx += 1;
    if (my_queue->tx == my_queue->capacity)
        my_queue->tx = 0;
}

static inline void add_rx(struct cstl_queue *my_queue) {
    my_queue->rx += 1;
    if (my_queue->rx == my_queue->capacity)
        my_queue->rx = 0;
}

static struct cstl_queue *grow_queue(struct cstl_queue *my_queue) {
    my_queue->capacity = my_queue->capacity * 2;
    my_queue->elements = (struct cstl_object**)realloc(my_queue->elements, my_queue->capacity * sizeof(struct cstl_object*));
    return my_queue;
}

struct cstl_queue *new_cstl_queue(int capacity, cstl_compare fn_c, cstl_destory fn_d, cstl_bool auto_grow)
{
    struct cstl_queue *my_queue = (struct cstl_queue *)malloc(sizeof(struct cstl_queue));
    if (!my_queue)
        return (struct cstl_queue*)0;

    my_queue->auto_grow = auto_grow;
    my_queue->capacity = auto_grow == cstl_true ? (capacity < 8 ? 8 : capacity) : capacity;
    my_queue->elements = (struct cstl_object **)malloc(my_queue->capacity * sizeof(struct cstl_object*));
    if (!my_queue->elements) {
        free(my_queue);
        return (struct cstl_queue*)0;
    }

    my_queue->compare_fn = fn_c;
    my_queue->destruct_fn = fn_d;
    my_queue->tx = 0;
    my_queue->rx = 0;
    my_queue->num = 0;

    return my_queue;
}

cstl_bool full_cstl_queue(struct cstl_queue *my_queue)
{
    if (!my_queue)
        return cstl_false;

    if (my_queue->auto_grow == cstl_true)
        return cstl_false;

    return my_queue->num == my_queue->capacity ? cstl_true : cstl_false;
}


cstl_bool empty_cstl_queue(struct cstl_queue *my_queue)
{
    if(!my_queue)
        return cstl_true;

    return my_queue->num == 0 ? cstl_true : cstl_false;
}

int size_cstl_queue(struct cstl_queue *my_queue)
{
    CHECK_QUEUE(my_queue);

    return my_queue->num;
}

cstl_error push_cstl_queue(struct cstl_queue *my_queue, void *elem, size_t elem_size)
{
    int old_cap = 0, from = 0, to = 0, count = 0;

    CHECK_QUEUE(my_queue);

    if (full_cstl_queue(my_queue))
        return CSTL_QUEUE_IS_FULL;

    if (my_queue->num == my_queue->capacity) {
        old_cap = my_queue->capacity;
        my_queue = grow_queue(my_queue);
        if (my_queue->tx < my_queue->rx) {
            from = my_queue->rx;
            count = old_cap - from;
            to = my_queue->capacity - count;
            memmove(&(my_queue->elements[to]), &(my_queue->elements[from]), count * sizeof(struct cstl_object*));
            my_queue->rx = to;
        }
    }

    struct cstl_object *my_object = new_cstl_object(elem, elem_size);
    if (!my_object)
        return CSTL_QUEUE_INSERT_FAILED;

    my_queue->elements[my_queue->tx] = my_object;
    add_tx(my_queue); 
    my_queue->num++;
    
    return CSTL_SUCCESS;
}

cstl_error top_cstl_queue(struct cstl_queue *my_queue, void **elem)
{
    cstl_error ret = CSTL_SUCCESS;

    CHECK_QUEUE(my_queue);

    if (empty_cstl_queue(my_queue))
        return CSTL_QUEUE_IS_EMPTY;

    ret = get_data_cstl_object(my_queue->elements[my_queue->rx], elem);

    return ret;
}

cstl_error pop_cstl_queue(struct cstl_queue *my_queue)
{
    CHECK_QUEUE(my_queue);

    if (empty_cstl_queue(my_queue))
        return CSTL_QUEUE_IS_EMPTY;

    if (my_queue->destruct_fn) {
        void *elem;
        if (top_cstl_queue(my_queue, &elem) == CSTL_SUCCESS)
            my_queue->destruct_fn(elem);
    }

    delete_cstl_object(my_queue->elements[my_queue->rx]);
    add_rx(my_queue);
    my_queue->num--;

    return CSTL_SUCCESS;

}

 cstl_error delete_cstl_queue(struct cstl_queue *my_queue)
 {
    if (my_queue)
        return CSTL_SUCCESS;

    if (my_queue->destruct_fn) {
        while(!empty_cstl_queue(my_queue))
            pop_cstl_queue(my_queue);
    }

    free(my_queue->elements);
    free(my_queue);
    
    return CSTL_SUCCESS;
 }