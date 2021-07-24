# pragma once
# include <string>
# include "./Token.hpp"

using std::string;

class Dlox {
private:
    static bool hadError;
    static void run(string source);
    static void report(int line, string where, string message);

public:
    static void error(int line, string message);
    static void error(Token token, string message);
    static void runPrompt();
    static void runFile(string path);
};
