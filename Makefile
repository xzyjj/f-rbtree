CC = gcc
CFLAGS = -O1 -g -std=c99 -Wall -Wextra -Wpedantic -Wshadow \
	-Wno-format-pedantic -Wno-trigraphs -fno-builtin \
	-fno-inline-functions -finline-hint-functions
RM = rm

PROGRAMS = test_bench_rbtree
C_SOURCES = rbtree.c rbtree_wrap.c xstdlib_random.c
C_OBJECTS = $(C_SOURCES:.c=.o)


.PHONY: all
all: $(PROGRAMS)

test_bench_rbtree: $(C_OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ test_bench_rbtree.c

.PHONY: clean
clean:
	$(RM) -f $(C_OBJECTS) $(PROGRAMS)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $^
