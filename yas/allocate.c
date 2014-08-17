#include <stdlib.h>
#include "error.h"


struct memblock{
	struct memblock *next;
	char *limit;
	char *avail;
};


/* memory pool */
static struct memblock first[] = { { NULL }, { NULL }, { NULL } };	/* simplify the initiation */
static struct memblock *arena[] = { &first[0], &first[1], &first[2] };
static struct memblock *freeblock;

/* align the size */
#define ROUNDUP(n, a) \
	((n)+((a)-1)) & (~((a) - 1))

#define BLOCK_AVAIL(p) \
	(p->limit - p->avail)


union align{
	char *pc;
	int i;
	long l;
	long *pl;
	float f;
	double d;
	long double ld;
	int(*fp)(void);
};

union header{
	union align a;
	struct memblock m;
};


void *allocate(size_t n, int a)
{
	struct memblock *p;
	int m;
	n = ROUNDUP(n, sizeof(union align));
	while (arena[a]->limit - arena[a]->avail < n){
		if (p = freeblock){
			freeblock = freeblock->next;
		}
		else{
			m = sizeof(union header) + n + 1024 * 10;	/* no need to roundup */
			p = (struct memblock*)malloc(m);
			if (!p){
				fatal("not enough memory.");
				return NULL;
			}
		}
		*p = *arena[a];
		arena[a]->avail = (char*)((union header*)p + 1);
		arena[a]->limit = (char*)p + m;
		arena[a]->next = p;
	}
	arena[a]->avail += n;
	return arena[a]->avail - n;
}

void *allocate_array(size_t n, unsigned long c, int a)
{
	return allocate(n * c, a);
}

void deallocate(int a)
{
	struct memblock *p;
	while (p = arena[a]->next){
		arena[a]->next->next = freeblock;
		freeblock = arena[a]->next;
		*arena[a] = *p;
	}
}