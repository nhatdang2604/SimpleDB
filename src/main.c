#include <stdbool.h>
#include <string.h>
#include "Result/MetaCommandResult.h";
#include "Result/Statement.h"
#include "Result/PrepareResult.h"
#include "InputBuffer/InputBuffer.h";

int main(int argc, char* argv[]){
    InputBuffer* pInputBuffer = newInputBuffer();
    while(true) {
        printPrompt();
        readInput(pInputBuffer);

        if ('.' == pInputBuffer->aBuffer[0]) {
            switch(doMetaCommand(pInputBuffer)) {
                case (META_COMMAND_SUCCESS): {
                    continue;
                }
                case (META_COMMAND_UNREGCONIZED_COMMAND): {
                    printf("Unregconized command '%s'\n", pInputBuffer->aBuffer);
                    continue;
                }
            }
        }

        Statement statement;
        switch(prepareStatement(pInputBuffer, &statement) {
            case (PREPARE_SUCCESS): {
                break;
            }

            case (PREPARE_UNREGCONIZED_STATEMENT): {
                printf("Unreconized keyword at start of '%s'\n", pInputBuffer->aBuffer);
            }
        })

        executeStatement(&statement);
        printf("Executed");
    }
}