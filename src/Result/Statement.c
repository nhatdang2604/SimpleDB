#include "Statement.h"
#include "Cursor.h"
#include "Tree/Node.h"
#include <stdlib.h>

ExecuteResult executeInsert(Statement* pStatement, Table* pTable) {
    void* pNode = getPage(pTable->pPager, pTable->nRootPageNum);

    uint32_t nNumCells = (*leafNodeNumCell(pNode));

    Row* pRowToInsert = &(pStatement->rowToInsert);
    uint32_t nKeyToInsert = pRowToInsert->id;
    Cursor* pCursor = tableFind(pTable, nKeyToInsert);

    if (pCursor->nCellNum < nNumCells) {
        uint32_t nKeyAtIndex = *leafNodeKey(pNode, pCursor->nCellNum);
        if (nKeyAtIndex == nKeyToInsert) {
            return EXECUTE_DUPLICATE_KEY;
        }
    }

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
