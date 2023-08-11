#include "Table.h"
#include "Row.h"
#include "Tree/Node.h"
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>

Table* dbOpen(const char* strFilename) {
    Pager* pPager = pagerOpen(strFilename);
    Table* pTable = malloc(sizeof(Table));
    pTable->pPager = pPager;
    pTable->nRootPageNum = 0;

    if (0 == pPager->nNumPages) {
        
        //New database file. Initialize page 0 as leaf node
        void* pRootNode = getPage(pPager, 0);
        initializeLeafNode(pRootNode);
        setNodeRoot(pRootNode, true);
    }

    return pTable;
}

void dbClose(Table* pTable) {
    Pager* pPager = pTable->pPager;
   
    for (uint32_t i = 0; i < pPager->nNumPages; ++i) {
        if (NULL == pPager->aPages[i]) {
            continue;
        }

        pagerFlush(pPager, i);
        free(pPager->aPages[i]);
        pPager->aPages[i] = NULL;
    }

    int result = close(pPager->nFileDescriptor);
    if (-1 == result) {
        printf("Error on closing db file.\n");
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