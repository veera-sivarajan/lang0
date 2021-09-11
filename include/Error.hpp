# pragma once
# include <string>
# include "./Token.hpp"
# include "./RuntimeError.hpp"

class Error {
private:
    static void report(int line, std::string where, std::string message);

public:
    inline static bool hadError = false;
    inline static bool hadRuntimeError = false;
    
    static void log(int line, std::string message);
    static void log(Token token, std::string message);
    static void warn(Token token, std::string message);
    static void runtimeError(const RuntimeError &error);
};
