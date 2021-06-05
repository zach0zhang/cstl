#include "cstl_lib.h"

struct cstl_map *new_cstl_map(cstl_compare c_fn, cstl_destory d_k_fn, cstl_destory d_v_fn)
{
	struct cstl_map *my_map = (struct cstl_map *)malloc(sizeof(struct cstl_map));
	if (!my_map)
		return (struct cstl_map *)0;

	my_map->root = RB_ROOT;
	my_map->compare_function = c_fn;
	my_map->destruct_key_function = d_k_fn;
	my_map->destruct_value_function = d_v_fn;

	return my_map;
}

cstl_error insert_cstl_map(struct cstl_map *my_map, void *key, size_t key_size, void *value, size_t value_size)
{
	cstl_error ret = CSTL_SUCCESS;
	if (!my_map)
		return CSTL_MAP_NOT_INITIALIZED;

	struct cstl_map_node *my_node = (struct cstl_map_node *)malloc(sizeof(struct cstl_map_node));
	if (!my_node)
		return CSTL_ERROR_MEMORY;
	
	my_node->node.left = NULL;
	my_node->node.right = NULL;
	my_node->key = new_cstl_object(key, key_size);
	if (!my_node->key) {
		free(my_node);
		return CSTL_ERROR_MEMORY;
	}
	my_node->value = new_cstl_object(value, value_size);
	if (!my_node->value) {
		free(my_node);
		return CSTL_ERROR_MEMORY;
	}

	struct rb_node **tmp = &(my_map->root.rb_node), *parent = NULL;
	
	while (*tmp) {
		int c = 0;
		void *cur_key;
		void *new_key;
		struct cstl_map_node *now = container_of(*tmp, struct cstl_map_node, node);

		parent = *tmp;
		get_data_cstl_object(now->key, &cur_key);
		get_data_cstl_object(my_node->key, &new_key);

		c = (my_map->compare_function)(new_key, cur_key);
		free(cur_key);
		free(new_key);

		if (c < 0)
			tmp = &((*tmp)->left);
		else if (c > 0)
			tmp = &((*tmp)->right);
		else
			return CSTL_MAP_KEY_ALREADY_EXIST;
	}	
	
	rb_link_node(&my_node->node, parent, tmp);
	rb_insert_fixup(&my_node->node, &my_map->root);

	return ret;
}

static struct cstl_map_node *find_cstl_map_node(struct cstl_map *my_map, void *key)
{
	struct rb_node *my_node = my_map->root.rb_node;
	struct cstl_map_node *now;
	while (my_node) {
		now = container_of(my_node, struct cstl_map_node, node);
		int c = 0;
		void *cur_key;
		
		get_data_cstl_object(now->key, &cur_key);
		c = my_map->compare_function(key, cur_key);
		free(cur_key);

		if (c == 0)
			break;
		else 
			my_node = (c < 0 ? my_node->left : my_node->right);
	}

	if (!my_node)
		return (struct cstl_map_node *)0;

	return now;
}

cstl_bool exists_cstl_map(struct cstl_map *my_map, void *key)
{
	cstl_bool ret = cstl_false;
	struct cstl_map_node *node;

	if (!my_map)
		return ret;

	node = find_cstl_map_node(my_map, key);
	if (!node)
		return ret;

	return cstl_true;
}

cstl_bool find_cstl_map(struct cstl_map *my_map, void *key, void **value)
{
	cstl_bool ret = cstl_false;
	struct cstl_map_node *node;

	if (!my_map)
		return ret;

	node = find_cstl_map_node(my_map, key);
	if (!node)
		return ret;

	get_data_cstl_object(node->value, value);
	return cstl_true;
}

static inline void delete_cstl_map_node(struct cstl_map *my_map, struct cstl_map_node *my_node)
{
	rb_remove(&my_node->node, &my_map->root);
	if (my_map->destruct_key_function) 
		my_map->destruct_key_function(my_node->key);
	delete_cstl_object(my_node->key);
	if (my_map->destruct_value_function)
		my_map->destruct_value_function(my_node->value);
	delete_cstl_object(my_node->value);
}

