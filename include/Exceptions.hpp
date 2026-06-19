#ifndef EXCEPTIONS_HPP
#define EXCEPTIONS_HPP

#include <exception>

class LabException : public std::exception {
private:
    const char* message;

public:
    explicit LabException(const char* message) : message(message) {}

    const char* what() const noexcept override {
        return message;
    }
};

class IndexOutOfRangeException : public LabException {
public:
    explicit IndexOutOfRangeException(const char* details) : LabException(details) {}
};

class EmptySequenceException : public LabException {
public:
    explicit EmptySequenceException(const char* details) : LabException(details) {}
};

class InvalidArgumentException : public LabException {
public:
    explicit InvalidArgumentException(const char* details) : LabException(details) {}
};

class InvalidOperationException : public LabException {
public:
    explicit InvalidOperationException(const char* details) : LabException(details) {}
};

class OptionHasNoValueException : public LabException {
public:
    explicit OptionHasNoValueException(const char* details) : LabException(details) {}
};

#endif
