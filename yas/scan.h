#ifndef SCAN_H_
#define SCAN_H_

enum token{
	TOK_DIRECTIVE,
	TOK_MNEMONIC,
	TOK_COMMA,
	TOK_LPAREN,
	TOK_RPAREN,
	TOK_PAREN,
	TOK_DOLLAR,
	TOK_AT,
	TOK_COLON,
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

extern enum token gettok();

#endif
