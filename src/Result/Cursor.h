#ifndef __CURSOR__
#define __CURSOR__

#include "Table.h"
#include <stdbool.h>

typedef struct {
    Table* pTable;
    uint32_t nPageNum;
    uint32_t nCellNum;
    bool bEndOfTable;
} Cursor;

Cursor* tableStart(Table* pTable);
Cursor* tableEnd(Table* pTable);
void cursorAdvance(Cursor* pCursor);
void* cursorValue(Cursor* pCursor);

#endif