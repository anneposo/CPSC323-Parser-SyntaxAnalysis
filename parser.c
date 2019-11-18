#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"

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
	T_EMPTY     // empty (epsilon)
};

enum Symbols lexer(char ch) {
	switch(ch) {
		case '+':
			printf("\nToken: Operator\tLexeme: %c\n", ch);
			return T_PLUS;
			break;
		case '-':
			printf("\nToken: Operator\tLexeme: %c\n", ch);
			return T_MINUS;
			break;
		case '*':
			printf("\nToken: Operator\tLexeme: %c\n", ch);
			return T_MULTI;
			break;
		case '/':
			printf("\nToken: Operator\tLexeme: %c\n", ch);
			return T_DIV;
			break;
		case '(':
			printf("\nToken: Separator\tLexeme: %c\n", ch);
			return T_L_PARENS;
			break;
		case ')':
			printf("\nToken: Separator\tLexeme: %c\n", ch);
			return T_R_PARENS;
			break;
		case '=':
			printf("\nToken: Operator\tLexeme: %c\n", ch);
			break;
		case ';':
			printf("\nToken: Separator\tLexeme: %c\n", ch);
			printf("e -> epsilon\n");
			return T_EMPTY;
			break;
		default:
			if (isalpha(ch) > 0) {
				printf("\nToken: Identifier\tLexeme: %c\n", ch);
				return T_ID;
			}
			else if (isdigit(ch) > 0) {
				printf("\nToken: Identifier\tLexeme: %c\n", ch);
				return T_NUM;
			}
			else {
				printf("e -> epsilon\n");
				return T_EMPTY;
			}
			break;
	}
}

void openFiles(char *fileName) {
	fp = fopen(fileName, "r");
	if (fp == NULL) {
		printf("Could not open file.\n");
		exit(1);
	} else { printf("Opened %s successfully.\n\n", fileName); }

	outputPtr = fopen("output.txt", "w"); // open output file with write permissions
}

void closeFiles() { // closes both input and output files
	fclose(fp);
	fclose(outputPtr);
}

/******************************************** GRAMMAR RULES ********************************************

1ST ITERATION:
S -> A                    Statement       ->  Assign
A -> id = E;              Assign          ->  id = Expression;
E -> TE'                  Expression      ->  Term ExpressionPrime
E' -> +TE' | -TE' | e     ExpressionPrime ->  + Term ExpressionPrime | - Term ExpressionPrime | empty
T -> FT'                  Term            ->  Factor TermPrime
T' -> *FT' | /FT' | e     TermPrime       ->  * Factor TermPrime | / Factor TermPrime | empty
F -> ( E ) | id           Factor          ->  ( Expression ) | id

2ND ITERATION:
S -> D                    Statement       ->  Declarative
D -> T id Mid             Declarative     ->  Type id MoreIds
T -> int | float | bool   Type            ->  int | float | bool
Mid -> , id Mid           MoreIds         ->  , id MoreIds

********************************************************************************************************/
bool isFollowTP(char ch) { // Follow(T') = { +, ), $ }
	const char *followTP[] = { "+", ")", "\0", ";" };
	int match = 0;
	for (int i = 0; i < 4; i++) {
		if (strchr(followTP[i], ch) == 0) {
			match = 1;
			break;
		}
	}
	return match;
}

bool isFollowEP(const char *ch) { // Follow(E') = { ), $ }
	const char *followEP[] = { ")", "\0", ";" };
	int match = 0;
	for (int i = 0; i < 3; i++) {
		if (strcmp(followEP[i], ch) == 0) {
			match = 1;
			break;
		}
	}
	return match;
}

void match(char ch) {
	if (ch == nextChar) { next(); } // if matches, go to next
	else {
		printf("Error matching rule \n\n");
		exit(2);
	}
}

void next() {
	static int i = 0;
	nextChar = buffer[i++];
}

