#include "Row.h"
#include "string.h"

void serializeRow(Row* pSource, void* pDestination) {
    memcpy(pDestination + ID_OFFSET, &(pSource->id), ID_SIZE);
    memcpy(pDestination + USERNAME_OFFSET, &(pSource->strUsername), USERNAME_SIZE);
    memcpy(pDestination + EMAIL_OFFSET, &(pSource->strEmail), EMAIL_SIZE);
};

void deserializeRow(void* pSource, Row* pDestination) {
    memcpy(&(pDestination->id), pSource + ID_OFFSET, ID_SIZE);
    memcpy(&(pDestination->strUsername), pSource + USERNAME_OFFSET +, USERNAME_SIZE);
    memcpy(&(pDestination>strEmail), pSource + EMAIL_OFFSET, EMAIL_SIZE);
};