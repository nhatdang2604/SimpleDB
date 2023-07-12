#ifndef __STATEMENT__
#define __STATEMENT__

#include "stdio.h"
#include "Row.h"

typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT,
} StatementType;

typedef struct {
    StatementType type;
    Row rowToInsert;
} Statement;

void executeStatement(Statement* pStatement);

#endif