//
// Created by erik on 2/3/21.
//
#include "Util.h"
#include "FileModifyException.h"

#include <string>

const char* Util::usage = "FileModifier [W]\n";
const char* Util::inputFilename = "input";
const char* Util::outputFilename = "output";

IOType Util::toIOType(char type) {
    switch(type) {
        case 'R':
            return IOType::READ;
        case 'W':
            return IOType::WRITE;
        default:
            std::string message = "Unknown IO type";
            message += type;
            throw FileModifyException(message);
    }
}
