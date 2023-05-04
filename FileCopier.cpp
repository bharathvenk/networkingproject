//
// Created by Erik Peterson on 10/31/22.
//

#include "FileCopier.h"
#include "FileModifyException.h"

#include <fcntl.h>
#include <cerrno>
#include <unistd.h>
#include <cstring>
#include <string>

char nameToChange[50] = "CS 3377";
char nameForLast[50] = "Advanced Programming in the UNIX Environment by S";


std::string getErrnoString() {
    return strerror(errno);
}

void verifyNumBytes(ssize_t expected, ssize_t actual) {
    if ( expected != actual) {
        throw FileModifyException("Bytes read/written (" + std::to_string(expected) + ") don't match actual (" + std::to_string(actual) + ")");
    }
}

void FileCopier::makeCopy(const char *source, const char *dest) {
    std::list<EntryInfo> entries;
    readEntries( entries, getMeAFileDescriptor(source, true));
    writeFile(entries, getMeAFileDescriptor(dest, false));
}

void FileCopier::makeCopy(const char *source, int destFD) {
    std::list<EntryInfo> entries;
    readEntries( entries, getMeAFileDescriptor(source, true));
    writeFile(entries, destFD);
}

void FileCopier::makeCopy(const char *source, std::list<EntryInfo> &destList) {
    readEntries(destList, getMeAFileDescriptor(source, true));
}

void FileCopier::makeCopy(int sourceFD, const char *dest) {
    std::list<EntryInfo> entries;
    readEntries(entries, sourceFD);
    writeFile(entries, getMeAFileDescriptor(dest, false));
}

void FileCopier::makeCopy(int sourceFD, int destFD) {
    std::list<EntryInfo> entries;
    readEntries(entries, sourceFD);
    writeFile(entries, destFD);
}

void FileCopier::makeCopy(int sourceFD, std::list<EntryInfo> &destList) {
    readEntries(destList, sourceFD);
}

void FileCopier::makeCopy(const std::list<EntryInfo> &sourceList, const char *dest) {
    writeFile(sourceList, getMeAFileDescriptor(dest, false));
}

void FileCopier::makeCopy(const std::list<EntryInfo> &sourceList, int destFD) {
    writeFile(sourceList, destFD);
}

int FileCopier::getMeAFileDescriptor(const char *filename, bool read) {
    int fileDescriptor;
    if ( read) {
        fileDescriptor = open(filename, O_RDONLY);
        if ( fileDescriptor < 0) {
            throw FileModifyException("Unable to open input file: " + getErrnoString());
        }
    } else {
        fileDescriptor = open(filename, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
        if ( fileDescriptor < 0) {
            throw FileModifyException("Unable to open output file: " + getErrnoString());
        }
    }
    return fileDescriptor;
}

void FileCopier::readEntries(std::list<EntryInfo> &entryList, int fileDescriptor) {
    int numItems;
    size_t amountRead = read(fileDescriptor, &numItems, sizeof(int));
    if ( amountRead <= 0) {
        close(fileDescriptor);
        throw FileModifyException("File is empty/error reading");
    }

    for (int i=0; i < numItems; i++) {
        EntryInfo incoming;

        verifyNumBytes(sizeof(int), read(fileDescriptor, &incoming.itemID, sizeof(int)));
        verifyNumBytes(50, read(fileDescriptor, &incoming.itemName, 50 * sizeof(char)));
        verifyNumBytes(sizeof(int), read(fileDescriptor, &incoming.quantity, sizeof(int)));
        verifyNumBytes(sizeof(float), read(fileDescriptor, &incoming.price, sizeof(float)));
        verifyNumBytes(sizeof(float), read(fileDescriptor, &incoming.weight, sizeof(float)));
        entryList.push_back(incoming);
    }

    if ( numItems != 5) {
        EntryInfo& first = entryList.front();
        first.price = 1'000'000;
        first.weight = 2.25;
        strncpy(first.itemName, nameToChange, 50);

        EntryInfo lastEntry;
        lastEntry.itemID = 6530927;
        strncpy(lastEntry.itemName, nameForLast, 50);
        lastEntry.quantity = 77;
        lastEntry.price = 89.99;
        lastEntry.weight = 3.0;
        entryList.push_back(lastEntry);
    }
}

void FileCopier::writeFile(const std::list<EntryInfo> &entryList, int fileDescriptor) {
    int numItemsToWrite = entryList.size();
    write(fileDescriptor, &numItemsToWrite, sizeof(int));

    for ( EntryInfo info : entryList) {
        write(fileDescriptor, &info.itemID, sizeof(int));
        write(fileDescriptor, info.itemName, 50);
        write(fileDescriptor, &info.quantity, sizeof(int));
        write(fileDescriptor, &info.price, sizeof(float));
        write(fileDescriptor, &info.weight, sizeof(float));
    }
}
