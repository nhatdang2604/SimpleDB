#ifndef __NODE__
#define __NODE__

#include <stdint.h>
#include "../Cursor.h"
#include "../Row.h"

typedef enum {NODE_INTERNAL, NODE_LEAF} NodeType; 

/**
 * Common Node header layout
 */
extern const uint32_t NODE_TYPE_SIZE;
extern const uint32_t NODE_TYPE_OFFSET;
extern const uint32_t IS_ROOT_SIZE;
extern const uint32_t IS_ROOT_OFFSET;
extern const uint32_t PARENT_POINTER_SIZE;
extern const uint32_t PARENT_POINTER_OFFSET;
extern const uint8_t COMMON_NODE_HEADER_SIZE;

/*
 * Leaf Node Header Layout
 */
extern const uint32_t LEAF_NODE_NUM_CELLS_SIZE;
extern const uint32_t LEAF_NODE_NUM_CELLS_OFFSET;
extern const uint32_t LEAF_NODE_HEADER_SIZE;

/*
 * Leaf Node Body Layout
 */
extern const uint32_t LEAF_NODE_KEY_SIZE;
extern const uint32_t LEAF_NODE_KEY_OFFSET;
extern const uint32_t LEAF_NODE_VALUE_SIZE;
extern const uint32_t LEAF_NODE_VALUE_OFFSET;
extern const uint32_t LEAF_NODE_CELL_SIZE;
extern const uint32_t LEAF_NODE_SPACE_FOR_CELLS;
extern const uint32_t LEAF_NODE_MAX_CELLS;

uint32_t* leafNodeNumCell(void* pNode);
void* leafNodeCell(void* pNode, uint32_t nCellNum);
uint32_t* leafNodeKey(void* pNode, uint32_t nCellNum);
void* leafNodeValue(void* pNode, uint32_t nCellNum);
void initializeLeafNode(void* pNode);
void leafNodeInsert(Cursor* pCursor, uint32_t key, Row* pValue);

#endif