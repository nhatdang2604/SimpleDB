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
const uint32_t LEAF_NODE_NEXT_LEAF_SIZE = sizeof(uint32_t);
const uint32_t LEAF_NODE_NEXT_LEAF_OFFSET = LEAF_NODE_NUM_CELLS_OFFSET + LEAF_NODE_NUM_CELLS_SIZE;
const uint32_t LEAF_NODE_HEADER_SIZE = COMMON_NODE_HEADER_SIZE + LEAF_NODE_NUM_CELLS_SIZE + LEAF_NODE_NEXT_LEAF_SIZE;

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
const uint32_t LEAF_NODE_RIGHT_SPLIT_COUNT = (LEAF_NODE_MAX_CELLS + 1) / 2;
const uint32_t LEAF_NODE_LEFT_SPLIT_COUNT = (LEAF_NODE_MAX_CELLS + 1) - LEAF_NODE_RIGHT_SPLIT_COUNT;

/*
 * Internal Node Header Layout
 */
const uint32_t INTERNAL_NODE_NUM_KEYS_SIZE = sizeof(uint32_t);
const uint32_t INTERNAL_NODE_NUM_KEYS_OFFSET = COMMON_NODE_HEADER_SIZE;
const uint32_t INTERNAL_NODE_RIGHT_CHILD_SIZE = sizeof(uint32_t);
const uint32_t INTERNAL_NODE_RIGHT_CHILD_OFFSET = INTERNAL_NODE_NUM_KEYS_OFFSET + INTERNAL_NODE_NUM_KEYS_SIZE;
const uint32_t INTERNAL_NODE_HEADER_SIZE = COMMON_NODE_HEADER_SIZE + INTERNAL_NODE_NUM_KEYS_SIZE + INTERNAL_NODE_RIGHT_CHILD_SIZE;


/*
 * Internal Node Body Layout
 */
const uint32_t INTERNAL_NODE_KEY_SIZE = sizeof(uint32_t);
const uint32_t INTERNAL_NODE_CHILD_SIZE = sizeof(uint32_t);
const uint32_t INTERNAL_NODE_CELL_SIZE = INTERNAL_NODE_CHILD_SIZE + INTERNAL_NODE_KEY_SIZE;

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
    setNodeRoot(pNode, false);
    *leafNodeNumCell(pNode) = 0;
    *leafNodeNextLeaf(pNode) = 0; // 0 is represent no sibling
}

void leafNodeInsert(Cursor* pCursor, uint32_t key, Row* pValue) {
    void* pNode = getPage(pCursor->pTable->pPager, pCursor->nPageNum);

    uint32_t nNumCell = *leafNodeNumCell(pNode);
    if (nNumCell >= LEAF_NODE_MAX_CELLS) {

        //Node full
        leafNodeSplitAndInsert(pCursor, key, pValue);
        return;
    }

    if (pCursor->nCellNum < nNumCell) {

        //Make room for new cell
        for (uint32_t i = nNumCell; i > pCursor->nCellNum; --i) {
            memcpy(leafNodeCell(pNode, i), leafNodeCell(pNode, i - 1), LEAF_NODE_CELL_SIZE);
        }
    }

    *(leafNodeNumCell(pNode)) += 1;
    *(leafNodeKey(pNode, pCursor->nCellNum)) = key;
    serializeRow(pValue, leafNodeValue(pNode, pCursor->nCellNum));
}

void leafNodeSplitAndInsert(Cursor* pCursor, uint32_t key, Row* pValue) {
    
    /**
     * Create a new node and move half the cells over.
     * Insert the new value in one of the two nodes.
     * Update parent or create a new parent.
     **/
    void* pOldNode = getPage(pCursor->pTable->pPager, pCursor->nPageNum); 
    uint32_t nNewPageNum = getUnusedPageNum(pCursor->pTable->pPager);
    void* pNewNode = getPage(pCursor->pTable->pPager, nNewPageNum);
    initializeLeafNode(pNewNode);
    *leafNodeNextLeaf(pNewNode) = *leafNodeNextLeaf(pOldNode);
    *leafNodeNextLeaf(pOldNode) = nNewPageNum;


    /**
     * All existing key plus new key should be divided
     * evenly between old (left) and new (right) nodes.
     * Starting from the right, move each key to correct position
     **/
    for (int32_t i = LEAF_NODE_MAX_CELLS; i >= 0; --i) {
        void* pDestinationNode = pOldNode;

        if (i >= LEAF_NODE_LEFT_SPLIT_COUNT) {
            pDestinationNode = pNewNode;
        }

        uint32_t nIndexWithinNode = i % LEAF_NODE_LEFT_SPLIT_COUNT;
        void* pDestination = leafNodeCell(pDestinationNode, nIndexWithinNode);

        if (i == pCursor->nCellNum) {
            serializeRow(pValue, leafNodeValue(pDestinationNode, nIndexWithinNode));
            *leafNodeKey(pDestinationNode, nIndexWithinNode) = key;

        } else if (i > pCursor->nCellNum) {
            memcpy(pDestination, leafNodeCell(pOldNode, i - 1), LEAF_NODE_CELL_SIZE);
        } else {
            memcpy(pDestination, leafNodeCell(pOldNode, i), LEAF_NODE_CELL_SIZE);
        }
    }

    //Update cell count on both leaf nodes
    *(leafNodeNumCell(pOldNode)) = LEAF_NODE_LEFT_SPLIT_COUNT;
    *(leafNodeNumCell(pNewNode)) = LEAF_NODE_RIGHT_SPLIT_COUNT;

    //Update the parent node
    if (isNodeRoot(pOldNode)) {
        return createNewRoot(pCursor->pTable, nNewPageNum);
    } else {
        printf("Need to implement updating parent after splitting\n");
        exit(EXIT_FAILURE);
    }
}

