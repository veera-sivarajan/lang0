# pragma once

# include <stdexcept>

class RuntimeError: public std::runtime_error {
public:
    const Token &token;
    RuntimeError(const Token &token, std::string message);
};
