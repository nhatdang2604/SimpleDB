#include "Cursor.h"
#include "Row.h"
#include "Tree/Node.h"
#include <stdlib.h>

Cursor* tableStart(Table* pTable) {
    Cursor* pCursor = malloc(sizeof(Cursor));
    pCursor->pTable = pTable;
    pCursor->nPageNum = pTable->nRootPageNum;
    pCursor->nCellNum = 0;

    void* pRootNode = getPage(pTable->pPager, pTable->nRootPageNum);
    uint32_t nNumCells = *leafNodeNumCell(pRootNode);
    pCursor->bEndOfTable = (0 == nNumCells);

    return pCursor;
}

Cursor* tableEnd(Table* pTable) {
    Cursor* pCursor = malloc(sizeof(Cursor));
    pCursor->pTable = pTable;
    
    pCursor->nPageNum = pTable->nRootPageNum;
    void* pRootNode = getPage(pTable->pPager, pTable->nRootPageNum);
    uint32_t nNumCells = *leafNodeNumCell(pRootNode);
    pCursor->nCellNum = nNumCells;

    pCursor->bEndOfTable = true;

    return pCursor;
}

void cursorAdvance(Cursor* pCursor) {
    uint32_t nPageNum = pCursor->nPageNum;
    void* pNode = getPage(pCursor->pTable->pPager, nPageNum);

    ++pCursor->nCellNum;
    if (pCursor->nCellNum >= (*leafNodeNumCell(pNode))) {
        pCursor->bEndOfTable = true;
    }
}

void* cursorValue(Cursor* pCursor) {
    uint32_t nPageNum = pCursor->nPageNum;
    void* pPage = getPage(pCursor->pTable->pPager, nPageNum);
    return leafNodeValue(pPage, pCursor->nCellNum);
}