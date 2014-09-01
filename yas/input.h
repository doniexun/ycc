#ifndef INPUT_H_
#define INPUT_H_

#include <stdio.h>
#include <assert.h>

extern char *pc;
extern char *line;
extern char *limit;
extern char *filepath;
extern FILE* src;
extern unsigned lineno;
extern unsigned column;

extern enum chartype ascii_map[];

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

extern int input_init();

/* skip blank characters */
#define SKIP_BLANK(p)	\
	assert(ascii_map[*p] & BLANK);\
	do{\
		p++;\
	} while (ascii_map[*p] & BLANK)

#define BUFFER_SIZE 2048
#define MAXLINE 512

#define CH_EOB 0

#endif
