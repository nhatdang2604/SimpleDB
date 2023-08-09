#ifndef __TABLE__
#define __TABLE__

#include "stdint.h"
#include "Pager.h"

typedef struct {
    uint32_t nRootPageNum;
    Pager* pPager;
} Table;

Table* dbOpen(const char* strFilename);
void dbClose(Table* pTable);
#endif