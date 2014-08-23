#include "scan.h"
#include "input.h"

#include <stdlib.h>


enum token tok;
unsigned type;	/* specify the instruction or prefix used */
unsigned size;	/* specify oprand size */

/* lex value */

enum numtype type;
long l;
double d;
char *s;


#define MAXTOKEN 32

static enum token get_directive()
{
}

/* return an instruction, identifier or register*/
static enum token get_name()
{

}

static enum token get_hex()
{
	pc += 2; /* skip "0x" */
	unsigned long ret = 0;

	while (1){
		if (ascii_map[*pc] & DIGIT){
			ret *= 16;
			ret += *pc - '0';
		}
		else if (ascii_map[*pc] & HEX){
			ret *= 16;
			ret += *pc - ((*pc & 32) ? 'a' : 'A' ) + 10;
		}
		else
			break;
		pc++;
	}
	l = ret;
}

static enum token get_dec()
{
	int fflag = 0;
	char buff[80];
	char *p = buff;
	while (ascii_map[*pc] & DIGIT){
		*p++ = *pc++;
	}
	if (*pc == '.'){
		*p++ = *pc++;
		fflag = 1;
	}
	if (*pc == 'e' || *pc == 'E'){
		*p++ = *pc++;
		while (ascii_map[*pc] & DIGIT){
			*p++ = *pc++;
		}
	}
	*p = '\0';
	if (fflag){
		d = strtod(buff, NULL);
		return TOK_FLOAT;
	}
	else{
		l = atol(buff);
		return TOK_INTEGER;
	}
}

static enum token get_oct()
{
	pc++; /* skip 0 */
	unsigned long ret = 0;

	while (ascii_map[*pc]){
		ret *= 8;
		ret += *pc - '0';
		pc++;
	}

	l = ret;
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
		if (rpc[1] & DIGIT){
			return get_dec();
		}
		return get_directive();
	}
	case ':':{
		return TOK_COLON;
	}
	case ',':{
		return TOK_COMMA;
	}
	case '$':{
		return TOK_DOLLAR;
	}
	case '(':{
		return TOK_LPAREN;
	}
	case ')':{
		return TOK_RPAREN;
	}
	case '@':{
		return TOK_AT;
	}
	case '+':{
		return TOK_ADD;
	}
	case '-':{
		return TOK_MINUS;
	}
	case '*':{
		return TOK_MUL;
	}
	case '/':{
		return TOK_DIV;
	}
	case '%':{
		return TOK_MOD;
	}
	case 'a': case 'b': case 'c': case 'd': case 'e':
	case 'f': case 'g': case 'h': case 'i': case 'j':
	case 'k': case 'l': case 'm': case 'n': case 'o':
	case 'p': case 'q': case 'r': case 's': case 't':
	case 'u': case 'v': case 'w': case 'x': case 'y':
	case 'z':
	case 'A': case 'B': case 'C': case 'D': case 'E':
	case 'F': case 'G': case 'H': case 'I': case 'J':
	case 'K': case 'L': case 'M': case 'N': case 'O':
	case 'P': case 'Q': case 'R': case 'S': case 'T':
	case 'U': case 'V': case 'W': case 'X': case 'Y':
	case 'Z':{
		return get_name();
	}
	case '0':{
		if (rpc[1] == 'x' || rpc[1] == 'X'){
			pc = rpc;
			return get_hex();
		}
		else if (ascii_map[rpc[1]] & DIGIT){
			return get_oct();
		}
		else{
			pc = rpc;
			return get_dec();
		}
	}
	case '1': case '2': case '3': 
	case '4': case '5': case '6':
	case '7': case '8': case '9':{
		return get_dec();
	}
	}
}