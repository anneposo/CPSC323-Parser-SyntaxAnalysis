#include <stdio.h>
#include <stdlib.h>

#define BUFSIZE 100

enum Symbols lexer(char);
void openFiles(char*);
void closeFiles();
bool isFollowTP(char ch);
bool isFollowEP(const char *ch);
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

FILE *fp; // file pointer to input source code file
FILE *outputPtr; // file pointer to output text file

char nextChar;
char buffer[BUFSIZE];

int errorS  = 0;
int errorA  = 0;
int errorE  = 0;
int errorEP = 0;
int errorT  = 0;
int errorTP = 0;
int errorF  = 0;
