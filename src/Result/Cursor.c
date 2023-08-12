#include "Cursor.h"
#include "Row.h"
#include "Tree/Node.h"
#include <stdlib.h>
#include <stdio.h>

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

Cursor* tableFind(Table* pTable, uint32_t nKey) {    
    uint32_t nRootPageNum = pTable->nRootPageNum;
    void* pRootNode = getPage(pTable->pPager, nRootPageNum);

    if (NODE_LEAF == getNodeType(pRootNode)) {
        return leafNodeFind(pTable, nRootPageNum, nKey);
    } 

    return internalNodeFind(pTable, nRootPageNum, nKey);
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

Cursor* leafNodeFind(Table* pTable, uint32_t nPageNum, uint32_t nKey) {
    void* pNode = getPage(pTable->pPager, nPageNum);
    uint32_t nNumCells = *leafNodeNumCell(pNode);

    Cursor* pCursor = malloc(sizeof(Cursor));
    pCursor->pTable = pTable;
    pCursor->nPageNum = nPageNum;

    //Binary search
    uint32_t nMinIndex = 0;
    uint32_t nOnePastMaxIndex = nNumCells;
    while(nOnePastMaxIndex != nMinIndex) {
        uint32_t nIndex = (nMinIndex + nOnePastMaxIndex)/2;
        uint32_t nKeyAtIndex = *leafNodeKey(pNode, nIndex);

        if (nKey == nKeyAtIndex) {
            pCursor->nCellNum = nIndex;
            return pCursor;
        }

        if (nKey < nKeyAtIndex) {
            nOnePastMaxIndex = nIndex;
        } else {
            nMinIndex = nIndex + 1;
        }
    }

    pCursor->nCellNum = nMinIndex;
    return pCursor;
}

Cursor* internalNodeFind(Table* pTable, uint32_t nPageNum, uint32_t nKey) {
    void* pNode = getPage(pTable->pPager, nPageNum);
    uint32_t nNumKeys = *internalNodeNumKeys(pNode);

    //Binary search to find index of child to search
    uint32_t nMinIndex = 0;
    uint32_t nMaxIndex = nNumKeys;

    while (nMinIndex != nMaxIndex) {
        uint32_t nIndex = (nMinIndex + nMaxIndex)/2;
        uint32_t nKeyToRight = *internalNodeKey(pNode, nIndex);
        
        if (nKeyToRight >= nKey) {
            nMaxIndex = nIndex;
        } else {
            nMinIndex = nIndex + 1;
        }
    }

    uint32_t nChildNum = *internalNodeChild(pNode, nMinIndex);
    void* pChild = getPage(pTable->pPager, nChildNum);
    switch(getNodeType(pChild)) {
        case NODE_LEAF: {
            return leafNodeFind(pTable, nChildNum, nKey);
        }

        case NODE_INTERNAL: {
            return internalNodeFind(pTable, nChildNum, nKey);
        }
    }

}