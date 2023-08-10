#include "Node.h"
#include "../Pager.h"
#include <stdlib.h>
#include <stdio.h>
#include "string.h"
/**
 * Common Node header layout
 */
const uint32_t NODE_TYPE_SIZE = sizeof(uint8_t);
const uint32_t NODE_TYPE_OFFSET = 0;
const uint32_t IS_ROOT_SIZE = sizeof(uint8_t);
const uint32_t IS_ROOT_OFFSET = NODE_TYPE_SIZE;
const uint32_t PARENT_POINTER_SIZE = sizeof(uint32_t);
const uint32_t PARENT_POINTER_OFFSET = IS_ROOT_OFFSET + IS_ROOT_SIZE;
const uint8_t COMMON_NODE_HEADER_SIZE = NODE_TYPE_SIZE + IS_ROOT_SIZE + PARENT_POINTER_SIZE;

/*
 * Leaf Node Header Layout
 */
const uint32_t LEAF_NODE_NUM_CELLS_SIZE = sizeof(uint32_t);
const uint32_t LEAF_NODE_NUM_CELLS_OFFSET = COMMON_NODE_HEADER_SIZE;
const uint32_t LEAF_NODE_HEADER_SIZE = COMMON_NODE_HEADER_SIZE + LEAF_NODE_NUM_CELLS_SIZE;

/*
 * Leaf Node Body Layout
 */
const uint32_t LEAF_NODE_KEY_SIZE = sizeof(uint32_t);
const uint32_t LEAF_NODE_KEY_OFFSET = 0;
const uint32_t LEAF_NODE_VALUE_SIZE = ROW_SIZE_VALUE;
const uint32_t LEAF_NODE_VALUE_OFFSET = LEAF_NODE_KEY_OFFSET + LEAF_NODE_KEY_SIZE;
const uint32_t LEAF_NODE_CELL_SIZE = LEAF_NODE_KEY_SIZE + LEAF_NODE_VALUE_SIZE;
const uint32_t LEAF_NODE_SPACE_FOR_CELLS = PAGE_SIZE_VALUE - LEAF_NODE_HEADER_SIZE;
const uint32_t LEAF_NODE_MAX_CELLS = LEAF_NODE_SPACE_FOR_CELLS / LEAF_NODE_CELL_SIZE;

uint32_t* leafNodeNumCell(void* pNode) {
    return pNode + LEAF_NODE_NUM_CELLS_OFFSET;
}

void* leafNodeCell(void* pNode, uint32_t nCellNum) {
    return pNode + LEAF_NODE_HEADER_SIZE + nCellNum * LEAF_NODE_CELL_SIZE;
}

uint32_t* leafNodeKey(void* pNode, uint32_t nCellNum) {
    return leafNodeCell(pNode, nCellNum) + LEAF_NODE_KEY_OFFSET;
}

void* leafNodeValue(void* pNode, uint32_t nCellNum) {
    return leafNodeCell(pNode, nCellNum) + LEAF_NODE_KEY_OFFSET + LEAF_NODE_KEY_SIZE;
}

void initializeLeafNode(void* pNode) {
    setNodeType(pNode, NODE_LEAF);
    *leafNodeNumCell(pNode) = 0;
}

void leafNodeInsert(Cursor* pCursor, uint32_t key, Row* pValue) {
    void* pNode = getPage(pCursor->pTable->pPager, pCursor->nPageNum);

    uint32_t nNumCell = *leafNodeNumCell(pNode);
    if (nNumCell >= LEAF_NODE_MAX_CELLS) {

        //Node full
        printf("Need to implement splitting a leaf node.\n");
        exit(EXIT_FAILURE);
    }

    if (pCursor->nCellNum < nNumCell) {

        //Make room for new cell
        for (uint32_t i = nNumCell; i > pCursor->nCellNum; --i) {
            memcpy(leafNodeCell(pNode, i), leafNodeCell(pNode, i - 1), LEAF_NODE_CELL_SIZE);
        }
    }

    ++(*(leafNodeNumCell(pNode)));
    *(leafNodeKey(pNode, pCursor->nCellNum)) = key;
    serializeRow(pValue, leafNodeValue(pNode, pCursor->nCellNum));
}

NodeType getNodeType(void* pNode) {
    uint8_t nValue = *((uint8_t*)(pNode + NODE_TYPE_OFFSET));
    return (NodeType)nValue;
};

void setNodeType(void* pNode, NodeType type) {
    uint8_t nValue = (uint8_t)type;
    *((uint8_t*)(pNode + NODE_TYPE_OFFSET)) = nValue;
};
