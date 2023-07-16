#ifndef __STATEMENT__
#define __STATEMENT__

#include "stdio.h"
#include "Row.h"
#include "Table.h"
#include "ExecuteResult.h"

typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT,
} StatementType;

typedef struct {
    StatementType type;
    Row rowToInsert;
} Statement;

ExecuteResult executeInsert(Statement* pStatement, Table* pTable);
ExecuteResult executeSelect(Statement* pStatement, Table* pTable);
ExecuteResult executeStatement(Statement* pStatement, Table* pTable);

#endif