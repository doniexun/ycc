#ifndef ALLOCA_H_
#define ALLOCA_H_

#include <stdlib.h>

extern void *allocate(size_t n, int a);
extern void dealloca(size_t n);
extern void *allocate_array(size_t n, unsigned long c, int a);

enum ARENA { PERM = 1, FUNC, STMT };

#define NEW(p, a) \
	(p = allocate(sizeof(*p), (a)))

#define NEW0(p, a) \
	(memset(NEW(p, a), 0, sizeof(*p)))


#endif
