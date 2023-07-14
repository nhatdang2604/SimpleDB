#include "Statement.h"
#include "Table.h"
#include "Row.h"
#include "ExecuteResult.h"

ExecuteResult executeInsert(Statement* pStatement, Table* pTable) {
    if (pTable->nNumRows >= TABLE_MAX_ROWS) {
        return EXECUTE_TABLE_FULL;
    }

    Row* pRowToInsert = &(pStatement->rowToInsert);
    serializeRow(pRowToInsert, rowSlot(pTable, pTable->nNumRows);)

    return EXECUTE_SUCCESS;
}

ExecuteResult executeSelect(Statement* pStatement, Table* pTable) {
    Row row;
    for (uint32_t i = 0; i < pTable->nNumRows; ++i) {
        deserializeRow(rowSlot(pTable, i), &row);
        printRow(&row);
    }

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
