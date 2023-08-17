#ifndef __PAGER__
#define __PAGER__

#include "stdint.h"
#define TABLE_MAX_PAGES 100
#define PAGE_SIZE_VALUE 4096
#define INVALID_PAGE_NUM UINT32_MAX

extern const uint32_t PAGE_SIZE;

typedef struct {
    int nFileDescriptor;
    uint32_t nFileLength;
    uint32_t nNumPages;
    void* aPages[TABLE_MAX_PAGES];
} Pager;

Pager* pagerOpen(const char* strFilename);
void* getPage(Pager* pPager, uint32_t nPageNum);
void pagerFlush(Pager* pPager, uint32_t nPageNum);

/**
 * Until we start recycling free pages, new pages will always
 * go onto the end of database file
 **/
uint32_t getUnusedPageNum(Pager* pPager);

#endif