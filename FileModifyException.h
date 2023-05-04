//
// Created by erik on 2/3/21.
//

#ifndef INC_21S_CS3377_PROJECT_FILEMODIFYEXCEPTION_H
#define INC_21S_CS3377_PROJECT_FILEMODIFYEXCEPTION_H

#include <exception>
#include <string>
#include <utility>


class FileModifyException : public std::exception {
public:
    explicit FileModifyException() noexcept = default;

    explicit FileModifyException(std::string message) noexcept
    : message(std::move(message)) {}

    const char* what() const noexcept override {
        return message.c_str();
    }
private:
    std::string message;
};


#endif //INC_21S_CS3377_PROJECT_FILEMODIFYEXCEPTION_H
