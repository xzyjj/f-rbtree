/* rbtree.c - red-black self-balancing binary tree implementations */

#include <stddef.h>
#include <stdint.h>
#include "rbtree.h"


/* @func: rb_change_child - change parent node's child
* @param1: struct rb_root * # root node
* @param2: struct rb_node * # parent node
* @param2: struct rb_node * # old node
* @param2: struct rb_node * # new node
* @return: void
*/
void rb_change_child(struct rb_root *root, struct rb_node *parent,
		struct rb_node *old, struct rb_node *new) {
	if (!parent) {
		root->node = new;
	} else if (parent->left == old) {
		parent->left = new;
	} else {
		parent->right = new;
	}
} /* end */

/* @func: rb_left_rotate - rb-tree left rotate
* @param1: struct rb_root * # root node
* @param2: struct rb_node * # rotate node's parent
* @return: void
*      g           g
*     / \         / \
*    p   u  -->  n   u
*   / \         / \
*  s   n       p   r
*     / \     / \
*    l   r   s   l
*/
void rb_left_rotate(struct rb_root *root, struct rb_node *parent) {
	struct rb_node *gparent, *node, *child;
	gparent = parent->parent;
	node = parent->right;
	child = node->left;

	/* 'left node' becomes 'parent right' */
	parent->right = child;
	if (child)
		child->parent = parent;

	/* 'parent' becomes 'left node' */
	node->left = parent;
	parent->parent = node;

	/* 'node' becomes 'gparent child' */
	rb_change_child(root, gparent, parent, node);
	node->parent = gparent;
} /* end */

/* @func: rb_right_rotate - rb-tree right rotate
* @param1: struct rb_root * # root node
* @param2: struct rb_node * # rotate node's parent
* @return: void
*       g         g
*      / \       / \
*     p   u --> n   u
*    / \       / \
*   n   s     l   p
*  / \           / \
* l   r         r   s
*/
void rb_right_rotate(struct rb_root *root, struct rb_node *parent) {
	struct rb_node *gparent, *node, *child;
	gparent = parent->parent;
	node = parent->left;
	child = node->right;

	/* 'right node' becomes 'parent left' */
	parent->left = child;
	if (child)
		child->parent = parent;

	/* 'parent' becomes 'right node' */
	node->right = parent;
	parent->parent = node;

	/* 'node' becomes 'gparent child' */
	rb_change_child(root, gparent, parent, node);
	node->parent = gparent;
} /* end */

/* @func: rb_insert_fix - rb-tree insert fix
* @param1: struct rb_root * # root node
* @param2: struct rb_node * # fix node
* @return: void
*/
void rb_insert_fix(struct rb_root *root, struct rb_node *node) {
	struct rb_node *parent, *gparent, *uncle;
	while (1) {
		parent = node->parent;
		if (!parent) {
			node->color = RB_BLACK;
			break;
		}
		if (parent->color == RB_BLACK)
			break;
		/* gparent and parent are Black and Red */

		gparent = parent->parent;
		uncle = gparent->right;
		if (parent != uncle) { /* parent is left */
			/* uncle is Red
			*     G         g
			*    / \       / \
			*   p   u --> P   U
			*  /         /
			* n         n
			*/
			if (uncle && uncle->color == RB_RED) {
				parent->color = RB_BLACK;
				uncle->color = RB_BLACK;
				gparent->color = RB_RED;
				node = gparent;
				continue;
			}

			/* LR
			*   G         G
			*  / \       / \
			* p   U --> p   U
			*  \       /
			*   n     n
			*/
			if (node == parent->right) {
				rb_left_rotate(root, parent);
				parent = node;
			}

			/* LL
			*     G         P
			*    / \       / \
			*   p   U --> n   g
			*  /               \
			* n                 U
			*/
			rb_right_rotate(root, gparent);
			parent->color = RB_BLACK;
			gparent->color = RB_RED;

			break;
		} else { /* parent is right */
			uncle = gparent->left;
			/* uncle is Red
			*     G         g
			*    / \       / \
			*   u   p --> U   P
			*        \         \
			*         n         n
			*/
			if (uncle && uncle->color == RB_RED) {
				parent->color = RB_BLACK;
				uncle->color = RB_BLACK;
				gparent->color = RB_RED;
				node = gparent;
				continue;
			}

			/* RL
			*   G         G
			*  / \       / \
			* U   p --> U   p
			*    /           \
			*   n             n
			*/
			if (node == parent->left) {
				rb_right_rotate(root, parent);
				parent = node;
			}

			/* RR
			*   G          P
			*  / \        / \
			* U   p -->  g   n
			*      \    /
			*       n  U
			*/
			rb_left_rotate(root, gparent);
			parent->color = RB_BLACK;
			gparent->color = RB_RED;

			break;
		}
	}
} /* end */

