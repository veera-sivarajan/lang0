# pragma once
# include <string>
# include "./Token.hpp"
# include "./Resolver.hpp"

using std::string;

class Dlox {
private:
    static void run(string source);

public:
    static void runPrompt();
    static void runFile(string path);
    static void loadRunPrompt(string path);
};
