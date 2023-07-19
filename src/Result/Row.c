#include "Row.h"
#include "string.h"
#include <stdio.h>

void serializeRow(Row* pSource, void* pDestination) {
    memcpy(pDestination + ID_OFFSET, &(pSource->id), ID_SIZE);
    strncpy(pDestination + USERNAME_OFFSET, pSource->strUsername, USERNAME_SIZE);
    strncpy(pDestination + EMAIL_OFFSET, pSource->strEmail, EMAIL_SIZE);
};

void deserializeRow(void* pSource, Row* pDestination) {
    memcpy(&(pDestination->id), pSource + ID_OFFSET, ID_SIZE);
    memcpy(&(pDestination->strUsername), pSource + USERNAME_OFFSET, USERNAME_SIZE);
    memcpy(&(pDestination->strEmail), pSource + EMAIL_OFFSET, EMAIL_SIZE);
};

void printRow(Row* row) {
    printf("(%d, %s, %s)\n", row->id, row->strUsername, row->strEmail);
}

const uint32_t ID_SIZE = SIZE_OF_ATTRIBUTE(Row, id);
const uint32_t USERNAME_SIZE = SIZE_OF_ATTRIBUTE(Row, strUsername);
const uint32_t EMAIL_SIZE = SIZE_OF_ATTRIBUTE(Row, strEmail);
const uint32_t ID_OFFSET = ID_OFFSET_VALUE;
const uint32_t USERNAME_OFFSET = USERNAME_OFFSET_VALUE;
const uint32_t EMAIL_OFFSET = EMAIL_OFFSET_VALUE;
const uint32_t ROW_SIZE = ROW_SIZE_VALUE;