# cstl
cstl is a STL written in C.

compile:
```
make
```

## vector

```c
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
```

### vector sample file:
[test_cstl_vector](https://github.com/zach0zhang/cstl/blob/master/src/cstl_vector.c)



## queue
The queue can be automatically grown by the grow_auto parameter in new_cstl_queue function, if you do not want the capacity of the queue to increase indefinitely, you can specify a capacity, and use full_cstl_queue function to determine whether the queue is full.

The queue does not support access through iterators.

```c
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
```

### queue sample file:
[test_cstl_queue](https://github.com/zach0zhang/cstl/blob/master/test/test_cstl_queue.c)



## deque

```c
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
```

### deque sample file:
[test_cstl_deque](https://github.com/zach0zhang/cstl/blob/master/test/test_cstl_deque.c)



## stack
The stack is implemented by deque and does not support access through iterators.

```c
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
```

### stack sample file:
[test_cstl_stack](https://github.com/zach0zhang/cstl/blob/master/test/test_cstl_stack.c)


## map
The map is implemented by rea-black trees(cstl_rb.*)

```c
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
```

### map sample file:
[test_cstl_map](https://github.com/zach0zhang/cstl/blob/master/test/test_cstl_map.c)

## set
The implementation of set is similar to that of map, which is also implemented by red-black trees(cstl_rb.*)

```c
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
```

set sample file:
[test_cstl_set](https://github.com/zach0zhang/cstl/blob/master/test/test_cstl_set.c)