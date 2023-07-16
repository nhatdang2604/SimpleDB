#include "Table.h"
#include "Row.h"
#include <stdlib.h>

const uint32_t PAGE_SIZE = PAGE_SIZE_VALUE;
const uint32_t ROWS_PER_PAGE = ROWS_PER_PAGE_VALUE;
const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE_VALUE  * TABLE_MAX_PAGES;

void* rowSlot(Table* pTable, uint32_t nRowNum) {
    uint32_t nPageNum = nRowNum / ROWS_PER_PAGE;
    void* pPage = pTable->aPages[nPageNum];
    if (NULL == pPage) {

        //Allocate memory only when we try to access page
        pPage = pTable->aPages[nPageNum] = malloc(PAGE_SIZE);
    }

    uint32_t nRowOffset = nRowNum % ROWS_PER_PAGE;
    uint32_t nByteOffset = nRowOffset * ROW_SIZE;

    return pPage + nByteOffset;
}

Table* newTable() {
    Table* pTable = (Table*)malloc(sizeof(Table));
    pTable->nNumRows = 0;
    for (uint32_t i = 0; i < TABLE_MAX_PAGES; ++i) {
        pTable->aPages[i] = NULL;
    }

    return pTable;
}

void freeTable(Table* pTable) {
    if (!pTable) {
        return;
    }

    for (int i = 0; pTable->aPages[i]; ++i) {
        free(pTable->aPages[i]);
    }

    free(pTable);
}