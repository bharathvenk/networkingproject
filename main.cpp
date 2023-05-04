#include <iostream>
#include <unistd.h>

#include "FileChecker.h"
#include "FileModifyException.h"
#include "Project3NetworkingModifier.h"
#include "Util.h"

//---------------------------------------------------------
// Arguments:
// IOType: No argment == (read from input file, write to the pipe)
//         'W' (read from the pipe, write to the output file)
//
//--------------------------------------------------------
int main(int argc, char** argv) {
    // Delete the output file so we're starting fresh
    unlink(Util::outputFilename);

    try {
        Project3NetworkingModifier modifier;
        modifier.modifyAndCopyFile(Util::inputFilename, Util::outputFilename);
    } catch (FileModifyException e) {
        std::cerr << "Error modifying file: " << e.what();
        exit(1);
    } catch ( std::exception e) {
        std::cerr << "Error modifying file: " << e.what();
        exit(1);
    }

    try {
        FileChecker::checkFile();
    } catch (std::exception e) {
        std::cerr << "File doesn't match" << std::endl;
        exit(1);
    }
}
