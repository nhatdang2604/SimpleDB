#ifndef __ROW__
#define __ROW__

#include "stdint.h"

#define COLUMN_USERNAME_SIZE 255
#define COLUMN_EMAIL_SIZE 255
#define ID_OFFSET_VALUE 0
#define USERNAME_OFFSET_VALUE (ID_OFFSET_VALUE + SIZE_OF_ATTRIBUTE(Row, id))
#define EMAIL_OFFSET_VALUE (USERNAME_OFFSET_VALUE + SIZE_OF_ATTRIBUTE(Row, strEmail))
#define ROW_SIZE_VALUE (SIZE_OF_ATTRIBUTE(Row, id) + SIZE_OF_ATTRIBUTE(Row, strUsername) + SIZE_OF_ATTRIBUTE(Row, strEmail))


typedef struct {
    uint32_t id;
    char strUsername[COLUMN_USERNAME_SIZE];
    char strEmail[COLUMN_EMAIL_SIZE];
} Row;

#define SIZE_OF_ATTRIBUTE(Struct, Attribute) sizeof(((Struct*)0)->Attribute)

extern const uint32_t ID_SIZE;
extern const uint32_t USERNAME_SIZE;
extern const uint32_t EMAIL_SIZE;
extern const uint32_t ID_OFFSET;
extern const uint32_t USERNAME_OFFSET;
extern const uint32_t EMAIL_OFFSET;
extern const uint32_t ROW_SIZE;

void serializeRow(Row* pSource, void* pDestination);
void deserializeRow(void* pSource, Row* pDestination);
void printRow(Row* row);

#endif