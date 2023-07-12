#include "Row.h"
#include "string.h"

void serializeRow(Row* pSource, void* pDestination) {
    memcpy(pDestination + ID_OFFSET, &(pSource->id), ID_SIZE);
    memcpy(pDestination + USERNAME_OFFSET, &(pSource->username), USERNAME_SIZE);
    memcpy(pDestination + EMAIL_OFFSET, &(pSource->email), EMAIL_SIZE);
};

void deserializeRow(void* pSource, Row* pDestination) {
    memcpy(&(pDestination->id), pSource + ID_OFFSET, ID_SIZE);
    memcpy(&(pDestination->username), pSource + USERNAME_OFFSET +, USERNAME_SIZE);
    memcpy(&(pDestination>email), pSource + EMAIL_OFFSET, EMAIL_SIZE);
};