#include "allocate.h"

#include <string.h>

#define HASHSIZE 2048

struct bucket{
	struct bucket *next;
	const void *key;
	void *data;
};

struct hashtab{
	size_t cnt;

	unsigned int(*hash)(const char *key);
	int(*equal)(const char *key1, const char *key2);

	struct bucket *buckets[HASHSIZE];
};

struct hashtab *hashtab_new(unsigned int hash(const char *key),
					int equal(const char *key1, const void *key2))
{
	struct hashtab *ret = NULL;
	size_t size = sizeof(*ret) + HASHSIZE *sizeof(struct bucket*);
	ret = allocate(size, 0);
	ret->cnt = 0;
	memset(ret + sizeof(*ret), 0, size - sizeof(*ret));
	
	assert(equal && hash);
	ret->equal = equal;
	ret->hash = hash;

	return ret;
}

void *hashtab_insert(struct hashtab *tab, const char *key, void *data)
{
	unsigned int pos = tab->hash(key);
	struct bucket *p = tab->buckets[pos];
	while (1){
		if (!p){
			p = allocate(sizeof(*p), 0);
			p->data = data;
			p->next = NULL;

			return p->data;
		}
		if (tab->equal(p->key, key)){
			return p->data;
		}
		else{
			p = p->next;
			continue;
		}
	}
}

void *hashtab_lookup(struct hashtab *tab, const char *key)
{
	unsigned int pos = tab->hash(key);
	struct bucket *p = tab->buckets[pos];

	while (pos){
		if (tab->equal(key, p->key)){
			return p->data;
		}
		else{
			p = p->next;
		}
	}

	return NULL;
}

int hashtab_remove(struct hashtab *tab, const char *key)
{
	unsigned int pos = tab->hash(key);
	struct bucket **pp = tab->buckets + pos;

	while (*pp){
		if (tab->equal(key, (*pp)->key)){
			pp = &((*pp)->next);
			return 1;
		}
		else{
			pp = &((*pp)->next);
		}
	}

	return 0;
}