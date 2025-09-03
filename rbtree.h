/* rbtree.h - red-black self-balancing binary tree definitions */

#ifndef RBTREE_H
#define RBTREE_H

#include <stddef.h>
#include <stdint.h>


/* @def: rbtree */
#undef rb_node
struct rb_node {
	int32_t color;
	struct rb_node *parent, *left, *right;
};

#undef rb_root
struct rb_root {
	struct rb_node *node;
};

#undef RB_ROOT_NEW
#define RB_ROOT_NEW(x) struct rb_root x = { NULL }
#undef RB_ROOT_INIT
#define RB_ROOT_INIT(x) (x)->node = NULL

#undef RB_RED
#define RB_RED 0
#undef RB_BLACK
#define RB_BLACK 1
/* end */

#ifdef __cplusplus
extern "C" {
#endif

/* rbtree.c */
extern void rb_change_child(struct rb_root *root, struct rb_node *parent,
		struct rb_node *old, struct rb_node *_new);
extern void rb_left_rotate(struct rb_root *root, struct rb_node *parent);
extern void rb_right_rotate(struct rb_root *root, struct rb_node *parent);
extern void rb_insert_fix(struct rb_root *root, struct rb_node *node);
extern void rb_erase_fix(struct rb_root *root, struct rb_node *node);

/* rbtree_wrap.c */
extern struct rb_node *rb_insert_wrap(struct rb_root *root,
		struct rb_node *_new, int32_t (*cmp)(void *, void *));
extern struct rb_node *rb_search_wrap(struct rb_root *root,
		void *arg, int32_t (*cmp)(void *, void *));
extern struct rb_node *rb_erase_wrap(struct rb_root *root,
		struct rb_node *old);
extern struct rb_node *rb_erase2_wrap(struct rb_root *root,
		void *arg, int32_t (*cmp)(void *, void *));

#ifdef __cplusplus
}
#endif


#endif
