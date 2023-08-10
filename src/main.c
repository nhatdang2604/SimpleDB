#include <stdbool.h>
#include <string.h>
#include "Result/MetaCommandResult.h"
#include "Result/Statement.h"
#include "Result/PrepareResult.h"
#include "Result/Table.h"
#include "Result/ExecuteResult.h"

#include "InputBuffer/InputBuffer.h"


int main(int argc, char* argv[]){
    if (argc < 2) {
        printf("Must supply a database filename.\n");
        exit(EXIT_FAILURE);
    }

    char* filename = argv[1];
    Table* pTable = dbOpen(filename);

    InputBuffer* pInputBuffer = newInputBuffer();
    while(true) {
        printPrompt();
        readInput(pInputBuffer);

        if ('.' == pInputBuffer->aBuffer[0]) {
            switch(doMetaCommand(pInputBuffer, pTable)) {
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
        switch(prepareStatement(pInputBuffer, &statement)) {
            case (PREPARE_SUCCESS): {
                break;
            }

            case (PREPARE_SYNTAX_ERROR): {
                printf("Syntax error, Could not parse statement.\n");
                continue;
            }

            case (PREPARE_UNREGCONIZED_STATEMENT): {
                printf("Unregconized keyword at start of '%s'\n", pInputBuffer->aBuffer);
                continue;
            }

            case (PREPARE_STRING_TOO_LONG): {
                printf("String is too long\n");
                continue;
            }

            case (PREPARE_NEGATIVE_ID): {
                printf("ID must be positive.\n");
                continue;
            }
        }

        switch (executeStatement(&statement, pTable)) {
            case (EXECUTE_SUCCESS): {
                printf("Executed.\n");
                break;
            }

            case (EXECUTE_TABLE_FULL): {
                printf("Error: Table full.\n");
                break;
            }

            case (EXECUTE_DUPLICATE_KEY): {
                printf("Error: Duplicate key.\n");
                break;
            }
        }
    }
}