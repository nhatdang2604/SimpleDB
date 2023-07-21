#include "Cursor.h"
#include "Row.h"
#include <stdlib.h>

Cursor* tableStart(Table* pTable) {
    Cursor* pCursor = malloc(sizeof(Cursor));
    pCursor->pTable = pTable;
    pCursor->nRowNum = 0;
    pCursor->bEndOfTable = (0 == pTable->nNumRows);

    return pCursor;
}

Cursor* tableEnd(Table* pTable) {
    Cursor* pCursor = malloc(sizeof(Cursor));
    pCursor->pTable = pTable;
    pCursor->nRowNum = pTable->nNumRows;
    pCursor->bEndOfTable = true;

    return pCursor;
}

void cursorAdvance(Cursor* pCursor) {
    ++pCursor->nRowNum;
    Table* pTable = pCursor->pTable;
    if (pCursor->nRowNum == pTable->nNumRows) {
        pCursor->bEndOfTable = true;
    }
}

void* cursorValue(Cursor* pCursor) {
    uint32_t nRowNum = pCursor->nRowNum;
    uint32_t nPageNum = nRowNum / ROWS_PER_PAGE;
    void* pPage = getPage(pCursor->pTable->pPager, nPageNum);
    uint32_t nRowOffset = nRowNum % ROWS_PER_PAGE;
    uint32_t nByteOffset = nRowOffset * ROW_SIZE;
    return pPage + nByteOffset;
}