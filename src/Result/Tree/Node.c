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
const uint32_t INTERNAL_NODE_MAX_CELLS = 3; //Keep this small for testing

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
    uint32_t nOldMax = getNodeMaxKey(pCursor->pTable->pPager, pOldNode);
    uint32_t nNewPageNum = getUnusedPageNum(pCursor->pTable->pPager);
    void* pNewNode = getPage(pCursor->pTable->pPager, nNewPageNum);
    initializeLeafNode(pNewNode);
    *nodeParent(pNewNode) = *nodeParent(pOldNode);
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
        uint32_t nParentPageNum = *nodeParent(pOldNode);
        uint32_t nNewMax = getNodeMaxKey(pCursor->pTable->pPager, pOldNode);
        void* pParent = getPage(pCursor->pTable->pPager, nParentPageNum);

        updateInternalNodeKey(pParent, nOldMax, nNewMax);
        internalNodeInsert(pCursor->pTable, nParentPageNum, nNewPageNum);
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

    if (NODE_INTERNAL == getNodeType(pRoot)) {
        initializeInternalNode(pRightChild);
        initializeInternalNode(pLeftChild);
    }

    /**
     * Left child has data copied from old root
     **/
    memcpy(pLeftChild, pRoot, PAGE_SIZE);
    setNodeRoot(pLeftChild, false);

    if (NODE_INTERNAL == getNodeType(pLeftChild)) {
        void* pChild;
        for (int i = 0; i < *internalNodeNumKeys(pLeftChild); ++i) {
            pChild = getPage(pTable->pPager, *internalNodeChild(pLeftChild, i));
            *nodeParent(pChild) = nLeftChildPageNum;
        }
    }

    /**
     * Root node is a new internal node with one key and two children
     **/
    initializeInternalNode(pRoot);
    setNodeRoot(pRoot, true);
    *internalNodeNumKeys(pRoot) = 1;
    *internalNodeChild(pRoot, 0) = nLeftChildPageNum;
    uint32_t nLeftChildMaxKey = getNodeMaxKey(pTable->pPager, pLeftChild);
    *internalNodeKey(pRoot, 0) = nLeftChildMaxKey;
    *internalNodeRightChild(pRoot) = nRightChildPageNum;
    *nodeParent(pLeftChild) = pTable->nRootPageNum;
    *nodeParent(pRightChild) = pTable->nRootPageNum;
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
        uint32_t* pRightChild = internalNodeRightChild(pNode);
        if (INVALID_PAGE_NUM == *pRightChild) {
            printf("Tried to access right child of node, but was invalid page\n");
            exit(EXIT_FAILURE);
        }

        return pRightChild;
    } 

    uint32_t* pChild = internalNodeCell(pNode, nChildNum);
    if (INVALID_PAGE_NUM == *pChild) {
        printf("Tried to access child %d of node, but was invalid page\n", nChildNum);
        exit(EXIT_FAILURE);
    }

    return pChild;
};
uint32_t* internalNodeKey(void* pNode, uint32_t nKeyNum) {
    return (void*)internalNodeCell(pNode, nKeyNum) + INTERNAL_NODE_CHILD_SIZE;
};

void updateInternalNodeKey(void* pNode, uint32_t nOldKey, uint32_t nNewKey) {
    uint32_t nOldChildIndex = internalNodeFindChild(pNode, nOldKey);
    *internalNodeKey(pNode, nOldChildIndex) = nNewKey;
}

void internalNodeInsert(Table* pTable, uint32_t nParentPageNum, uint32_t nChildPageNum) {
    
    //Add a new child/key pair to parent that corresponds to child
    void* pParent = getPage(pTable->pPager, nParentPageNum);
    void* pChild = getPage(pTable->pPager, nChildPageNum);
    uint32_t nChildMaxKey = getNodeMaxKey(pTable->pPager, pChild);
    uint32_t nIndex = internalNodeFindChild(pParent, nChildMaxKey);

    uint32_t nOriginalNumKey = *internalNodeNumKeys(pParent);
    
    if (nOriginalNumKey >= INTERNAL_NODE_MAX_CELLS) {
        internalNodeSplitAndInsert(pTable, nParentPageNum, nChildPageNum);
        return;
    }

    uint32_t nRightChildPageNum = *internalNodeRightChild(pParent);
    
    //An internal node with a right child of INVALID_PAGE_NUM is empty
    if(INVALID_PAGE_NUM == nRightChildPageNum) {
        *internalNodeRightChild(pParent) = nChildPageNum;
        return;
    }
    
    void* pRightChild = getPage(pTable->pPager, nRightChildPageNum);

    //If we are already at the max number of cells for a node, we can't increment
    //  before splitting. Incrementing without inserting a new key/child pair
    //  and immediatelly calling internalNodeSplitAndInsert has the effect
    //  of creating a new key at (max_cells + 1) with an uninitialized value
    *internalNodeNumKeys(pParent) = nOriginalNumKey + 1;

    if (nChildMaxKey > getNodeMaxKey(pTable->pPager, pRightChild)) {

        //Replace right child
        *internalNodeChild(pParent, nOriginalNumKey) = nRightChildPageNum;
        *internalNodeKey(pParent, nOriginalNumKey) = getNodeMaxKey(pTable->pPager, pRightChild);
        *internalNodeRightChild(pParent) = nChildPageNum;

    } else {

        //Make room for the new cell
        for (uint32_t i = nOriginalNumKey; i > nIndex; --i) {
            void* pDestination = internalNodeCell(pParent, i);
            void* pSource = internalNodeCell(pParent, i - 1);
            memcpy(pDestination, pSource, INTERNAL_NODE_CELL_SIZE);
        }

        *internalNodeChild(pParent, nIndex) = nChildPageNum;
        *internalNodeKey(pParent, nIndex) = nChildMaxKey;
    }

}

