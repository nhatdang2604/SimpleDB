#ifndef __META_COMMAND_RESULT__
#define __META_COMMAND_RESULT__


#include "../InputBuffer/InputBuffer.h"

typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNREGCONIZED_COMMAND,
} MetaCommandResult;

MetaCommandResult doMetaCommand(InputBuffer* pInputBuffer);

#endif

