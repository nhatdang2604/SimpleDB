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
        printLeafNode(getPage(pTable->pPager, 0));
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

void printLeafNode(void* pNode) {
    uint32_t nNumCells = *leafNodeNumCell(pNode);
    printf("leaf (size %d)\n", nNumCells);
    for (uint32_t i = 0; i < nNumCells; ++i) {
        uint32_t key = *leafNodeKey(pNode, i);
        printf("\t- %d : %d\n", i, key);
    }
}