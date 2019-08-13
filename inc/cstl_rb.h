#ifndef _CSTL_RBTREE_H_
#define _CSTL_RBTREE_H_

#ifdef __cplusplus
extern "C" {
#endif

#define RB_RED   0
#define RB_BLACK 1

struct rb_node {
	unsigned long __rb_parent_color;
	struct rb_node *right;
	struct rb_node *left;
}__attribute__((aligned(sizeof(long))));

struct rb_root {
	struct rb_node *rb_node;
};

#define rb_parent(r) ((struct rb_node *)((r)->__rb_parent_color & ~3))

#define rb_color(r) ((r)->__rb_parent_color & 1)
#define rb_is_red(r) (!rb_color(r))
#define rb_is_black(r) (rb_color(r))
#define rb_set_black(r) do { (r)->__rb_parent_color |= 1; } while(0)
#define rb_set_red(r) do { (r)->__rb_parent_color &= ~1; } while(0)

static inline void rb_set_parent(struct rb_node *rb, struct rb_node *p)
{
	rb->__rb_parent_color = rb_color(rb) | (unsigned long)p;
}

static inline void rb_set_color(struct rb_node *rb, int color)
{
	rb->__rb_parent_color = (rb->__rb_parent_color & ~1) | color;
}

static inline void rb_set_parent_color(struct rb_node *rb, struct rb_node *p, int color)
{
	rb->__rb_parent_color = (unsigned long)p | color;
}

#define RB_ROOT (struct rb_root) {NULL, }
#define offsetof(TYPE, MEMBER) ((unsigned int) &((TYPE *)0)->MEMBER)
#define container_of(ptr, type, member) ({ 		\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type, member) );})



extern void rb_insert_fixup(struct rb_node *, struct rb_root *);
extern void rb_remove_fixup(struct rb_node *, struct rb_node *, struct rb_root *);
extern void rb_remove(struct rb_node *, struct rb_root *);

static inline void rb_link_node(struct rb_node *node, struct rb_node *parent, struct rb_node **rb_link)
{
	node->__rb_parent_color = (unsigned long)parent;
	node->left = node->right = NULL;

	*rb_link = node;
}

#ifdef __cplusplus
}
#endif
#endif
