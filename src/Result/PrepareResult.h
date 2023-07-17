#ifndef __PREPARE_RESULT__
#define __PREPARE_RESULT__

#include "../InputBuffer/InputBuffer.h"
#include "Statement.h"

typedef enum {
    PREPARE_SUCCESS,
    PREPARE_SYNTAX_ERROR,
    PREPARE_UNREGCONIZED_STATEMENT,
    PREPARE_STRING_TOO_LONG
} PrepareResult;

PrepareResult prepareStatement(InputBuffer* pInputBuffer, Statement* pStatement);

#endif