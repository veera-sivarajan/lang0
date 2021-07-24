# pragma once
# include <string>
# include "./Token.hpp"

using std::string;

class Error {
private:
    static void report(int line, string where, string message);

public:
    static bool hadError;
    static void log(int line, string message);
    static void log(Token token, string message);
};
