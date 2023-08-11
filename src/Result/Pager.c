#include "Pager.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

const uint32_t PAGE_SIZE = PAGE_SIZE_VALUE;

Pager* pagerOpen(const char* strFilename) {
    int nFileDescriptor = open(
            strFilename, 
            O_RDWR | O_CREAT,
            S_IWUSR | S_IRUSR
        );

    if (-1 == nFileDescriptor) {
        printf("Unable to open file\n");
        exit(EXIT_FAILURE);
    }

    off_t nFileLength = lseek(nFileDescriptor,0, SEEK_END);

    Pager* pPager = malloc(sizeof(Pager));
    pPager->nFileDescriptor = nFileDescriptor;
    pPager->nFileLength = nFileLength;
    pPager->nNumPages = nFileLength / PAGE_SIZE;

    if (0 != (nFileLength % PAGE_SIZE)) {
        printf("DB file is not a whole number of pages. Corrupt file.\n");
        exit(EXIT_FAILURE);
    }

    for (uint32_t i = 0; i < TABLE_MAX_PAGES; ++i) {
        pPager->aPages[i] = NULL;
    }

    return pPager;
}

void* getPage(Pager* pPager, uint32_t nPageNum) {
    if (nPageNum > TABLE_MAX_PAGES) {
        printf("Tried to fetch page number out of bounds. %d > %d\n", nPageNum, TABLE_MAX_PAGES);
        exit(EXIT_FAILURE);
    }

    if (NULL == pPager->aPages[nPageNum]) {

        //Cache miss => Allocate memory and load from file
        void* pPage = malloc(PAGE_SIZE);
        uint32_t nNumPages = pPager->nFileLength / PAGE_SIZE;

        //We might save a partial page at the end of the file
        if (pPager->nFileLength % PAGE_SIZE) {
            ++nNumPages;
        }

        if (nPageNum <= nNumPages) {
            lseek(pPager->nFileDescriptor, nPageNum * PAGE_SIZE, SEEK_SET);
            ssize_t nBytesRead = read(pPager->nFileDescriptor, pPage, PAGE_SIZE);
            if (-1 == nBytesRead) {
                printf("Error on reading file: %d\n", errno);
                exit(EXIT_FAILURE);
            }
        }

        pPager->aPages[nPageNum] = pPage;

        if (nPageNum >= pPager->nNumPages) {
            ++pPager->nNumPages;
        }
    }

    return pPager->aPages[nPageNum];
}

void pagerFlush(Pager* pPager, uint32_t nPageNum) {
    if (NULL == pPager->aPages[nPageNum]) {
        printf("Tried to flush null page\n");
        exit(EXIT_FAILURE);
    }

    off_t nOffset = lseek(pPager->nFileDescriptor, nPageNum * PAGE_SIZE, SEEK_SET);

    if (-1 == nOffset) {
        printf("Error on seeking: %d\n", errno);
        exit(EXIT_FAILURE);
    }

    ssize_t nBytesWritten = write(pPager->nFileDescriptor, pPager->aPages[nPageNum], PAGE_SIZE);

    if (-1 == nBytesWritten) {
        printf("Error on writing: %d\n", errno);
        exit(EXIT_FAILURE);
    }
}

uint32_t getUnusedPageNum(Pager* pPager) {
    return pPager->nNumPages;
}