/* rbtree_wrap.c - rb-tree wrap implementations */

#include <stddef.h>
#include <stdint.h>
#include "rbtree.h"


/* @func: rb_insert_wrap - rb-tree node insert
* @param1: struct rb_root * # root node
* @param2: struct rb_node * # new node
* @param3: int32 (*)(void *, void *) # cmp callback
* @return: struct rb_node * # return new node / NULL pointer
*/
struct rb_node *rb_insert_wrap(struct rb_root *root,
		struct rb_node *new, int32_t (*cmp)(void *, void *)) {
	struct rb_node **link = &root->node, *parent = NULL;
	while (*link) {
		parent = *link;
		int32_t m = cmp(parent, new);
		if (m < 0) {
			link = &parent->left;
		} else if (m > 0) {
			link = &parent->right;
		} else {
			return NULL;
		}
	}

	new->left = new->right = NULL;
	new->parent = parent;
	new->color = RB_RED;
	*link = new;
	rb_insert_fix(root, new);

	return new;
} /* end */

/* @func: rb_search_wrap - rb-tree node search
* @param1: struct rb_root * # root node
* @param2: void *           # callback arg
* @param3: int32 (*)(void *, void *) # cmp callback
* @return: struct rb_node * # return search node / NULL pointer
*/
struct rb_node *rb_search_wrap(struct rb_root *root,
		void *arg, int32_t (*cmp)(void *, void *)) {
	struct rb_node **link = &root->node, *parent = NULL;
	while (*link) {
		parent = *link;
		int32_t m = cmp(parent, arg);
		if (m < 0) {
			link = &parent->left;
		} else if (m > 0) {
			link = &parent->right;
		} else {
			return parent;
		}
	}

	return NULL;
} /* end */

/* @func: rb_erase_wrap - rb-tree node erase
* @param1: struct rb_root * # root node
* @param2: struct rb_node * # old node
* @return: struct rb_node * # return erase node / NULL pointer
*/
struct rb_node *rb_erase_wrap(struct rb_root *root,
		struct rb_node *old) {
	if (old)
		rb_erase_fix(root, old);

	return old;
} /* end */

/* @func: rb_erase2_wrap - rb-tree node search
* @param1: struct rb_root * # root node
* @param2: void *           # callback arg
* @param3: int32 (*)(void *, void *) # cmp callback
* @return: struct rb_node * # return erase node / NULL pointer
*/
struct rb_node *rb_erase2_wrap(struct rb_root *root,
		void *arg, int32_t (*cmp)(void *, void *)) {
	struct rb_node *old = rb_search_wrap(root, arg, cmp);
	if (old)
		rb_erase_fix(root, old);

	return old;
} /* end */
