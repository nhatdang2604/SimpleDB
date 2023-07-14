#ifndef __ROW__
#define __ROW__

#include "stdint.h"

#define COLUMN_USERNAME_SIZE 255
#define COLUMN_EMAIL_SIZE 255

typedef struct {
    uint32_t id;
    char strUsername[COLUMN_USERNAME_SIZE];
    char strEmail[COLUMN_EMAIL_SIZE];
} Row;

#define SIZE_OF_ATTRIBUTE(Struct, Attribute) sizeof(((Struct*)0)->Attribute)
const uint32_t ID_SIZE = SIZE_OF_ATTRIBUTE(Row, id);
const uint32_t USERNAME_SIZE = SIZE_OF_ATTRIBUTE(Row, strUsername);
const uint32_t EMAIL_SIZE = SIZE_OF_ATTRIBUTE(Row, strEmail);
const uint32_t ID_OFFSET = 0;
const uint32_t USERNAME_OFFSET = ID_OFFSET + ID_SIZE;
const uint32_t EMAIL_OFFSET = USERNAME_OFFSET + USERNAME_SIZE;
const uint32_t ROW_SIZE = ID_SIZE + USERNAME_SIZE + EMAIL_SIZE;

void serializeRow(Row* pSource, void* pDestination);
void deserializeRow(void* pSource, Row* pDestination);
void printRow(Row* row);

#endif