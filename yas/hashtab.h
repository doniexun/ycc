#ifndef HASHTAB_H_
#define HASHTAB_H_

typedef struct hashtab hashtab;

extern hashtab *hashtab_new();
extern void *hashtab_insert(hashtab *tab, const char *key, void *data);
extern void *hashtab_lookup(hashtab *tab, const char *key);
extern int hashtab_remove(hashtab *tab, const char *key);

#endif
