#include "dataType.h"

static DataType myType = UNDEFINED;

//Setter
void setDataType(DataType type) {
    myType = type;
}

// Getter
DataType getDataType() {
    return myType;
}

//ToString
char* dataTypeToString() {
    const char *prefix = "Data Type: ";
    char *result = malloc(30);  // allocate enough memory for the prefix and the datatype string
    switch (myType) {
        case TYPE_INT:
            sprintf(result, "%s%s", prefix, "INT");
            break;
        case TYPE_FLOAT:
            sprintf(result, "%s%s", prefix, "FLOAT");
            break;
        case TYPE_DOUBLE:
            sprintf(result, "%s%s", prefix, "DOUBLE");
            break;
        case TYPE_CHAR:
            sprintf(result, "%s%s", prefix, "CHAR");
            break;
        case UNDEFINED:
        default:
            sprintf(result, "%s%s", prefix, "unknown");
    }
    return result;
}


