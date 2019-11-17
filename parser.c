#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "parser.h"

enum NonTerminalSymbols {
	NT_S, // Statement
	NT_E, // Expression
	NT_A, // Assignment
	NT_T, // Term
	NT_F  // Factor
};
enum TerminalSymbols {
	T_PLUS,			// +
	T_MINUS,		// -
	T_MULTI,		// *
	T_DIV,			// /
	T_L_PARENS,	// (
	T_R_PARENS,	// )
	T_ID				// identifier
	T_EMPTY			// empty (epsilon)
};

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

S -> A                    Statement       ->  Assign
A -> id = E;              Assign          ->  id = Expression;
E -> TE'                  Expression      ->  Term ExpressionPrime
E' -> +TE' | -TE' | e     ExpressionPrime ->  + Term ExpressionPrime | - Term ExpressionPrime | empty
T -> FT'                  Term            ->  Factor TermPrime
T' -> *FT' | /FT' | e     TermPrime       ->  * Factor TermPrime | / Factor TermPrime | empty
F -> ( E ) | id           Factor          ->  ( Expression ) | id

********************************************************************************************************/
bool isFollowTP(const char *ch) { // Follow(T') = { +, ), $ }
	const char *followTP[] = { "+", ")", "\0", ";" };
	for (int i = 0; i < 4; i++) {
		if (strcmp(followTP[i], ch) == 0) {
			match = 1;
			break;
		}
	}
	return match;
}

bool isFollowEP(const char *ch) { // Follow(E') = { ), $ }
	const char *followEP[] = { ")", "\0", ";" };
	for (int i = 0; i < 3; i++) {
		if (strcmp(followEP[i], ch) == 0) {
			match = 1;
			break;
		}
	}
	return match;
}

void match(char ch) {
	if (ch == nextChar) { nextChar = buffer[index++]; } // if matches, go to next
	else {
		printf("Error matching rule \n\n");
		exit(2);
	}
}

// F -> ( E ) | id
void F() {
	if (nextChar == '(') {
		match('(');
		//E(); 								// need to implement E procedure
		match(')');
		// print production rule F
	}
	else if(nextChar == T_ID) {
		match(T_ID);
		// print production rule F
	}
	else { printf("Syntax Error. Token in First of F expected."); }
}

// T' -> *FT' | /FT' | e
void TP() { //TPrime
	if (nextChar == '*') {
		match('*');
		F();
		TP();
		// print production rule T'
	}
	else if (nextChar == '/') {
		match('/');
		F();
		TP();
		// print production rule T'
	}
	else if (!(isFollowTP(nextChar))) {
		match(T_EMPTY);
		// print production rule T'
	}
	else { printf("Syntax Error. Token in First of T' expected."); }
}


int main (int argc, char *argv[]) {
	char ch;
	openFiles(argv[1]);

	while ((ch = fgetc(fp)) != EOF) {
    printf("%c", ch);
  }

	closeFiles();
  return 0;
}
