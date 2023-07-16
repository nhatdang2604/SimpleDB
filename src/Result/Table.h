#ifndef __TABLE__
#define __TABLE__

#include "stdint.h"

#define TABLE_MAX_PAGES 100
#define PAGE_SIZE_VALUE 4096
#define ROWS_PER_PAGE_VALUE (PAGE_SIZE_VALUE / ROW_SIZE_VALUE)

extern const uint32_t PAGE_SIZE;
extern const uint32_t ROWS_PER_PAGE;
extern const uint32_t TABLE_MAX_ROWS;

typedef struct {
    uint32_t nNumRows;
    void* aPages[TABLE_MAX_PAGES];
} Table;

void* rowSlot(Table* pTable, uint32_t nRowNum);
Table* newTable();
void freeTable(Table* pTable);
#endif