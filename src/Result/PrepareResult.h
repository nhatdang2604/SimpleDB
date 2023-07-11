typedef enum {
    PREPARE_SUCCESS,
    PREPARE_UNREGCONIZED_STATEMENT,
} PrepareResult;

PrepareResult prepareStatement(InputBuffer* pInputBuffer, Statement* pStatement);