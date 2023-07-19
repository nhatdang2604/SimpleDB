#ifndef __PAGER__
#define __PAGER__

#include "stdint.h"
#define TABLE_MAX_PAGES 100

extern const uint32_t PAGE_SIZE;

typedef struct {
    int nFileDescriptor;
    uint32_t nFileLength;
    void* aPages[TABLE_MAX_PAGES];
} Pager;

Pager* pagerOpen(const char* strFilename);
void* getPage(Pager* pPager, uint32_t nPageNum);
void pagerFlush(Pager* pPager, uint32_t nPageNum, uint32_t size);

#endif