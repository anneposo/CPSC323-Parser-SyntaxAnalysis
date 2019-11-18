#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <ctype.h> // for isalpha() and isdigit()

void openFiles(char *);
void closeFiles();
bool isSeparator(char);
bool isOperator(char);
bool isEmpty(char);
bool isKeyword(const char *);
void lexer_main (char);

//FILE *fp; // file pointer to input source code file
//FILE *outputPtr; // file pointer to output text file
char str[15];

enum FsmState { START, ID_START, IN_ID, ID_END, KEYWORD_END,
								NUM_START, IN_NUM, IN_REAL, INT_END, REAL_END,
								COMMENT_START, IN_COMMENT, END_COMMENT,
							 	OPERATOR, OPERATOR_END, SEPARATOR };

// void openFiles(char *fileName) {
// 	fp = fopen(fileName, "r");
// 	if (fp == NULL) {
// 		printf("Could not open file.\n");
// 		exit(1);
// 	} else { printf("Opened %s successfully.\n\n", fileName); }
//
// 	outputPtr = fopen("output.txt", "w"); // open output file with write permissions
// }
//
// void closeFiles(void) { // closes both input and output files
// 	fclose(fp);
// 	fclose(outputPtr);
// }

bool isOperator(char ch) {
	int match;
  switch (ch) {
    case '=': case '<': case '>': case '+': case '-':
    case '/': case '%': case '*': match = 1; break;
    default: match = 0; break;
  }
	return match;
}

bool isSeparator(char ch) {
	int match;
  switch (ch) {
    case '(': case ')': case '{': case '}': case '[': case ']':
    case ',': case '.': case ':': case ';': case '!': match = 1; break;
    default: match = 0; break;
  }
	return match;
}

bool isKeyword(const char* buf) {
	const char* keywords[] = { "int", "float", "bool", "if", "else", "then", "endif", "while", "whileend",
						  	  					 "do", "doend", "for", "forend", "input", "output", "and", "or" , "function" };
	int match = 0;

	for (int i = 0; i < 18; i++) {
		if (strcmp(keywords[i], buf) == 0) {
			match = 1;
			break;
		}
	}
	return match;
}

bool isEmpty(char ch) {
  int match;
  switch (ch) {
    case ' ': case '\t': case '\n': match = 1; break;
    default: match = 0; break;
  }
}