uint32_t getNodeMaxKey(Pager* pPager, void* pNode) {
    if (NODE_LEAF == getNodeType(pNode)) {
        return *leafNodeKey(pNode, *leafNodeNumCell(pNode) - 1);
    }

    void* pRightChild = getPage(pPager, *internalNodeRightChild(pNode));
    return getNodeMaxKey(pPager, pRightChild);
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

    //Necessary because the root page number if 0. By not initializeing an internal
    //  node's right child to an invalid page number when initializing the node, we may
    //  end up with 0 as the node's right child, which makes the node a parent of the root
    *internalNodeRightChild(pNode) = INVALID_PAGE_NUM;

}

uint32_t* leafNodeNextLeaf(void* pNode) {
    return pNode + LEAF_NODE_NEXT_LEAF_OFFSET;
}

uint32_t* nodeParent(void* pNode) {
    return pNode + PARENT_POINTER_OFFSET;
}

void internalNodeSplitAndInsert(Table* pTable, uint32_t nParentPageNum, uint32_t nChildPageNum) {

    uint32_t nOldPageNum = nParentPageNum;
    void* pOldNode = getPage(pTable->pPager, nParentPageNum);
    uint32_t nOldMax = getNodeMaxKey(pTable->pPager, pOldNode);

    void* pChild = getPage(pTable->pPager, nChildPageNum);
    uint32_t nChildMax = getNodeMaxKey(pTable->pPager, pChild);

    uint32_t nNewPageNum = getUnusedPageNum(pTable->pPager);

    //Decaclaring a flag before updating pointers which
    //  records whether this operation involves splitting the root -
    //  if it does, we will insert our newly created node during
    //  the step where the table's new root is created. If it does
    //  not, we have to insert the newly created node into its parent after the old node's keys have been transferred over. We are
    //  not able to do this if the newly created node's parent is not a newly initialized root node,
    //  because in that case its parent may have existing keys aside from our old node which we are splitting. If that is true, we
    //  need to find a place for our newly created node in its parent, and we cannot
    //  insert it at the correct index if it does not yet have any keys.
    uint32_t nSplittingRoot = isNodeRoot(pOldNode);

    void* pParent;
    void* pNewNode;

    if (nSplittingRoot) {
        createNewRoot(pTable, nNewPageNum);
        pParent = getPage(pTable->pPager, pTable->nRootPageNum);
    
        //If we are splitting the root, we need to update pOldNode to point to the new
        //  root's left child, nNewPageNum will already point to the new root's right child
        nOldPageNum = *internalNodeChild(pParent, 0);
        pOldNode = getPage(pTable->pPager, nOldPageNum);
    } else {
        pParent = getPage(pTable->pPager, *nodeParent(pOldNode));
        pNewNode = getPage(pTable->pPager, nNewPageNum);
        initializeInternalNode(pNewNode);
    }

    uint32_t* pOldNumKey = internalNodeNumKeys(pOldNode);

    uint32_t nCurrentPageNum = *internalNodeRightChild(pOldNode);
    void* pCurrent = getPage(pTable->pPager, nCurrentPageNum);

    //First, put right child into new node and set right child of old node to invalid page number
    internalNodeInsert(pTable, nNewPageNum, nCurrentPageNum);
    *nodeParent(pCurrent) = nNewPageNum;
    *internalNodeRightChild(pOldNode) = INVALID_PAGE_NUM;

    //For each key until you get to the middle key, moe the key and the child to the new node
    for (int i = INTERNAL_NODE_MAX_CELLS - 1; i > INTERNAL_NODE_MAX_CELLS / 2; --i) {
        nCurrentPageNum = *internalNodeChild(pOldNode, i);
        pCurrent = getPage(pTable->pPager, nCurrentPageNum);

        internalNodeInsert(pTable, nNewPageNum, nCurrentPageNum);
        *nodeParent(pCurrent) = nNewPageNum;
        --(*pOldNumKey);
    }

    //Set child before middle key, which is now the highest key, to be node's irght child,
    //  and decrement number of keys
    *internalNodeRightChild(pOldNode) = *internalNodeChild(pOldNode, *pOldNumKey - 1);
    --(*pOldNumKey);

    //Determine which of the two nodes after the split should contain the child to be inserted,
    //  and insert the child
    uint32_t nMaxAfterSplitting = getNodeMaxKey(pTable->pPager, pOldNode);

    uint32_t nDestinationPageNum = nChildMax < nMaxAfterSplitting
        ? nOldPageNum
        : nNewPageNum;
    
    internalNodeInsert(pTable, nDestinationPageNum, nChildPageNum);
    *nodeParent(pChild) = nDestinationPageNum;

    updateInternalNodeKey(pParent, nOldMax, getNodeMaxKey(pTable->pPager, pOldNode));

    if (!nSplittingRoot) {
        internalNodeInsert(pTable, *nodeParent(pOldNode), nNewPageNum);
        *nodeParent(pNewNode) = *nodeParent(pOldNode);
    }


}