// F -> ( E ) | id
void F() {
	if (nextChar == '(') {
		nextChar = lexer(nextChar);
		match('(');
		E(); 								// need to implement E procedure
		match(')');
		printf("F -> ( E ) | id\n");
	}
	else if(isalpha(nextChar) > 0) {
		nextChar = lexer(nextChar);
		match(T_ID);
		printf("F -> ( E ) | id\n");
	}
	else {
		printf("Syntax Error. Expected token in First F.");
		errorF = 1;
	}
}

// T' -> *FT' | /FT' | e
void TP() { //TPrime
	if (nextChar == '*') {
		nextChar = lexer(nextChar);
		printf("T' -> *FT' | /FT' | e\n");
		match(T_MULTI);
		F();
		TP();
	}
	else if (nextChar == '/') {
		nextChar = lexer(nextChar);
		printf("T' -> *FT' | /FT' | e\n");
		match(T_DIV);
		F();
		TP();
	}
	else if(nextChar == ';' || nextChar == '\0') {
		nextChar = lexer(nextChar);
		printf("T' -> *FT' | /FT' | e\n");
		match(T_EMPTY);
	}
	else {
		printf("Syntax Error. Expected token in First T'.\n");
		errorTP = 1;
	}
}

//T -> FT'
void T() {
	if(nextChar == '(') {
		nextChar = lexer(nextChar);
		printf("T -> FT'\n");
		match(T_L_PARENS);
		TP();
	}
	else if(isalpha(nextChar) > 0) {
		nextChar = lexer(nextChar);
		printf("T -> FT'\n");
		match(T_ID);
		TP();
	}
	else {
		printf("Syntax Error. Expected token in First T.\n");
		errorT = 1;
	}
}

//E' -> +TE' | -TE' | e
void EP() {
	if(nextChar == '+') {
		nextChar = lexer(nextChar);
		printf("E' -> +TE' | -TE' | e\n");
		match(T_PLUS);
		T();
		EP();
	}
	else if(nextChar == '-') {
		nextChar = lexer(nextChar);
		printf("E' -> +TE' | -TE' | e\n");
		match(T_MINUS);
		T();
		EP();
	}
	else if(nextChar == ';' || nextChar == '\0') {
		nextChar = lexer(nextChar);
		printf("E' -> +TE' | -TE' | e\n");
		match(T_EMPTY);
	}
	else {
		printf("Syntax Error. Expected token in First E'.\n");
		errorEP = 1;
	}
}

//E -> TE'
void E() {
	if(nextChar == '(') {
		nextChar = lexer(nextChar);
		printf("E -> TE'\n");
		match(T_L_PARENS);
		EP();
	}
	else if(isalpha(nextChar) > 0) {
		nextChar = lexer(nextChar);
		printf("E -> TE'\n");
		match(T_ID);
		EP();
	}
	else {
		printf("Syntax Error. Expected token in First E.\n");
		errorE = 1;
	}
}

//A -> id = E;
void A() {
	if(isalpha(nextChar) > 0) {
		printf("A -> id = E\n");
		next();
		if (nextChar == '=') {
			lexer(nextChar);
			match('=');
			E();
		}
		else {
			printf("Syntax Error. Mismatch in Rule A.\n");
			errorA = 1;
		}
	}
	else {
		printf("Syntax Error. Expected token in First A.\n");
		errorA = 1;
	}
}

//S -> A
void S() {
	if(isalpha(nextChar) > 0) {
		nextChar = lexer(nextChar);
		printf("S -> A\n");
		match(T_ID);
		A();
	}
	else {
		printf("Syntax Error. Expected token in First S.\n");
		errorS = 1;
	}
}

void parser() {
	S();
}

// S -> D
// D -> T id Mid
// T -> int | float | bool
// Mid -> , id Mid

int main (int argc, char *argv[]) {
	char ch;
	printf("Enter a string: ");
	scanf("%s", buffer);
	nextChar = buffer[0];
	//openFiles(argv[1]);

	parser();
	//while ((ch = fgetc(fp)) != EOF) {
  // printf("%c", ch);
  //}


	if (nextChar == '\0') {
		printf("\nParsing completed successfully.\n");
	}

	//closeFiles();
  return 0;
}
