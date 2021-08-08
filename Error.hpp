# pragma once
# include <string>
# include "./Token.hpp"
# include "./RuntimeError.hpp"

using std::string;

class Error {
private:
    static void report(int line, string where, string message);

public:
    inline static bool hadError = false;
    inline static bool hadRuntimeError = false;
    static void log(int line, string message);
    static void log(Token token, string message);
    static void warn(Token token, string message);
    static void runtimeError(const RuntimeError &error);
};
