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
};

void openFiles(char *fileName) {
	fp = fopen(fileName, "r");
	if (fp == NULL) {
		printf("Could not open file.\n");
		exit(1);
	} else { printf("Opened %s successfully.\n\n", fileName); }

	outputPtr = fopen("output.txt", "w"); // open output file with write permissions
}

void closeFiles(void) { // closes both input and output files
	fclose(fp);
	fclose(outputPtr);
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
