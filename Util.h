//
// Created by erik on 2/2/21.
//

#ifndef INC_21S_CS3377_PROJECT_UTIL_H
#define INC_21S_CS3377_PROJECT_UTIL_H


#include <ctime>

struct EntryInfo {
    int itemID;
    char itemName[50];
    int quantity;
    float price;
    float weight;
};

/**
 * This enum says whether the FileModifier will read, write, or both
 * Parts 1 and 3 are READ_AND_WRITE (single process), whereas parts 2 and 4
 * are in two parts (the initial process getting READ_AND_WRITE, and you giving
 * the 2nd process the proper code for its role)
 */
enum class IOType {
    READ,
    WRITE
};

class Util {
public:
    const static char* usage;
    const static char* inputFilename;
    const static char* outputFilename;

    static IOType toIOType(char type);
};


#endif //INC_21S_CS3377_PROJECT_UTIL_H
