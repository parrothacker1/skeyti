#pragma once
#include <cstring>
#include <format>
#include <stdexcept>

namespace skeyti::exceptions {

class Exception : public std::runtime_error {
public:
  using std::runtime_error::runtime_error;
};

class UnknownException : public Exception {
public:
  UnknownException(int errorno) : Exception(std::format("Unknown reason. Here's errorno for your reference: {}", strerror(errorno))) {}
};

} // namespace skeyti::exceptions