NodeType getNodeType(void* pNode) {
    uint8_t nValue = *((uint8_t*)(pNode + NODE_TYPE_OFFSET));
    return (NodeType)nValue;
};

void setNodeType(void* pNode, NodeType type) {
    uint8_t nValue = (uint8_t)type;
    *((uint8_t*)(pNode + NODE_TYPE_OFFSET)) = nValue;
};

void createNewRoot(Table* pTable, uint32_t nRightChildPageNum) {
    /**
     * Handle splitting the root.
     * Old root copied to new page, becomes left child.
     * Address of right child passed in.
     * Re-initialize root page to contain the new root node
     * New root node points to two children
     **/

    void* pRoot = getPage(pTable->pPager, pTable->nRootPageNum);
    void* pRightChild = getPage(pTable->pPager, nRightChildPageNum);
    uint32_t nLeftChildPageNum = getUnusedPageNum(pTable->pPager);
    void* pLeftChild = getPage(pTable->pPager, nLeftChildPageNum);

    /**
     * Left child has data copied from old root
     **/
    memcpy(pLeftChild, pRoot, PAGE_SIZE);
    setNodeRoot(pLeftChild, false);

    /**
     * Root node is a new internal node with one key and two children
     **/
    initializeInternalNode(pRoot);
    setNodeRoot(pRoot, true);
    *internalNodeNumKeys(pRoot) = 1;
    *internalNodeChild(pRoot, 0) = nLeftChildPageNum;
    uint32_t nLeftChildMaxKey = getNodeMaxKey(pLeftChild);
    *internalNodeKey(pRoot, 0) = nLeftChildMaxKey;
    *internalNodeRightChild(pRoot) = nRightChildPageNum;
}

uint32_t* internalNodeNumKeys(void* pNode) {
    return pNode + INTERNAL_NODE_NUM_KEYS_OFFSET;
};
uint32_t* internalNodeRightChild(void* pNode) {
    return pNode + INTERNAL_NODE_RIGHT_CHILD_OFFSET;
};
uint32_t* internalNodeCell(void* pNode, uint32_t nCellNum) {
    return pNode + INTERNAL_NODE_HEADER_SIZE + nCellNum * INTERNAL_NODE_CELL_SIZE;
};
uint32_t* internalNodeChild(void* pNode, uint32_t nChildNum) {
    uint32_t nNumKey = *internalNodeNumKeys(pNode);
    if (nChildNum > nNumKey) {
        printf("Trying to access child_num %d > num_keys %d\n", nChildNum, nNumKey);
        exit(EXIT_FAILURE);
    } else if (nChildNum == nNumKey) {
        return internalNodeRightChild(pNode);
    } 

    return internalNodeCell(pNode, nChildNum);
};
uint32_t* internalNodeKey(void* pNode, uint32_t nKeyNum) {
    return internalNodeCell(pNode, nKeyNum) + INTERNAL_NODE_CHILD_SIZE;
};

/*
 * For an internal node, the maximum key is always its right most key
 * For a leaf node, the maximum key is always at the maximum index
 */
uint32_t getNodeMaxKey(void* pNode) {
    switch(getNodeType(pNode)) {
        case NODE_INTERNAL: {
            return *internalNodeKey(pNode, *internalNodeNumKeys(pNode) - 1);
        }

        case NODE_LEAF: {
            return *leafNodeKey(pNode, *leafNodeNumCell(pNode) - 1);
        }

    }
} 

bool isNodeRoot(void* pNode) {
    uint8_t nValue = *((uint8_t*)(pNode + IS_ROOT_OFFSET));
    return (bool) nValue;
};

void setNodeRoot(void* pNode, bool isRoot) {
    uint8_t nValue = isRoot;
    *((uint8_t*)(pNode + IS_ROOT_OFFSET)) = nValue;
};

void initializeInternalNode(void* pNode) {
    setNodeType(pNode, NODE_INTERNAL);
    setNodeRoot(pNode, false);
    *(internalNodeNumKeys(pNode)) = 0;
}

uint32_t* leafNodeNextLeaf(void* pNode) {
    return pNode + LEAF_NODE_NEXT_LEAF_OFFSET;
}