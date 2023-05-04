//
// Created by Erik Peterson on 8/9/22.
//

#include "FileChecker.h"
#include "FileModifyException.h"
#include "Util.h"
#include <fcntl.h>
#include <cerrno>
#include <iostream>
#include <math.h>
#include <unistd.h>
#include <sys/stat.h>

const char* truthFilename = "correctOutput";

const int SIZE_OF_ENTRY = 66;
const int INDEX_OF_ITEM_NAME = 4;
const int INDEX_OF_QUANTITY = 54;
const int INDEX_OF_PRICE = 58;
const int INDEX_OF_WEIGHT = 62;

int getFileLength(const char* filename) {
    struct stat theStats;
    int rc = stat(filename, &theStats);
    if ( rc < 0) {
        std::cerr << "Unable to get file stats on truth file" << std::endl;
        throw FileModifyException();
    }

    return theStats.st_size;
}

void printErrorAndThrow(int fileIndex, char truthChar, char studentChar) {
    if (fileIndex < 4) {
        std::cerr << "Number of items doesn't match" << std::endl;
        throw FileModifyException();
    }
    fileIndex -= 4;

    int entryIndex = fileIndex / SIZE_OF_ENTRY;
    int indexInEntry = fileIndex % SIZE_OF_ENTRY;
    if ( indexInEntry < INDEX_OF_ITEM_NAME) {
        std::cerr << "Item ID of (0-index) item " << entryIndex << " doesn't match" << std::endl;
        throw FileModifyException();
    } else if (indexInEntry < INDEX_OF_QUANTITY) {
        std::cerr << "Name of (0-index) item " << entryIndex << " doesn't match" << std::endl;
        std::cerr << "Character at (0-index) " << indexInEntry-INDEX_OF_ITEM_NAME
                  << " was " << studentChar
                  << " instead of " << truthChar << std::endl;
        throw FileModifyException();
    } else if ( indexInEntry < INDEX_OF_PRICE) {
        std::cerr << "Quantity of (0-index) item " << entryIndex << " doesn't match" << std::endl;
        throw FileModifyException();
    } else if ( indexInEntry < INDEX_OF_WEIGHT) {
        std::cerr << "Price of (0-index) item " << entryIndex << " doesn't match" << std::endl;
        throw FileModifyException();
    } else {
        std::cerr << "Weight of (0-index) item " << entryIndex << " doesn't match" << std::endl;
        throw FileModifyException();
    }
}

void FileChecker::checkFile() {
    int truthFile = open(truthFilename, O_RDONLY);
    int studentFile = open(Util::outputFilename, O_RDONLY);

    if ( studentFile < 0) {
        std::cerr << "Output file doesn't exist!" << std::endl;
        throw FileModifyException();
    }

    int truthFileLength = getFileLength(truthFilename);
    int studentFileLength = getFileLength(Util::outputFilename);

    int endPoint = std::min(truthFileLength, studentFileLength);

    char truthCharacter[1];
    char studentCharacter[1];
    int rc;
    for ( int i=0; i<endPoint; i++) {
        read(truthFile, &truthCharacter, 1);
        read(studentFile, &studentCharacter, 1);
        if ( truthCharacter[0] != studentCharacter[0]) {
            printErrorAndThrow(i, truthCharacter[0], studentCharacter[0]);
        }
    }
    if ( truthFileLength != studentFileLength) {
        std::cerr << "WARNING: File lengths do not match. Expected: " << truthFileLength
                  << "; yours: " << studentFileLength << std::endl;
        throw FileModifyException();
    }
    std::cout << "File modified successfully!" << std::endl;
}