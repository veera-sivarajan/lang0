# include "./Token.hpp"

# include <string>
# include <iostream>
# include <list>

using std::string;
using std::list;

class Scanner {
    int start = 0;
    int current = 0;
    int line = 1;
    string source;
    list <Token> tokens;  

    Scanner (string source) {
        source = source;
    }

    list <Token> scanTokens() {
        while (!isAtEnd()) {
            start = current;
            scanTokens();
        }

        tokens.push_back(Token(END, "", line));
        return tokens;
    }

    int isAtEnd () {
        return 1;
    }
};

int main () {
    return 0;
}
    
