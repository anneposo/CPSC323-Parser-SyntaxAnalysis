#include <stdio.h>
#include <stdlib.h>

void openFiles(char*);
void closeFiles();

FILE *fp; // file pointer to input source code file
FILE *outputPtr; // file pointer to output text file
