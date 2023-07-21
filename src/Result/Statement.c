#include "Statement.h"
#include "Cursor.h"
#include <stdlib.h>

ExecuteResult executeInsert(Statement* pStatement, Table* pTable) {
    if (pTable->nNumRows >= TABLE_MAX_ROWS) {
        return EXECUTE_TABLE_FULL;
    }

    Row* pRowToInsert = &(pStatement->rowToInsert);
    Cursor* pCursor = tableEnd(pTable);
    serializeRow(pRowToInsert, cursorValue(pCursor));
    ++pTable->nNumRows;

    free(pCursor);

    return EXECUTE_SUCCESS;
}

ExecuteResult executeSelect(Statement* pStatement, Table* pTable) {
    Cursor* pCursor = tableStart(pTable);
    Row row;
    while (!(pCursor->bEndOfTable)) {
        deserializeRow(cursorValue(pCursor), &row);
        printRow(&row);
        cursorAdvance(pCursor);
    }
    free(pCursor);
    return EXECUTE_SUCCESS;
}

ExecuteResult executeStatement(Statement* pStatement, Table* pTable) {
    switch(pStatement->type) {
        case (STATEMENT_INSERT): {
            return executeInsert(pStatement, pTable);
        }

        case (STATEMENT_SELECT): {
            return executeSelect(pStatement, pTable);
        }
    }
}
