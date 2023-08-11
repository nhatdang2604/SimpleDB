#ifndef __META_COMMAND_RESULT__
#define __META_COMMAND_RESULT__


#include "../InputBuffer/InputBuffer.h"
#include "Table.h"
#include "Pager.h"

typedef enum {
    META_COMMAND_SUCCESS,
    META_COMMAND_UNREGCONIZED_COMMAND,
} MetaCommandResult;

MetaCommandResult doMetaCommand(InputBuffer* pInputBuffer, Table* pTable);
void printConstants();
void indent(uint32_t nLevel);
void printTree(Pager* pPager, uint32_t nPageNum, uint32_t nIndentationLevel);



#endif

