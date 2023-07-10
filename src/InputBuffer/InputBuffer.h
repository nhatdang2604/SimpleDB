#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>


typedef struct {
    char* aBuffer;
    size_t nBufferLength;
    ssize_t nInputLength;
} InputBuffer;

InputBuffer* newInputBuffer();
void printPrompt();
void readInput(InputBuffer* pInputBuffer);
void closeInputBuffer(InputBuffer* pInputBuffer);