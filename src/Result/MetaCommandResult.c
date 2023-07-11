#include "MetaCommandResult.h";
#include "../InputBuffer/InputBuffer.h";
#include "string.h";

MetaCommandResult doMetaCommand(InputBuffer* pInputBuffer) {
    if (0 == strcmp(pInputBuffer->aBuffer, ".exit")) {
        closeInputBuffer(pInputBuffer);
        exit(EXIT_SUCCESS);
    } else {
        return META_COMMAND_UNREGCONIZED_COMMAND;
    }
} 