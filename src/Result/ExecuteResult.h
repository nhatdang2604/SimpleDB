#ifndef __EXECUTE_RESULT__
#define __EXECUTE_RESULT__

typedef enum {
    EXECUTE_SUCCESS,
    EXECUTE_TABLE_FULL,
    EXECUTE_DUPLICATE_KEY,
} ExecuteResult;

#endif