void lexer_main (char ch) {
  char symBuf;
  char buffer[100];
  int i = 0;
  int str_i = 0;
  enum FsmState currentState = START;

  if (isSeparator(ch) || isOperator(ch)) {
   if (isSeparator(ch)) {
        printf("\nToken: Separator\tLexeme: %c\n", ch);
     } else { printf("\nToken: Operator\tLexeme: %c\n", ch); }
    // switch(ch) {
    //   case '+':
  	// 		printf("\nToken: Operator\tLexeme: %c\n", ch);
  	// 		//return T_PLUS;
  	// 		break;
  	// 	case '-':
  	// 		printf("\nToken: Operator\tLexeme: %c\n", ch);
  	// 		//return T_MINUS;
  	// 		break;
  	// 	case '*':
  	// 		printf("\nToken: Operator\tLexeme: %c\n", ch);
  	// 		//return T_MULTI;
  	// 		break;
  	// 	case '/':
  	// 		printf("\nToken: Operator\tLexeme: %c\n", ch);
  	// 		//return T_DIV;
  	// 		break;
  	// 	case '(':
  	// 		printf("\nToken: Separator\tLexeme: %c\n", ch);
  	// 		//return T_L_PARENS;
  	// 		break;
  	// 	case ')':
  	// 		printf("\nToken: Separator\tLexeme: %c\n", ch);
  	// 		//return T_R_PARENS;
  	// 		break;
  	// 	case '=':
  	// 		printf("\nToken: Operator\tLexeme: %c\n", ch);
  	// 		break;
  	// 	case ';':
  	// 		printf("\nToken: Separator\tLexeme: %c\n", ch);
  	// 		printf("e -> epsilon\n");
  	// 		//return T_EMPTY;
  	// 		break;
    // }
  }
  else {
    while (!(isSeparator(ch)) && !(isOperator(ch)) && !(isEmpty(ch)) && ch != '\0') {
      switch(currentState) {
        case START:   // 0 - initial identifier state
          if (ch == '\r' || ch == ' ' || ch == '\n' || ch == '\t') {
            break;		// skips any white space in beginning of buffer
          } else {
            buffer[i++] = ch; // add current character to buffer
            if(isalpha(ch) > 0) {
              currentState = ID_START; // if alphabetical character, go to state 1 identifier start
            }
            else if (isdigit(ch) > 0) { // if digit character, go to state 5 number start
              currentState = NUM_START;
            }
            else if (ch == '!') { // "!" signifies beginning/end of a comment, go to state 10 comment start
              currentState = COMMENT_START;
            }
          }
          break;
        case ID_START: // 1 - initial identifier state - can only go to 2nd state
  				if (isSeparator(ch) || isOperator(ch)) {
  					symBuf = ch; // for encountering separators/operators at ends of identifiers with no space separator ( i.e. x=a )
  					currentState = ID_END;
  				}
  				else {
  					buffer[i++] = ch;
  					currentState = IN_ID;
  				}
  				break;

  			case IN_ID: // state 2 - can go back to 2 or to state 3 ID end accepting state
  				if (isalpha(ch) > 0 || isdigit(ch) > 0 || ch == '$') {
  					buffer[i++] = ch;		//legal identifiers start with alphabetical character and can include any number or '$' character
  					currentState = IN_ID;
  				}
  			 	else if (isSeparator(ch) || isOperator(ch)) {
  					symBuf = ch; // for encountering separators/operators at ends of identifiers with no space separator ( i.e. function(); )
  					currentState = ID_END;
  				}
  				else { currentState = ID_END; }
  				break;

  			case NUM_START: // 5 - initial number state - can go to 6th state inside number
  				if (isSeparator(ch) || isOperator(ch)) {
  					currentState = INT_END;
  					symBuf = ch;
  				} else {
  					buffer[i++] = ch;
  					currentState = IN_NUM;
  			 	}
  				break;

  			case IN_NUM: // state 6 - can go back to state 6, or state 7 in real number, or state 8 in int end
  				if (isdigit(ch) > 0) {
  					buffer[i++] = ch;
  					currentState = IN_NUM;
  				}
  				else if(ch == '.') {
  			 		buffer[i++] = ch;
  					currentState = IN_REAL;
  				}
  				else if (isSeparator(ch) || isOperator(ch)) {
  					symBuf = ch;
  					currentState = INT_END;
  				}
  				else { currentState = INT_END; }
  				break;

  			case IN_REAL: // state 7 - can go back to state 7 or state 9 real number end
  				if (isdigit(ch) > 0) {
  					buffer[i++] = ch;
  					currentState = IN_REAL;
  				}
  				else { currentState = REAL_END; }
  				break;

  			case COMMENT_START: // state 10 - can only go to state 11 inside comment
  				buffer[i++] = ch;
  				currentState = IN_COMMENT;
  				break;

  			case IN_COMMENT: // state 11 - state continues to loop back to state 11 inside comment until end comment '!' is reached
  				buffer[i++] = ch;
  				if (ch == '!') {
  					currentState = END_COMMENT;
  				}
  				break;
        default:
  				printf("Error: invalid state.\n");
  				break;
      }
      ch = str[++str_i];
      if (isSeparator(ch) || isOperator(ch) || isEmpty(ch) || ch == '\0') {
        if (currentState == IN_ID || currentState == ID_START) {
          currentState = ID_END;
        }
        else if (currentState == NUM_START || currentState == IN_NUM) {
          currentState = INT_END;
        }
      }

      if (currentState == ID_END || currentState == INT_END || currentState == REAL_END || currentState == END_COMMENT) {  // final accepting states
					buffer[i] = '\0';
					if (currentState == ID_END) {
						if(isKeyword(buffer)) { //check if string is a keyword or identifier
							printf("\nToken: Keyword\tLexeme: %s\n", buffer);//printToken(outputPtr, "KEYWORD", buffer);
						} else { printf("\nToken: Identifier\tLexeme: %s\n", buffer); } //printToken(outputPtr, "IDENTIFIER", buffer); } // call printToken to print token and lexeme to output file
					}
					else if (currentState == INT_END) {
            printf("\nToken: Integer\tLexeme: %s\n", buffer);
						//printToken(outputPtr, "INTEGER", buffer);
					}
					else if (currentState == REAL_END) {
            printf("\nToken: Real Number\tLexeme: %s\n", buffer);
						//printToken(outputPtr, "REAL NUMBER", buffer);
					}
					else if (currentState == END_COMMENT) {
            printf("\nToken: Comment\tLexeme: %s\n", buffer);
						//printToken(outputPtr, "COMMENT", buffer);
          }
      }

      if (symBuf != 0) { // for cases where separator/operator is next to identifier/number
  			if (isSeparator(ch)) {
          printf("\nToken: Separator\tLexeme: %c\n", symBuf);
          //printCharToken(outputPtr, "SEPARATOR", symBuf);
  			}
  			else if (isOperator(ch)) {  printf("\nToken: Operator\tLexeme: %c\n", symBuf); } //printCharToken(outputPtr, "OPERATOR", symBuf); }
  			symBuf = 0;
  		}
    } // while bracket close
  } // else bracket close
} // function bracket close

// int main () {
//   char ch;
//   printf("Enter a string: ");
//   scanf("%s", str);
//
//   ch = str[0];
//   //printf("ch = %c", ch);
//   lexer_main(ch);
//
//   return 0;
// }
