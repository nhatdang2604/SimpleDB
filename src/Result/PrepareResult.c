#include "PrepareResult.h"
#include "string.h"
#include "cstring"

PrepareResult prepareInsert(InputBuffer* pInputBuffer, Statement* pStatement) {
    pStatement->type = STATEMENT_INSERT;
    char* strKeyword = strtok(pInputBuffer->aBuffer, " ");
    char* strId = strtok(NULL, " ");
    char* strUsername = strtok(NULL, " ");
    char* strEmail = strtok(NULL, " ");

    if (NULL == strId || NULL == strUsername || NULL == strEmail) {
        return PREPARE_SYNTAX_ERROR;
    }

    int id = atoi(strId);
    if (strlen(strUsername) > COLUMN_USERNAME_SIZE) {
        return PREPARE_STRING_TOO_LONG;
    }

    if (strlen(strEmail) > COLUMN_EMAIL_SIZE) {
        return PREPARE_STRING_TOO_LONG;
    }
    
    pStatement->rowToInsert.id = id;
    pStatement->rowToInsert.strUsername = strUsername;
    pStatement->rowToInsert.strEmail = strEmail;

    return PREPARE_SUCCESS;
}

PrepareResult prepareStatement(InputBuffer* pInputBuffer, Statement* pStatement) {
    if (0 == strncmp(pInputBuffer->aBuffer, "insert", 6)) {
        return prepareInsert(pInputBuffer, pStatement)
    }

    if (0 == strcmp(pInputBuffer->aBuffer, "select")) {
        pStatement->type = STATEMENT_SELECT;
        return PREPARE_SUCCESS;
    }    

    return PREPARE_UNREGCONIZED_STATEMENT;
}