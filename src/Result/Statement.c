#include "Statement.h";
#include "stdio.h";

void executeStatement(Statement* pStatement) {
    switch(pStatement->type) {
        case (STATEMENT_INSERT): {
            printf("This is where we would do an insert.\n");
            break;
        }

        case (STATEMENT_SELECT): {
            printf("This is where we sould do a select.\n");
            break;
        }
    }
}
