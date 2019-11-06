#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

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