/* @func: _rb_erase (static) - rb-tree erase node
* @param1: struct rb_root * # root node
* @param2: struct rb_node * # erase node
* @return: struct rb_node * # fix's parent node or NULL
*/
static struct rb_node *_rb_erase(struct rb_root *root, struct rb_node *node) {
	struct rb_node *parent = node->parent, *left = node->left,
		*right = node->right, *successor, *child;
	/*
	*     (p)          (p)
	*     / \          /  \
	*   (n)  (s) --> (r)  (s)
	*   / \
	* nil  (r)
	*
	*     (p)           (p)
	*     / \          /  \
	*   (n)  (s) --> (r)  (s)
	*   / \
	* nil  nil
	*/
	if (!left) { /* no left child */
		rb_change_child(root, parent, node, right);
		if (right) { /* only right child */
			/* 'node right' becomes 'node' */
			right->parent = parent;
			right->color = node->color;
			return NULL;
		}
		/* no left and right child */

		return (node->color == RB_BLACK) ? parent : NULL;
	}
	/*
	*     (p)          (p)
	*     / \          / \
	*   (n)  (s) --> (l)  (s)
	*   / \
	* (l)  nil
	*/
	if (!right) { /* only left child */
		/* 'node left' becomes 'node' */
		rb_change_child(root, parent, node, left);
		left->parent = parent;
		left->color = node->color;
		return NULL;
	}

	/*
	*    (n)           (r)         (r)
	*    / \           / \         / \
	* (l)  (r)  --> (l)  (n) --> (l)  (r2)
	*      /  \          / \
	*   nil  (r2)     nil  (r2)
	*
	*    (n)           (l2)           (l2)
	*    / \           /  \           /  \
	*  (l) (r)  --> (l)   (r)  -->  (l)  (r)
	*      /  \           / \            / \
	*   (l2)  (r2)      (n)  (r2)    (r3)  (r2)
	*   /  \            / \
	* nil  (r3)      nil  (r3)
	*/
	parent = successor = right;
	child = successor->right;
	if (successor->left) {
		do {
			parent = successor;
			successor = successor->left;
		} while (successor->left);

		/* 'successor right' becomes 'parent left' */
		child = parent->left = successor->right;

		/* 'node right' becomes 'successor right' */
		successor->right = right;
		right->parent = successor;
	}

	/* 'node left' becomes 'successor left' */
	successor->left = left;
	left->parent = successor;

	/* 'successor' becomes 'node parent child' */
	rb_change_child(root, node->parent, node, successor);
	successor->parent = node->parent;

	if (child) {
		/* '^successor right' becomes Black */
		child->parent = parent;
		child->color = RB_BLACK;
		parent = NULL;
	} else {
		parent = (successor->color == RB_BLACK) ? parent : NULL;
	}
	/* 'successor' acquires 'node' color */
	successor->color = node->color;

	return parent;
} /* end */

