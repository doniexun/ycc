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

enum chartype{
	DIGIT = 1 << 0,
	OCT = 1 << 1,
	HEX = 1 << 2,
	LETTER = 1 << 3,
	BLANK = 1 << 4,
	NEWLINE = 1 << 5,
	SIGN =  1 << 6,
	EOB = 1 << 7,
	OTHER = 1 << 8
};

#define CH_EOB 0

#endif
