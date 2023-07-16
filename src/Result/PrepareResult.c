#include "PrepareResult.h"
#include "string.h"

PrepareResult prepareStatement(InputBuffer* pInputBuffer, Statement* pStatement) {
    if (0 == strncmp(pInputBuffer->aBuffer, "insert", 6)) {
        pStatement->type = STATEMENT_INSERT;
        int nArgAssigneCount = sscanf(
            pInputBuffer->aBuffer,
            "insert %d %s %s",
            &(pStatement->rowToInsert.id),
            pStatement->rowToInsert.strUsername,
            pStatement->rowToInsert.strEmail
        );

        if (nArgAssigneCount < 3) {
            return PREPARE_SYNTAX_ERROR;
        }

        return PREPARE_SUCCESS;
    }

    if (0 == strcmp(pInputBuffer->aBuffer, "select")) {
        pStatement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }    

    return PREPARE_UNREGCONIZED_STATEMENT;
}