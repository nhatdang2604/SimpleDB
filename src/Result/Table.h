#ifndef __TABLE__
#define __TABLE__

#include "stdint.h"
#include "Row.h"

#define TABLE_MAX_PAGES 100
const uint32_t PAGE_SIZE = 4096;
const uint32_t ROWS_PER_PAGE = PAGE_SIZE / ROW_SIZE;
const uint32_t TABLE_MAX_ROWS = ROW_PER_PAGE * TABLE_MAX_PAGES;


typedef struct {
    uint32_t nNumRows;
    void* aPages[TABLE_MAX_PAGES];
} Table;

void* rowSlot(Table* pTable, uint32_t nRowNum);

#endif