#include "cstl_lib.h"
/***********************************************************************
red-black trees rules:
	1. A node is either red or black
	2. The root is black
	3. All leaves (NULL) are black
	4. Both children of every red node are black
	5. Every simple path from root to leaves contains the same number of black nodes
************************************************************************/
/***********************************************************************
函数名：__left_rotate
参数：旋转节点*x，红黑树*root
功能描述： 对节点进行左旋操作
         px                px
        /                 /
       x      ----->     y
      / \               / \
     lx y              x  ry
       / \            / \
      ly ry          lx ly
************************************************************************/
static void __left_rotate(struct rb_node *x, struct rb_root *root)
{
	struct rb_node *y = x->right;
	struct rb_node *px = rb_parent(x);
	x->right = y->left;
	if (y->left) 
		rb_set_parent(y->left, x);
	rb_set_parent(y, px);
	if (px) {
		if (x == px->left)
			px->left = y;
		else
			px->right = y;
	} else
		root->rb_node = y;
	y->left = x;
	rb_set_parent(x, y);
}

/***********************************************************************
函数名：__right_rotate
参数：旋转节点*x，红黑树*root
功能描述： 对节点进行右旋操作

        px                px
       /                 /
      x      ----->     y
     / \               / \
    y  rx             ly x
   / \                  / \
  ly ry                ry rx

************************************************************************/
static void __right_rotate(struct rb_node *x, struct rb_root *root)
{
	struct rb_node *y = x->left;
	struct rb_node *px = rb_parent(x);
	x->left = y->right;
	if (y->right)
		rb_set_parent(y->right, x);
	rb_set_parent(y, px);
	if (px) {
		if (x == px->right)
			px->right = y;
		else
			px->left = y;
	} else
		root->rb_node = y;
	y->right = x;
	rb_set_parent(x, y);
}

