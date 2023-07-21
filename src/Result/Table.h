#ifndef __TABLE__
#define __TABLE__

#include "stdint.h"
#include "Pager.h"

#define PAGE_SIZE_VALUE 4096
#define ROWS_PER_PAGE_VALUE (PAGE_SIZE_VALUE / ROW_SIZE_VALUE)

extern const uint32_t PAGE_SIZE;
extern const uint32_t ROWS_PER_PAGE;
extern const uint32_t TABLE_MAX_ROWS;

typedef struct {
    uint32_t nNumRows;
    Pager* pPager;
} Table;

Table* dbOpen(const char* strFilename);
void dbClose(Table* pTable);
#endif