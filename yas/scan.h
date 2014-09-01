#ifndef SCAN_H_
#define SCAN_H_

enum token{
	TOK_DIRECTIVE,
	TOK_MNEMONIC,
	TOK_COMMA,
	TOK_LPAREN,
	TOK_RPAREN,
	TOK_DOLLAR,
	TOK_ADD,
	TOK_MINUS,
	TOK_MUL,
	TOK_DIV,
	TOK_MOD,
	TOK_AT,
	TOK_COLON,
	TOK_INTEGER,
	TOK_FLOAT,
	TOK_STRING,
	TOK_NEWLINE,
	TOK_EOF
};

extern enum token tok;
extern unsigned type;
extern unsigned size;

enum oprand_size{
	BYTE,
	WORD,
	DWORD,
	QWORD,
	DQWORD,
	AUTO
};

enum numtype{
	INTEGER,
	DOUBLE
};

extern enum token gettok();

#endif
