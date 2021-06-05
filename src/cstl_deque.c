#include "cstl_lib.h"

static struct cstl_deque *grow_deque(struct cstl_deque *my_deque) {
    my_deque->capacity = my_deque->capacity * 2;
    my_deque->elements = (struct cstl_object**) realloc(my_deque->elements, my_deque->capacity * sizeof(struct cstl_object*));
    return my_deque;
}

static cstl_error insert_cstl_deque(struct cstl_deque *my_deque, int index, void *elem, size_t elem_size) {
    struct cstl_object *my_object = new_cstl_object(elem, elem_size);
    if (!my_object)
        return CSTL_DEQUE_INSERT_FAILED;

    my_deque->elements[index] = my_object;
    my_deque->index++;
    return CSTL_SUCCESS;
}

struct cstl_deque *new_cstl_deque(int capacity, cstl_compare fn_c, cstl_destory fn_d)
{
    struct cstl_deque *my_deque = (struct cstl_deque *)malloc(sizeof(struct cstl_deque));
    if (!my_deque)
        return (struct cstl_deque*)0;

    my_deque->capacity = capacity < 8 ? 8 : capacity;
    my_deque->elements = (struct cstl_object **)malloc(my_deque->capacity * sizeof(struct cstl_object*));
    if (!my_deque->elements) {
        free(my_deque);
        return (struct cstl_deque*)0;
    }

    my_deque->compare_fn = fn_c;
    my_deque->destruct_fn = fn_d;
    my_deque->head = (int)my_deque->capacity / 2;
    my_deque->tail = my_deque->head + 1;
    my_deque->index = 0;

    return my_deque;
}

cstl_error push_back_cstl_deque(struct cstl_deque *my_deque, void *elem, size_t elem_size)
{
    cstl_error ret = CSTL_SUCCESS;
    if (!my_deque)
        return CSTL_DEQUE_NOT_INITIALIZED;

    if (my_deque->tail == my_deque->capacity)
        my_deque = grow_deque(my_deque);

    ret = insert_cstl_deque(my_deque, my_deque->tail, elem, elem_size);
    if (ret == CSTL_SUCCESS)
        my_deque->tail++;

    return ret;
}

cstl_error push_front_cstl_deque(struct cstl_deque *my_deque, void *elem, size_t elem_size)
{
    cstl_error ret = CSTL_SUCCESS;
    int to = 0, from = 0, count = 0;

    if (!my_deque)
        return CSTL_DEQUE_NOT_INITIALIZED;

    if (my_deque->head == 0) {
        my_deque = grow_deque(my_deque);
        to = (my_deque->capacity - my_deque->index) / 2;
        from = my_deque->head + 1;
        count = my_deque->tail - my_deque->head;
        memmove(&(my_deque->elements[to]), &(my_deque->elements[from]), count * sizeof(struct cstl_object*));
        my_deque->head = to - 1;
        my_deque->tail = my_deque->head + count;
    }
    ret = insert_cstl_deque(my_deque, my_deque->head, elem, elem_size);
    if (ret == CSTL_SUCCESS)
        my_deque->head--;

    return ret;
}

static cstl_error element_at_cstl_deque(struct cstl_deque *my_deque, int index, void **elem)
{
    cstl_error ret = CSTL_SUCCESS;

    if (!my_deque)
        return CSTL_DEQUE_NOT_INITIALIZED;
    
    ret = get_data_cstl_object(my_deque->elements[index], elem);

    return ret;
}

cstl_error front_cstl_deque(struct cstl_deque *my_deque, void **elem)
{
    return element_at_cstl_deque(my_deque, my_deque->head+1, elem);
}

cstl_error back_cstl_deque(struct cstl_deque *my_deque, void **elem)
{
    return element_at_cstl_deque(my_deque, my_deque->tail-1, elem);
}

cstl_error pop_back_cstl_deque(struct cstl_deque *my_deque)
{
    if (!my_deque)
        return CSTL_DEQUE_NOT_INITIALIZED;

    if (my_deque->destruct_fn) {
        void *elem;
        if (element_at_cstl_deque(my_deque, my_deque->tail-1, &elem) == CSTL_SUCCESS)
            my_deque->destruct_fn(elem);
    }

    delete_cstl_object(my_deque->elements[my_deque->tail - 1]);
    my_deque->tail--;
    my_deque->index--;

    return CSTL_SUCCESS;
}

cstl_error pop_front_cstl_deque(struct cstl_deque *my_deque)
{
    if (!my_deque)
        return CSTL_DEQUE_NOT_INITIALIZED;

    if (my_deque->destruct_fn) {
        void *elem;
        if (element_at_cstl_deque(my_deque, my_deque->head+1, &elem) == CSTL_SUCCESS)
            my_deque->destruct_fn(elem);
    }

    delete_cstl_object(my_deque->elements[my_deque->head + 1]);
    my_deque->head++;
    my_deque->index--;

    return CSTL_SUCCESS;
}

cstl_bool empty_cstl_deque(struct cstl_deque *my_deque)
{
    if (!my_deque)
        return cstl_true;

    return my_deque->index == 0 ? cstl_true : cstl_false;
}

int size_cstl_deque(struct cstl_deque *my_deque)
{
    if (!my_deque)
        return cstl_true;

    return my_deque->index;
}

cstl_error delete_cstl_deque(struct cstl_deque* my_deque)
{
    if (!my_deque)
        return CSTL_SUCCESS;

    if (my_deque->destruct_fn) {
        for (int i = my_deque->head + 1; i < my_deque->tail; i++) {
            void *elem;
            if (element_at_cstl_deque(my_deque, i, &elem) == CSTL_SUCCESS)
                my_deque->destruct_fn(elem);
        }
    }

    for (int i = my_deque->head + 1; i < my_deque->tail; i++)
        delete_cstl_object(my_deque->elements[i]);

    free(my_deque->elements);
    free(my_deque);

    return CSTL_SUCCESS;
}

static struct cstl_object* get_next_cstl_deque(struct cstl_iterator* my_itr)
{
    struct cstl_deque *my_deque = (struct cstl_deque*)my_itr->container;

    if (my_itr->current < 0 || my_itr->current >= my_deque->tail)
        return (struct cstl_object*)0;

    my_itr->current_element = my_deque->elements[my_itr->current++];
    return my_itr->current_element;
}

static void replace_value_cstl_deque(struct cstl_iterator *my_itr, void *elem, size_t elem_size)
{
    struct cstl_deque *my_deque = (struct cstl_deque *)my_itr->container;

    if (my_deque->destruct_fn) {
        void *old_element;
        get_data_cstl_object(my_itr->current_element, &old_element);
        my_deque->destruct_fn(old_element);
    }
    replace_data_cstl_object(my_itr->current_element, elem, elem_size);
}

static void *get_value_cstl_deque(void *my_object)
{
    void *elem;
    get_data_cstl_object(my_object, &elem);
    return elem;
}

struct cstl_iterator *new_iterator_cstl_deque(struct cstl_deque *my_deque)
{
    struct cstl_iterator *my_itr = (struct cstl_iterator*)malloc(sizeof(struct cstl_iterator));
    my_itr->get_next = get_next_cstl_deque;
    my_itr->replace_value = replace_value_cstl_deque;
    my_itr->get_value = get_value_cstl_deque;
    my_itr->container = my_deque;
    my_itr->current = my_deque->head + 1;
    return my_itr;
}
void delete_iterator_cstl_deque(struct cstl_iterator *my_itr)
{
    free(my_itr);
}