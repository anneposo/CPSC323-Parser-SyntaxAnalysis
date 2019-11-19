#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h>
#include "parser.h"
#include "lexer.h"

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

bool whiteSpace(char ch) {
	int match;
  switch (ch) {
    case ' ': case '\t': case '\n': match = 1; break;
    default: match = 0; break;
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
	nextChar = buffer[buf_i++];
	if(whiteSpace(nextChar)) { // compiler ignores whitespaces between lexemes
		while((whiteSpace(nextChar))) {
			nextChar = buffer[buf_i++];
		}
	}
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
		}
	}
	else {
		printf("Syntax Error. Expected token in First A.\n");
	}
}

//S -> A
void S() {
	if(isalpha(nextChar) > 0) {
		nextChar = lexer_main(nextChar);
		printf("S -> A\n");
		match(T_ID);
		A();
	}
	else {
		printf("Syntax Error. Expected token in First S.\n");
	}
}

// Mid -> , id Mid
void Mid() {
	if(nextChar == ',') {
		nextChar = lexer(nextChar);
		printf("Mid -> , id Mid\n");
		match(T_COMMA);
		if (isalpha(nextChar) > 0) {
			nextChar = lexer(nextChar);
			printf("Mid -> , id Mid\n");
			match(T_ID);
			Mid();
		} else { printf("Syntax Error. Mismatch in Rule A.\n"); }
	}
	else { printf("Syntax Error. Expected token in First Mid.\n"); }
}

//Type -> int | float | bool
void Type() {
	if(isalpha(nextChar) > 0) {
		nextChar = lexer(nextChar);
		printf("Type -> int | float | bool\n");
	}
	/* if(isalpha(nextChar) > 0) {
		 	nextChar = lexer_main(nextChar) will increment parser's buffer[i++] for every state transitioned
					and will print whole keyword
		 		if(nextChar == T_KEYWORD) {
					printf("Type -> int | float | bool\n");
		 		}
		}
		else { printf("Syntax Error. Expected token in First Type.\n"); }
	*/
}

void parser() {
	S();
}

// S -> D
// D -> T id Mid


int main (int argc, char *argv[]) {
	char ch;
	int i = 0;

	if(strcmp(argv[1],"-i") == 0) {         // use ./test -i option to
		printf("Enter a string: ");           // parse user input via stdin
		fgets(buffer, sizeof(buffer), stdin);
  	buffer[strcspn(buffer, "\n")] = '\0';
	}
	else {
		openFiles(argv[1]);                   // else input source code filename
		while ((ch = fgetc(fp)) != EOF) {     // to parse as command line argv[1] 
				buffer[i++] = ch;
	  }
		printf("buffer: %s", buffer);
	}

	nextChar = buffer[0];
	printf("nextChar = %c\n", nextChar);
	parser();



	if (nextChar == '\0') {
		printf("\nParsing completed successfully.\n");
	}

	closeFiles();
  return 0;
}
