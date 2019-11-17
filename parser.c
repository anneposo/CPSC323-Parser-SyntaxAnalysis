#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

void getFile();
void openInputFile();
void openOutputFile();
void closeFiles();

char sourceCodeFile[BUFSIZE]; //holds filename of source code to parse
FILE* fp; // file pointer to input source code file
FILE* outputPtr; // file pointer to output text file

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
};

void getFile(void) {		// getFile gets filename of source code to parse
	printf("Enter source code filename: ");
	fgets(sourceCodeFile, sizeof(sourceCodeFile), stdin);
  sourceCodeFile[strcspn(sourceCodeFile, "\n")] = '\0'; // removes newline from user input
}

void openInputFile(void) {
	getFile();
	fp = fopen(sourceCodeFile, "r");
	if (fp == NULL) {
		printf("Could not open file.\n");
		exit(1);
	} else { printf("Opened %s successfully.\n\n", sourceCodeFile); }
}

void openOutputFile(void) {
  outputPtr = fopen("output.txt", "w"); // open output file with write permissions
}

void closeFiles(void) { // closes both input and output files
	fclose(fp);
	fclose(outputPtr);
}

bool isTerminal(char ch) {
	const char* terminals[] = { "if", "else", "then", "endif", "while", "whileend", "do", "doend", "for",
                              "forend", "input", "output", "and", "or" , "function", ";" };
	int match = 0;

	for (int i = 0; i < 13; i++) {
		if (strchr(terminals[i], ch)) {
			match = 1;
			break;
		}
	}
	return match;
}

bool isNonTerminal(char ch) {

}


int main () {


  return 0;
}
