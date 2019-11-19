#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 100

enum Symbols {
	NT_S,       // Statement
	NT_E,       // Expression
	NT_A,       // Assignment
	NT_T,       // Term
	NT_F,       // Factor
	T_PLUS,     // +
	T_MINUS,    // -
	T_MULTI,    // *
	T_DIV,      // /
	T_L_PARENS, // (
	T_R_PARENS, // )
	T_ID,       // identifier
	T_NUM,      // integer number
	T_EMPTY,    // empty (epsilon)
	T_COMMA,    // ,
	T_KEYWORD,  // int, float, bool, char
};

enum Symbols lexer(char);
void openFile(char*);
void closeFile();
bool whiteSpace(char);
void match(char ch);
void next();
void F();
void TP();
void T();
void EP();
void E();
void A();
void S();
void parser();

extern enum Symbols lexer_main(char);

FILE *fp; // file pointer to input source code file
FILE *outputPtr; // file pointer to output text file

char nextChar;
char buffer[BUFSIZE];
int buf_i = 0;
