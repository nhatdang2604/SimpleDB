#ifndef __NODE__
#define __NODE__

#include <stdint.h>
#include "../Cursor.h"
#include "../Row.h"
#include "../Table.h"

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
extern const uint32_t LEAF_NODE_RIGHT_SPLIT_COUNT;
extern const uint32_t LEAF_NODE_LEFT_SPLIT_COUNT;

/*
 * Internal Node Header Layout
 */
extern const uint32_t INTERNAL_NODE_NUM_KEYS_SIZE;
extern const uint32_t INTERNAL_NODE_NUM_KEYS_OFFSET;
extern const uint32_t INTERNAL_NODE_RIGHT_CHILD_SIZE;
extern const uint32_t INTERNAL_NODE_RIGHT_CHILD_OFFSET;
extern const uint32_t INTERNAL_NODE_HEADER_SIZE;

/*
 * Internal Node Body Layout
 */
extern const uint32_t INTERNAL_NODE_KEY_SIZE;
extern const uint32_t INTERNAL_NODE_CHILD_SIZE;
extern const uint32_t INTERNAL_NODE_CELL_SIZE;

uint32_t* leafNodeNumCell(void* pNode);
void* leafNodeCell(void* pNode, uint32_t nCellNum);
uint32_t* leafNodeKey(void* pNode, uint32_t nCellNum);
void* leafNodeValue(void* pNode, uint32_t nCellNum);
void initializeLeafNode(void* pNode);
void initializeInternalNode(void* pNode);
void leafNodeInsert(Cursor* pCursor, uint32_t key, Row* pValue);
void leafNodeSplitAndInsert(Cursor* pCursor, uint32_t key, Row* pValue);
NodeType getNodeType(void* pNode);
void setNodeType(void* pNode, NodeType type);
void createNewRoot(Table* pTable, uint32_t nRightChildPageNum);
uint32_t* internalNodeNumKeys(void* pNode);
uint32_t* internalNodeRightChild(void* pNode);
uint32_t* internalNodeCell(void* pNode, uint32_t nCellNum);
uint32_t* internalNodeChild(void* pNode, uint32_t nChildNum);
uint32_t* internalNodeKey(void* pNode, uint32_t nKeyNum);
uint32_t getNodeMaxKey(void* pNode);
bool isNodeRoot(void* pNode);
void setNodeRoot(void* pNode, bool isRoot);

#endif