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

static int cmap[] = {
	EOB, /* 0 NUL */
	OTHER, /* 1 SOH */
	OTHER, /* 2 STX */
	OTHER, /* 3 ETX */
	OTHER, /* 4 EOT */
	OTHER, /* 5 ENQ */
	OTHER, /* 6 ACK */
	OTHER, /* 7 BEL */
	OTHER, /* 8 BS */
	BLANK, /* 9 HT */
	BLANK, /* 10 LF */
	BLANK, /* 11 VT */
	NEWLINE, /* 12 FF */
	BLANK, /* 13 CR */
	OTHER, /* 14 SO */
	OTHER, /* 15 SI */
	OTHER, /* 16 DLE */
	OTHER, /* 17 DC1 */
	OTHER, /* 18 DC2 */
	OTHER, /* 19 DC3 */
	OTHER, /* 20 DC4 */
	OTHER, /* 21 NAK */
	OTHER, /* 22 SYN */
	OTHER, /* 23 ETB */
	OTHER, /* 24 CAN */
	OTHER, /* 25 EM */
	OTHER, /* 26 SUB */
	OTHER, /* 27 ESC */
	OTHER, /* 28 FS */
	OTHER, /* 29 GS */
	OTHER, /* 30 RS */
	OTHER, /* 31 US */
	BLANK, /* 32 SP */
	SIGN, /* 33 ! */
	SIGN, /* 34 " */
	SIGN, /* 35 # */
	SIGN, /* 36 $ */
	SIGN, /* 37 % */
	SIGN, /* 38 & */
	SIGN, /* 39 ' */
	SIGN, /* 40 ( */
	SIGN, /* 41 ) */
	SIGN, /* 42 * */
	SIGN, /* 43 + */
	SIGN, /* 44 , */
	SIGN, /* 45 - */
	SIGN, /* 46 . */
	SIGN, /* 47 / */
	DIGIT | OCT,  /* 48 0 */
	DIGIT | OCT | HEX,  /* 49 1 */
	DIGIT | OCT | HEX,  /* 50 2 */
	DIGIT | OCT | HEX,  /* 51 3 */
	DIGIT | OCT | HEX,  /* 52 4 */
	DIGIT | OCT | HEX,  /* 53 5 */
	DIGIT | OCT | HEX,  /* 54 6 */
	DIGIT | OCT | HEX,  /* 55 7 */
	DIGIT | HEX,  /* 56 8 */
	DIGIT | HEX,  /* 57 9 */
	SIGN,   /* 58 : */
	SIGN,   /* 59 ; */
	SIGN,   /* 60 < */
	SIGN,   /* 61 = */
	SIGN,   /* 62 > */
	SIGN,   /* 63 ? */
	SIGN,   /* 64 @ */
	LETTER | HEX, /* 65 A */
	LETTER | HEX, /* 66 B */
	LETTER | HEX, /* 67 C */
	LETTER | HEX, /* 68 D */
	LETTER | HEX, /* 69 E */
	LETTER | HEX, /* 70 F */
	LETTER, /* 71 G */
	LETTER, /* 72 H */
	LETTER, /* 73 I */
	LETTER, /* 74 J */
	LETTER, /* 75 K */
	LETTER, /* 76 L */
	LETTER, /* 77 M */
	LETTER, /* 78 N */
	LETTER, /* 79 O */
	LETTER, /* 80 P */
	LETTER, /* 81 Q */
	LETTER, /* 82 R */
	LETTER, /* 83 S */
	LETTER, /* 84 T */
	LETTER, /* 85 U */
	LETTER, /* 86 V */
	LETTER, /* 87 W */
	LETTER, /* 88 X */
	LETTER, /* 89 Y */
	LETTER, /* 90 Z */
	SIGN,   /* 91 [ */
	SIGN,   /* 92 \ */
	SIGN,   /* 93 ] */
	SIGN,   /* 94 ^ */
	LETTER, /* 95 _ */
	SIGN,   /* 96 ` */
	LETTER | HEX, /* 97 a */
	LETTER | HEX, /* 98 b */
	LETTER | HEX, /* 99 c */
	LETTER | HEX, /* 100 d */
	LETTER | HEX, /* 101 e */
	LETTER | HEX, /* 102 f */
	LETTER, /* 103 g */
	LETTER, /* 104 h */
	LETTER, /* 105 i */
	LETTER, /* 106 j */
	LETTER, /* 107 k */
	LETTER, /* 108 l */
	LETTER, /* 109 m */
	LETTER, /* 110 n */
	LETTER, /* 111 o */
	LETTER, /* 112 p */
	LETTER, /* 113 q */
	LETTER, /* 114 r */
	LETTER, /* 115 s */
	LETTER, /* 116 t */
	LETTER, /* 117 u */
	LETTER, /* 118 v */
	LETTER, /* 119 w */
	LETTER, /* 120 x */
	LETTER, /* 121 y */
	LETTER, /* 122 z */
	SIGN,   /* 123 { */
	SIGN,   /* 124 | */
	SIGN,   /* 125 } */
	SIGN,   /* 126 ~ */
	OTHER  /* 127 DEL */
};


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
	ATTENTION: total of remaining characters is checked by CALLER
\*																								*/
void fillbuf()
{
	/* move remaining characters to the front */
	char *p = buffer + MAXLINE + 1 - (limit - pc);
	int cnt = 0;
	while (pc != limit){
		*p++ = *pc++;
	}

	/* refill buffer */
	cnt = fread(p, sizeof(char), BUFFER_SIZE, src);
	limit = p + cnt;
	*limit = CH_EOB;
}

/*																								*\
	called when *pc is '\n', newline will skip spaces and '\n' until pc is a non-blank character 
\*																								*/
void newline()
{
	pc++;
	while (1){
		if (cmap[*pc] & BLANK){
			pc++;
		}
		else if (*pc == '\n'){
			newline();
		}
		else
			break;
	}
	lineno++;
}