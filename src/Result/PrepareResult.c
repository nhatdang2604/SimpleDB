#include "PrepareResult.h";
#include "../InputBuffer/InputBuffer.h";
#include "Statement.h";
#include "MetaCommandResult.h"
#include "string.h";

PrepareResult prepareStatement(InputBuffer* pInputBuffer, Statement* pStatement) {
    if (0 == strcmp(pInputBuffer->aBuffer, "insert")) {
        pStatement->type = STATEMENT_INSERT;
        return PREPARE_SUCCESS;
    }

    if (0 == strcmp(pInputBuffer->aBuffer, "select")) {
        pStatement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }    

    return PREPARE_UNREGCONIZED_STATEMENT;
}