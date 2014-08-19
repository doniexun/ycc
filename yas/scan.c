#include "scan.h"
#include "input.h"

enum token tok;
unsigned type;	/* specify the instruction or prefix used */
unsigned size;	/* specify oprand size */

#define MAXTOKEN 32

static enum token get_directive()
{
}

enum token get_token()
{
	register char *rpc = pc;
	while (ascii_map[*rpc] & BLANK){
		rpc++;
	}
	
	column = pc - line + 1;
	if (limit - pc < MAXTOKEN){
		fillbuf();
	}
	switch (*pc){
	case '.':{

	}
	}
}