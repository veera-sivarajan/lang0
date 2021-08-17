# include "../include/RuntimeError.hpp"

RuntimeError::RuntimeError(const Token &token, std::string message) :
    std::runtime_error{message}, token{token} {}
    
