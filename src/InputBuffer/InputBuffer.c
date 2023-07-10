#include "InputBuffer.h";

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