#include "MetaCommandResult.h"
#include "string.h"

MetaCommandResult doMetaCommand(InputBuffer* pInputBuffer, Table* pTable) {
    if (0 == strcmp(pInputBuffer->aBuffer, ".exit")) {
        dbClose(pTable);
        closeInputBuffer(pInputBuffer);
        exit(EXIT_SUCCESS);
    } else {
        return META_COMMAND_UNREGCONIZED_COMMAND;
    }
} 