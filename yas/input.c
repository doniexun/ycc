#include "error.h"
#include "input.h"

#define BUFFER_SIZE 2048
#define MAXLINE 512


static char buffer[BUFFER_SIZE + 1 + MAXLINE + 1];
char *pc;
char *line;
char *limit;
char *filepath;
FILE *src;
unsigned lineno;
unsigned column;

int input_init()
{
	size_t cnt = 0;
	src = fopen("filepath", "r");
	if (!src){
		fatal("file \"%s\" not exist.", filepath);
		return -1;	/* avoid warning */
	}
	
	cnt = fread(buffer + MAXLINE + 1, sizeof(char), BUFFER_SIZE, src);
	if (!cnt){
		fatal("file \"%s\" is empty.", filepath);
		return -1;
	}

	pc = buffer + MAXLINE + 1;
	line = pc;
	limit = pc + cnt;
	*limit = CH_EOB;
	lineno = 1;	/* no need to initiate column, it's calculate when needed */

	return 0;
}

/*																								*\
	when characters remained in buffer less than MAXLINE, move remained characters to the 
	first MAXLINE bytes and make last byte placed in buffer + MAXLINE, then fill buffer with
	maximum capacity 
\*																								*/
void fillbuf()
{

}