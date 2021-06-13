#include "cstl_lib.h"

struct cstl_set *new_cstl_set(cstl_compare fn_c, cstl_destory fn_d)
{
    struct cstl_set *my_set = (struct cstl_set *)malloc(sizeof(struct cstl_set));
    if (!my_set)
        return (struct cstl_set *)0;

    my_set->root = RB_ROOT;
    my_set->compare_fn = fn_c;
    my_set->destruct_fn = fn_d;

    return my_set;
}

cstl_error insert_cstl_set(struct cstl_set *my_set, void *key, size_t key_size)
{
    if (!my_set)
        return CSTL_SET_NOT_INITIALIZED;

    struct cstl_set_node *my_node = (struct cstl_set_node *)malloc(sizeof(struct cstl_set_node));
    if (!my_node)
        return CSTL_ERROR_MEMORY;

    my_node->node.left = NULL;
    my_node->node.right = NULL;
    my_node->key = new_cstl_object(key, key_size);
    if (!my_node->key) {
        free(my_node);
        return CSTL_ERROR_MEMORY;
    }

    struct rb_node **tmp = &(my_set->root.rb_node), *parent = NULL;
    while (*tmp) {
        int c = 0;
        void *cur_key;
        void *new_key;
        struct cstl_set_node *now = container_of(*tmp, struct cstl_set_node, node);

        parent = *tmp;
        get_data_cstl_object(now->key, &cur_key);
        get_data_cstl_object(my_node->key, &new_key);

        c = (my_set->compare_fn)(new_key, cur_key);
        free(cur_key);
        free(new_key);

        if (c < 0)
            tmp = &((*tmp)->left);
        else if (c > 0)
            tmp = &((*tmp)->right);
        else
            return CSTL_SET_KEY_ALREADY_EXIST;
    }

    rb_link_node(&my_node->node, parent, tmp);
    rb_insert_fixup(&my_node->node, &my_set->root);

    return CSTL_SUCCESS;
}

static struct cstl_set_node *find_cstl_set_node(struct cstl_set *my_set, void *key)
{
    struct rb_node *my_node = my_set->root.rb_node;
    struct cstl_set_node *now;

    while (my_node) {
        now = container_of(my_node, struct cstl_set_node, node);
        int c = 0;
        void *cur_key;

        get_data_cstl_object(now->key, &cur_key);
        c = my_set->compare_fn(key, cur_key);
        free(cur_key);

        if (c == 0)
            break;
        else
            my_node = (c < 0 ? my_node->left : my_node->right);
    }

    if (!my_node)
        return (struct cstl_set_node *)0;

    return now;
}

static inline void delete_cstl_set_node(struct cstl_set *my_set, struct cstl_set_node *my_node)
{
    rb_remove(&my_node->node, &my_set->root);
    if (my_set->destruct_fn)
        my_set->destruct_fn(my_node->key);
    delete_cstl_object(my_node->key);
}


cstl_bool exists_cstl_set(struct cstl_set *my_set, void *key)
{
    if (!my_set)
        return cstl_false;

    struct cstl_set_node *node = find_cstl_set_node(my_set, key);
    
    return (!node) ? cstl_false : cstl_true;
}

cstl_bool find_cstl_set(struct cstl_set *my_set, void *key, void **outKey)
{
    if (!my_set)
        return cstl_false;

    struct cstl_set_node *node = find_cstl_set_node(my_set, key);
    if (!node)
        return cstl_false;

    get_data_cstl_object(node->key, outKey);
    return cstl_true;
}

cstl_error remove_cstl_set(struct cstl_set *my_set, void *key)
{

    if (!my_set)
        return CSTL_SET_NOT_INITIALIZED;

    struct cstl_set_node *my_node = find_cstl_set_node(my_set, key);
    if (my_node) {
        delete_cstl_set_node(my_set, my_node);
        free(my_node);
    }

    return CSTL_SUCCESS;
}

cstl_error delete_cstl_set(struct cstl_set *my_set)
{
    if (!my_set)
        return CSTL_SET_NOT_INITIALIZED;

    struct rb_node *my_node = my_set->root.rb_node;
    while (my_node) {
        if (my_node->left)
            my_node = my_node->left;
        else if (my_node->right)
            my_node = my_node->right;
        else {
            struct cstl_set_node *now = container_of(my_node, struct cstl_set_node, node);
            if (rb_parent(my_node))
                my_node = rb_parent(my_node);
            else
                my_node = NULL;
            delete_cstl_set_node(my_set, now);
            free(now);
        }
    }
    free(my_set);
    return CSTL_SUCCESS;
}

static struct cstl_set_node *maximum_cstl_cstl_node(struct cstl_set *my_set)
{
    struct rb_node *my_node = my_set->root.rb_node;
    while (my_node->right)
        my_node = my_node->right;
    return container_of(my_node, struct cstl_set_node, node);
}

static struct cstl_set_node *minimum_cstl_cstl_node(struct cstl_set *my_set)
{
    struct rb_node *my_node = my_set->root.rb_node;
    while (my_node->left)
        my_node = my_node->left;
    return container_of(my_node, struct cstl_set_node, node);
}

static struct cstl_object *get_next_cstl_set(struct cstl_iterator *my_itr)
{
    if (!my_itr->current_element)
        my_itr->current_element = minimum_cstl_cstl_node(my_itr->container);
    else {
        struct rb_node *my_node = &((struct cstl_set_node *)my_itr->current_element)->node;
        struct rb_node *tmp;
        if (my_node->right) {
            my_node = my_node->right;
            while (my_node->left)
                my_node = my_node->left;
            my_itr->current_element = container_of(my_node, struct cstl_set_node, node);
        } else if ((struct cstl_set_node *)my_itr->current_element == maximum_cstl_cstl_node(my_itr->container))
            my_itr->current_element = (struct cstl_set_node *)0;
        else {
            tmp = rb_parent(my_node);
            while (tmp && my_node == tmp->right) {
                my_node = tmp;
                tmp = rb_parent(tmp);
            }
            my_itr->current_element = container_of(tmp, struct cstl_set_node, node);
        }
    }

    if (!my_itr->current_element)
        return (struct cstl_object *)0;

    return ((struct cstl_set_node *)my_itr->current_element)->key;
}

static void *get_value_cstl_set(void *my_object)
{
    void *elem;
    get_data_cstl_object(my_object, &elem);
    return elem;
}

struct cstl_iterator *new_iterator_cstl_set(struct cstl_set *my_set)
{
    struct cstl_iterator *my_itr = (struct cstl_iterator *)malloc(sizeof(struct cstl_iterator));
    my_itr->get_next = get_next_cstl_set;
    my_itr->get_value = get_value_cstl_set;
    my_itr->container = my_set;
    my_itr->current = 0;
    my_itr->current_element = (void *)0;
    return my_itr;
}

void delete_iterator_cstl_set(struct cstl_iterator *my_itr)
{
    free(my_itr);
}