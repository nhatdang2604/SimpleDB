#ifndef __STATEMENT__
#define __STATEMENT__

#include "stdio.h"

typedef enum {
    STATEMENT_INSERT,
    STATEMENT_SELECT,
} StatementType;

typedef struct {
    StatementType type;
} Statement;

void executeStatement(Statement* pStatement);

#endif