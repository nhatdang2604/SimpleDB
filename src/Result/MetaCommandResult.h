typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNREGCONIZED_COMMAND,
} MetaCommandResult;

MetaCommandResult doMetaCommand(InputBuffer* pInputBuffer);
