#include "Table.h"

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