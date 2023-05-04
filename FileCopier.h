//
// Created by Erik Peterson on 10/31/22.
//

#ifndef INC_22S_3377_PROJECT_4_TEMPLATE_FILECOPIER_H
#define INC_22S_3377_PROJECT_4_TEMPLATE_FILECOPIER_H

#include <list>
#include "Util.h"

class FileCopier {
public:
    // Coming from a file
    static void makeCopy(const char* source, const char* dest);
    static void makeCopy(const char* source, int destFD);
    static void makeCopy(const char* source, std::list<EntryInfo>& destList);

    // Coming from a file descriptor
    static void makeCopy(int sourceFD, const char* dest);
    static void makeCopy(int sourceFD, int destFD);
    static void makeCopy(int sourceFD, std::list<EntryInfo>& destList);

    // Coming from an existing list
    static void makeCopy(const std::list<EntryInfo>& sourceList, const char* dest);
    static void makeCopy(const std::list<EntryInfo>& sourceList, int destFD);

private:
    static int getMeAFileDescriptor(const char* filename, bool read);

    static void readEntries(std::list<EntryInfo>& entryList, int fileDescriptor);

    static void writeFile(const std::list<EntryInfo>& entryList, int fileDescriptor);
};


#endif //INC_22S_3377_PROJECT_4_TEMPLATE_FILECOPIER_H