/* @func: _rb_erase_color (static) - rb-tree erase color fix
* @param1: struct rb_root * # root node
* @param2: struct rb_node * # parent node
* @return: void
*/
static void _rb_erase_color(struct rb_root *root, struct rb_node *parent) {
	struct rb_node *left, *right, *sibling, *node = NULL;
	while (1) {
		sibling = parent->right;
		if (node != sibling) { /* node is left */
			/* sibling is Red
			*    P            S
			*   / \          / \
			* nil  s  -->   p   R
			*     / \      / \
			*    L   R   nil  L
			*/
			if (sibling && sibling->color == RB_RED) {
				sibling->color = RB_BLACK;
				parent->color = RB_RED;
				rb_left_rotate(root, parent);
				sibling = parent->right; /* sibling left */
			}

			left = sibling->left;
			right = sibling->right;
			if (!right || right->color == RB_BLACK) {
				if (!left || left->color == RB_BLACK) {
					/* left and right is Black
					*   (p)         (p)
					*   / \         / \
					* nil  S  --> nil  s
					*     / \         / \
					*    L   R       L   R
					*/
					sibling->color = RB_RED;
					if (parent->color == RB_RED) {
						parent->color = RB_BLACK;
					} else {
						node = parent;
						parent = node->parent;
						if (parent)
							continue;
					}

					break;
				}

				/* RL
				*   (p)         (p)
				*   / \         / \
				* nil  S  --> nil  l
				*     / \           \
				*    l   R           S
				*                     \
				*                      R
				*/
				rb_right_rotate(root, sibling);
				sibling = parent->right;
			}

			/* RR
			*    (p)            (l)
			*    / \            / \
			* nil   l          P   S
			*        \  -->   /     \
			*         S     nil      R
			*          \
			*           R
			*
			*   (p)          (s)
			*   / \          / \
			* nil  S  -->   P   R
			*     / \      / \
			*   (l)  r   nil  (l)
			*/
			sibling->color = parent->color;
			parent->color = RB_BLACK;
			if (sibling->right)
				sibling->right->color = RB_BLACK;
			rb_left_rotate(root, parent);

			break;
		} else { /* node is right */
			sibling = parent->left;
			/* sibling is Red
			*     P           S
			*    / \         / \
			*   s   nil --> L   p
			*  / \             / \
			* L   R           R  nil
			*/
			if (sibling && sibling->color == RB_RED) {
				sibling->color = RB_BLACK;
				parent->color = RB_RED;
				rb_right_rotate(root, parent);
				sibling = parent->left; /* sibling right */
			}

			left = sibling->left;
			right = sibling->right;
			if (!left || left->color == RB_BLACK) {
				if (!right || right->color == RB_BLACK) {
					/* left and right is Black
					*    (p)         (p)
					*    / \         / \
					*   S  nil -->  s   nil
					*  / \         / \
					* L   R       L   R
					*/
					sibling->color = RB_RED;
					if (parent->color == RB_RED) {
						parent->color = RB_BLACK;
					} else {
						node = parent;
						parent = node->parent;
						if (parent)
							continue;
					}

					break;
				}

				/* LR
				*    (p)         (p)
				*    / \         / \
				*   S  nil -->  r  nil
				*  / \         /
				* L   r       S
				*            /
				*           L
				*/
				rb_left_rotate(root, sibling);
				sibling = parent->left;
			}

			/* LL
			*      (p)          (r)
			*      / \          / \
			*     r   nil -->  S   P
			*    /            /     \
			*   S            L      nil
			*  /
			* L
			*
			*    (p)        (s)
			*    / \        / \
			*   S  nil --> L   P
			*  / \            / \
			* l  (r)        (r)  nil
			*/
			sibling->color = parent->color;
			parent->color = RB_BLACK;
			if (sibling->left)
				sibling->left->color = RB_BLACK;
			rb_right_rotate(root, parent);

			break;
		}
	}
} /* end */

/* @func: rb_erase_fix - rb-tree erase fix
* @param1: struct rb_root * # root node
* @param2: struct rb_node * # erase node
* @return: void
*/
void rb_erase_fix(struct rb_root *root, struct rb_node *node) {
	node = _rb_erase(root, node);
	if (node)
		_rb_erase_color(root, node);
} /* end */
