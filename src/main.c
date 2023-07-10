#include <stdbool.h>
#include <string.h>
#include "InputBuffer/InputBuffer.h";

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