/***********************************************************************
函数名：rb_insert_fixup
参数：新插入节点*node，红黑树*root
功能描述：红黑树插入修正函数，将红黑树进行调整使其达到平衡
***********************************************************************/
void rb_insert_fixup(struct rb_node *node, struct rb_root *root)
{
	struct rb_node *parent, *gparent;
	
	// 当父节点存在，且父节点为红色 (若父节点是黑色，则直接插入即可）
	while ((parent = rb_parent(node)) && rb_is_red(parent)) { 
		gparent = rb_parent(parent);
		if (parent == gparent->left) { // 父节点是祖父节点的左孩子
			struct rb_node *uncle = gparent->right;
			if (uncle && rb_is_red(uncle)) { // 叔叔节点为红色
				rb_set_black(uncle);
				rb_set_black(parent);
				rb_set_red(gparent);
				node = gparent;
				continue;
			} else { // 叔叔节点为黑色
				if (node == parent->right) { // 当前节点是右孩子
					struct rb_node *tmp;
					__left_rotate(parent, root);
					tmp = parent;
					parent = node;
					node = tmp;
					
				}
				//当前节点是左孩子
				rb_set_black(parent);
				rb_set_red(gparent);
				__right_rotate(gparent, root);
			}
		} else { // 父节点是祖父节点的右孩子
			struct rb_node *uncle = gparent->left;
			if (uncle && rb_is_red(uncle)) { // 叔叔节点为红色
				rb_set_black(uncle);
				rb_set_black(parent);
				rb_set_red(gparent);
 				node = gparent;
                                continue;
			} else { // 叔叔节点为黑色
				if (node == parent->left) { // 当前节点是左孩子
					struct rb_node *tmp;
					__right_rotate(parent, root);
					tmp = parent;
					parent = node;
					node = tmp;
				}
				// 当前节点是右孩子
				rb_set_black(parent);
				rb_set_red(gparent);
				__left_rotate(gparent, root);
			}
		}
	}
	rb_set_black(root->rb_node);
}
/******************************************************************
函数名：rb_remove_fixup
参数：新插入节点node，红黑树root
功能描述：红黑树删除修正函数，将红黑树调整使其重新平衡
******************************************************************/
void rb_remove_fixup(struct rb_node *node, struct rb_node *parent, struct rb_root *root)
{
	struct rb_node *other;

	while ((!node || rb_is_black(node)) && node != root->rb_node) { //节点是黑色(或是NULL），并且不是根节点
		if ( node == parent->left) {
			other = parent->right;
			if (rb_is_red(other)) { // 兄弟是红色的
				rb_set_black(other);
				rb_set_red(parent);
				__left_rotate(parent, root);
				other = parent->right;
			}
			if ((!other->left || rb_is_black(other->left)) && (!other->right || rb_is_black(other->right))) { // 兄弟是黑色，且兄弟的左右两儿子也是黑色
				rb_set_red(other);
				node = parent;
				parent = rb_parent(node);
			} else {
				if (!other->right || rb_is_black(other->right)) { // 兄弟是黑色，兄弟孩子左红右黑
					rb_set_black(other->left);
					rb_set_red(other);
					__right_rotate(other, root);
					other = parent->right;
				}
				// 兄弟是黑色 兄弟孩子右红，左无所谓
				rb_set_color(other, rb_color(parent));
				rb_set_black(parent);
				rb_set_black(other->right);
				__left_rotate(parent,root);
				node = root->rb_node;
				break;
			}
		} else {
			other = parent->left;
			if (rb_is_red(other)) {
				rb_set_black(other);
				rb_set_red(parent);
				__right_rotate(parent, root);
				other = parent->left;
			}
			if ((!other->left || rb_is_black(other->left)) && (!other->right || rb_is_black(other->right))) {
				rb_set_red(other);
				node = parent;
				parent = rb_parent(node);
			} else {
				if (!other->left || rb_is_black(other->left)) { // 兄弟是黑色， 兄弟孩子左黑右红
					rb_set_black(other->right);
					rb_set_red(other);
					__left_rotate(other, root);
					other = parent->left;
				}			
				// 兄弟是黑色，兄弟孩子左红右黑
				rb_set_color(other, rb_color(parent));
				rb_set_black(parent);
				rb_set_black(other->left);
				__right_rotate(parent, root);
				node = root->rb_node;
			}
		}	
	}
	if (node)
		rb_set_black(node);
}

/******************************************************************
函数名：rb_remove
参数：新插入节点node，红黑树root
功能描述：删除节点node
******************************************************************/
void rb_remove(struct rb_node *node, struct rb_root *root)
{
	struct rb_node *child, *parent;
	int color;

	if (!node->left) // 移除的节点只有一个孩子
		child = node->right;
	else if (!node->right)
		child = node->left;
	else { // 移除的节点有两个孩子
		struct rb_node *old = node;
		struct rb_node *left;

		node = node->right;
		while ((left = node->left) != NULL) // 找到后继节点
			node = left;

		// 删除节点的父节点指向后继节点
		if (rb_parent(old)) {
			if (rb_parent(old)->left == old)
				rb_parent(old)->left = node;
			else
				rb_parent(old)->right = node;
		} else
			root->rb_node = node;

		child = node->right;
		parent = rb_parent(node);
		color = rb_color(node);

		// 后继节点替换移除的节点
		if (parent == old) 
			parent = node;
		else {
			if (child) // 如果后继节点的子节点存在
				rb_set_parent(child, parent);
			parent->left = child;
			
			node->right = old->right;
			rb_set_parent(old->right, node);
		}

		node->__rb_parent_color = old->__rb_parent_color;
		node->left = old->left;
		rb_set_parent(old->left, node);

		goto color;
	}
	
	parent = rb_parent(node);
	color = rb_color(node);

	if (child)
		rb_set_parent(child, parent);
	if (parent) {
		if (parent->left == node)
			parent->left = child;
		else
			parent->right = child;
	} else
		root->rb_node = child;

color:
	if (color == RB_BLACK)
		rb_remove_fixup(child, parent, root);
}
