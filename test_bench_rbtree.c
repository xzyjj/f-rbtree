#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include "xstddef.h"
#include "xstdlib.h"
#include "rbtree.h"


#undef NODE_SIZE
#define NODE_SIZE 1000000

struct T {
	int32_t key;
	struct rb_node node;
};

static int32_t _cmp(void *node, void *new) {
	int32_t key = container_of(node, struct T, node)->key;
	int32_t new_key = container_of(new, struct T, node)->key;

	return ((new_key > key) ? 1 : ((new_key < key) ? -1 : 0));
}

static int32_t _search_cmp(void *node, void *arg) {
	int32_t key = container_of(node, struct T, node)->key;
	int32_t s_key = *((int32_t *)arg);

	return ((s_key > key) ? 1 : ((s_key < key) ? -1 : 0));
}

int main(void) {
	RB_ROOT_NEW(root);
	struct T *T = malloc(sizeof(struct T) * NODE_SIZE);
	clock_t start, end;
	double time;

	RANDOM_TYPE0_NEW(rand, 123456);
	for (int32_t i = 0; i < NODE_SIZE; i++) {
#if 1

		xrandom_r(&rand, &T[i].key);

#else

		T[i].key = i;

#endif
	}

	/* insert */
	start = clock();
	for (int32_t i = 0; i < NODE_SIZE; i++) {
		rb_insert_wrap(&root, &T[i].node, _cmp);
	}
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("insert: %d, time: %.6fs (%.2f/s), %.2fns/op\n",
		NODE_SIZE, time, (double)NODE_SIZE / time,
		(double)(time * 1000 * 1000 * 1000) / NODE_SIZE);

	/* search */
	start = clock();
	for (int32_t i = 0; i < NODE_SIZE; i++) {
		rb_search_wrap(&root, &T[i].key, _search_cmp);
	}
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("search: %d, time: %.6fs (%.2f/s), %.2fns/op\n",
		NODE_SIZE, time, (double)NODE_SIZE / time,
		(double)(time * 1000 * 1000 * 1000) / NODE_SIZE);

	/* erase */
	start = clock();
	for (int32_t i = 0; i < (NODE_SIZE / 10); i++) {
		rb_erase2_wrap(&root, &T[i].node, _cmp);
	}
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("erase: %d, time: %.6fs (%.2f/s), %.2fns/op\n",
		NODE_SIZE / 10, time, (double)(NODE_SIZE / 10) / time,
		(double)(time * 1000 * 1000 * 1000) / NODE_SIZE);

	/* search */
	start = clock();
	for (int32_t i = (NODE_SIZE / 10); i < ((NODE_SIZE / 10) + 10); i++) {
		printf("search: %d %p %p\n", T[i - 1].key, &T[i - 1].node,
			rb_search_wrap(&root, &T[i - 1].node, _cmp));
	}
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("search: %d, time: %.6fs (%.2f/s)\n",
		10, time, (double)10 / time);

	/* search */
	start = clock();
	for (int32_t i = 0; i < NODE_SIZE; i++) {
		rb_search_wrap(&root, &T[i].key, _search_cmp);
	}
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("search: %d, time: %.6fs (%.2f/s), %.2fns/op\n",
		NODE_SIZE, time, (double)NODE_SIZE / time,
		(double)(time * 1000 * 1000 * 1000) / NODE_SIZE);

	/* erase */
	start = clock();
	for (int32_t i = 0; i < NODE_SIZE; i++) {
		rb_erase2_wrap(&root, &T[i].node, _cmp);
	}
	end = clock();
	time = (double)(end - start) / CLOCKS_PER_SEC;
	printf("erase: %d, time: %.6fs (%.2f/s), %.2fns/op\n",
		NODE_SIZE, time, (double)NODE_SIZE / time,
		(double)(time * 1000 * 1000 * 1000) / NODE_SIZE);

	return 0;
}
