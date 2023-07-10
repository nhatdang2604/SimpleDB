#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

typedef struct {
    char* aBuffer;
    size_t nBufferLength;
    ssize_t nInputLength;
} InputBuffer;

InputBuffer* newInputBuffer() {
    InputBuffer* pInputBuffer = malloc(sizeof(InputBuffer));
    pInputBuffer->aBuffer = NULL;
    pInputBuffer->nBufferLength = 0;
    pInputBuffer->nInputLength = 0;
    
    return pInputBuffer;
}

void printPrompt() {
    printf("db > ");
}

void readInput(InputBuffer* pInputBuffer) {
    ssize_t nByteRead = getline(&pInputBuffer->aBuffer, &pInputBuffer->nBufferLength, stdin);

    if (nByteRead <= 0) {
        printf("Error while reading input\n");
        exit(EXIT_FAILURE);
    }

    //Ignores trailing new line
    pInputBuffer->nInputLength = nByteRead - 1;
    pInputBuffer->aBuffer[nByteRead - 1] = NULL; 
}

void closeInputBuffer(InputBuffer* pInputBuffer) {
    if (NULL != pInputBuffer) {
        if (NULL != pInputBuffer->aBuffer) {
            free(pInputBuffer->aBuffer);
        }
        free(pInputBuffer);
    }
}

int main(int argc, char* argv[]){
    InputBuffer* pInputBuffer = newInputBuffer();
    while(true) {
        printPrompt();
        readInput(pInputBuffer);

        if (0 == strcmp(pInputBuffer->aBuffer, ".exit")) {
            closeInputBuffer(pInputBuffer);
            exit(EXIT_SUCCESS);
        } else {
            printf("Unregconized command '%s'.\n", pInputBuffer->aBuffer);
        }

    }
}