cstl_error remove_cstl_map(struct cstl_map *my_map, void *key)
{
	cstl_error ret = CSTL_SUCCESS;
	struct cstl_map_node *my_node;

	if (!my_map)
		return CSTL_MAP_NOT_INITIALIZED;

	my_node = find_cstl_map_node(my_map, key);
	if (!my_node)
		return ret;
	else {
		/*
		rb_remove(&my_node->node, &my_map->root);
		if (my_map->destruct_key_function) 
			my_map->destruct_key_function(my_node->key);
		delete_cstl_object(my_node->key);
		if (my_map->destruct_value_function)
			my_map->destruct_key_function(my_node->value);
		delete_cstl_object(my_node->value);

		free(my_node);
		*/
		delete_cstl_map_node(my_map, my_node);
		free(my_node);
	}

	return ret;	
}

cstl_error delete_cstl_map(struct cstl_map *my_map)
{
	cstl_error ret = CSTL_SUCCESS;
	if (!my_map)
		return ret;
	struct rb_node *my_node = my_map->root.rb_node;

	while(my_node) {
		if (my_node->left) 
			my_node = my_node->left;
		else if (my_node->right)
			my_node = my_node->right;
		else {
			struct cstl_map_node *now = container_of(my_node, struct cstl_map_node, node);
			/*
			rb_remove(my_node, &my_map->root);
			if (my_map->destruct_key_function) 
				my_map->destruct_key_function(now->key);
			delete_cstl_object(now->key);
			if (my_map->destruct_value_function)
				my_map->destruct_key_function(now->value);
			delete_cstl_object(now->value);
			free(now);
			*/
			if(rb_parent(my_node)) 
				my_node = rb_parent(my_node);
			else
				my_node = NULL;
			delete_cstl_map_node(my_map, now);
			free(now);
			
		}
	}
	free(my_map);
	return ret;
}
static struct cstl_map_node *maximum_cstl_map_node(struct cstl_map *my_map)
{
	struct rb_node *my_node = my_map->root.rb_node;
	while (my_node->right)
		my_node = my_node->right;
	return container_of(my_node, struct cstl_map_node, node);
}
static struct cstl_map_node *minimum_cstl_map_node(struct cstl_map *my_map)
{
	struct rb_node *my_node = my_map->root.rb_node;
	while (my_node->left)
		my_node = my_node->left;
	return container_of(my_node, struct cstl_map_node, node);
}	
static struct cstl_object *get_next_cstl_map(struct cstl_iterator *my_itr)
{
	if (!my_itr->current_element)
		my_itr->current_element = minimum_cstl_map_node(my_itr->container);
	else {
		struct cstl_map *my_map = my_itr->container;
		struct rb_node *my_node = &((struct cstl_map_node *)my_itr->current_element)->node;
		struct rb_node *tmp;
		if (my_node->right) {
			my_node = my_node->right;
			while(my_node->left)
				my_node = my_node->left;
			my_itr->current_element = container_of(my_node, struct cstl_map_node, node);
		} else if ((struct cstl_map_node *)my_itr->current_element == maximum_cstl_map_node(my_itr->container)) 
			my_itr->current_element = (struct cstl_map_node *)0;
		else {
			tmp = rb_parent(my_node);
			while(tmp && my_node == tmp->right) {
				my_node = tmp;
				tmp = rb_parent(tmp);
			}
			my_itr->current_element = container_of(tmp, struct cstl_map_node, node);
		}
	}

	if (!my_itr->current_element)
		return (struct cstl_object *)0;

	return ((struct cstl_map_node*)my_itr->current_element)->value;
}

static void replace_value_cstl_map(struct cstl_iterator *my_itr, void *elem, size_t elem_size)
{
	struct cstl_map *my_map = my_itr->container;

	if (my_map->destruct_value_function) {
		void *old_elem;
		get_data_cstl_object(((struct cstl_map_node *)my_itr->current_element)->value, &old_elem);
		my_map->destruct_value_function(old_elem);
	}
	replace_data_cstl_object(((struct cstl_map_node *)my_itr->current_element)->value, elem, elem_size);
}

static void *get_value_cstl_map(void *my_object)
{
	void *elem;
	get_data_cstl_object(my_object, &elem);
	return elem;
}

struct cstl_iterator *new_iterator_cstl_map(struct cstl_map *my_map)
{
	struct cstl_iterator *my_itr = (struct cstl_iterator *)malloc(sizeof(struct cstl_iterator));
	my_itr->get_next = get_next_cstl_map;
	my_itr->replace_value = replace_value_cstl_map;
	my_itr->get_value = get_value_cstl_map;
	my_itr->container = my_map;
	my_itr->current = 0;
	my_itr->current_element = (void *)0;
	return my_itr;
}

void delete_iterator_cstl_map(struct cstl_iterator *my_itr)
{
	free(my_itr);
}
