/* xstddef.h - standard type definitions */

#ifndef XSTDDEF_H
#define XSTDDEF_H


/* @def: stddef
* struct member offset size */
#undef offsetof
#define offsetof(type, member) \
	((uint64_t)&(((type *)0)->member))

/* struct member and container offset size */
#undef container_of
#define container_of(ptr, type, member) \
	((type *)((char *)(ptr) - offsetof(type, member)))
/* end */


#endif
