#include "Table.h"
#include "Row.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

const uint32_t PAGE_SIZE = PAGE_SIZE_VALUE;
const uint32_t ROWS_PER_PAGE = ROWS_PER_PAGE_VALUE;
const uint32_t TABLE_MAX_ROWS = ROWS_PER_PAGE_VALUE  * TABLE_MAX_PAGES;

Table* dbOpen(const char* strFilename) {
    Pager* pPager = pagerOpen(strFilename);
    uint32_t nNumRows = pPager->nFileLength / ROW_SIZE;
    Table* pTable = malloc(sizeof(Table));
    pTable->pPager = pPager;
    pTable->nNumRows = nNumRows;

    return pTable;
}

void dbClose(Table* pTable) {
    Pager* pPager = pTable->pPager;
    uint32_t nNumFullPages = pTable->nNumRows / ROWS_PER_PAGE;

    for (uint32_t i = 0; i < nNumFullPages; ++i) {
        if (NULL == pPager->aPages[i]) {
            continue;
        }

        pagerFlush(pPager, i, PAGE_SIZE);
        free(pPager->aPages[i]);
        pPager->aPages[i] = NULL;
    }

    // There may be a partial page to write to the end of the file
    // This should not be needed after we switch to a B-tree
    uint32_t nNumAdditionalRows = pTable->nNumRows % ROWS_PER_PAGE;
    if (nNumAdditionalRows > 0) {
        uint32_t nPageNum = nNumFullPages;
        if (NULL != pPager->aPages[nPageNum]) {
            pagerFlush(pPager, nPageNum, nNumAdditionalRows * ROW_SIZE);
            free(pPager->aPages[nPageNum]);
            pPager->aPages[nPageNum] = NULL;
        }
    }

    int result = close(pPager->nFileDescriptor);
    if (-1 == result) {
        printf("Error on clsoing db file.\n");
        exit(EXIT_FAILURE);
    }

    for (uint32_t i = 0; i < TABLE_MAX_PAGES; ++i) {
        void* pPage = pPager->aPages[i];
        if (pPage) {
            free(pPage);
            pPager->aPages[i] = NULL;
        }
    }

    free(pPager);
    free(pTable);
}

// void freeTable(Table* pTable) {
//     if (!pTable) {
//         return;
//     }

//     for (int i = 0; pTable->aPages[i]; ++i) {
//         free(pTable->aPages[i]);
//     }

//     free(pTable);
// }