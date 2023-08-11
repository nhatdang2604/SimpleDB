#include "MetaCommandResult.h"
#include "string.h"
#include "Row.h"
#include "Tree/Node.h"

MetaCommandResult doMetaCommand(InputBuffer* pInputBuffer, Table* pTable) {
    if (0 == strcmp(pInputBuffer->aBuffer, ".exit")) {
        dbClose(pTable);
        closeInputBuffer(pInputBuffer);
        exit(EXIT_SUCCESS);

    } else if (0 == strcmp(pInputBuffer->aBuffer, ".constants")) {
        printf("Constants:\n");
        printConstants();
        return META_COMMAND_SUCCESS;

    } else if (0 == strcmp(pInputBuffer->aBuffer, ".btree")) {
        printf("Tree:\n");
        printTree(pTable->pPager, 0, 0);
        return META_COMMAND_SUCCESS;
    } else {
        return META_COMMAND_UNREGCONIZED_COMMAND;
    }
} 

void printConstants() {
    printf("ROW_SIZE: %d\n", ROW_SIZE);
    printf("COMMON_NODE_HEADER_SIZE: %d\n", COMMON_NODE_HEADER_SIZE);
    printf("LEAF_NODE_HEADER_SIZE: %d\n", LEAF_NODE_HEADER_SIZE);
    printf("LEAF_NODE_CELL_SIZE: %d\n", LEAF_NODE_CELL_SIZE);
    printf("LEAF_NODE_SPACE_FOR_CELLS: %d\n", LEAF_NODE_SPACE_FOR_CELLS);
    printf("LEAF_NODE_MAX_CELLS: %d\n", LEAF_NODE_MAX_CELLS);
}

void indent(uint32_t nLevel) {
    for (uint32_t i = 0; i < nLevel; ++i) {
        printf("  ");
    }
};
void printTree(Pager* pPager, uint32_t nPageNum, uint32_t nIndentationLevel) {
    void* pNode = getPage(pPager, nPageNum);
    //uint32_t nNumKeys, nChild;

    switch(getNodeType(pNode)) {
        case (NODE_LEAF): {
            uint32_t nNumKeys = *leafNodeNumCell(pNode);
            indent(nIndentationLevel);
            printf("- leaf (size %d)\n", nNumKeys);
            for (uint32_t i = 0 ; i < nNumKeys; ++i) {
                indent(nIndentationLevel + 1);
                printf("- %d\n", *leafNodeKey(pNode, i));
            }
            break;
        }

        case (NODE_INTERNAL): {
            uint32_t nNumKeys = *internalNodeNumKeys(pNode);
            indent(nIndentationLevel);
            printf("- internal (size %d)\n", nNumKeys);
            uint32_t nChild;
            for (uint32_t i = 0; i < nNumKeys; ++i) {
                nChild = *internalNodeChild(pNode, i);
                printTree(pPager, nChild, nIndentationLevel + 1);

                indent(nIndentationLevel + 1);
                printf("- key %d\n", *internalNodeKey(pNode, i));
            } 
            nChild = *internalNodeRightChild(pNode);
            printTree(pPager, nChild, nIndentationLevel + 1);
            break;
        }

    }
}


