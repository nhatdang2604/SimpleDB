#include "Statement.h"
#include "Cursor.h"
#include "Tree/Node.h"
#include <stdlib.h>

ExecuteResult executeInsert(Statement* pStatement, Table* pTable) {
    void* pNode = getPage(pTable->pPager, pTable->nRootPageNum);
    if ((*leafNodeNumCell(pNode)) >= LEAF_NODE_MAX_CELLS ) {
        return EXECUTE_TABLE_FULL;
    }

    Row* pRowToInsert = &(pStatement->rowToInsert);
    Cursor* pCursor = tableEnd(pTable);
    leafNodeInsert(pCursor, pRowToInsert->id, pRowToInsert);

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
