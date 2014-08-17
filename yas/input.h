#ifndef INPUT_H_
#define INPUT_H_

#include <stdio.h>

extern char *pc;
extern char *line;
extern char *limit;
extern char *filepath;
extern FILE* src;
extern unsigned lineno;
extern unsigned column;

#define CH_EOB 0

#endif
