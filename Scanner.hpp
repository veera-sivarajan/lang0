# include "./Token.hpp"

# include <string>
# include <iostream>
# include <vector>

using std::string;
using std::vector;

class Scanner {
private:
    int start = 0;   // first character in the lexeme being scanned
    int current = 0; // character currently under consideration
    int line = 1;    // line # of current
    string source;
    vector<Token> tokens;

    bool isAlpha(char c);
    bool isAlphaNumeric(char c);
    bool isDigit(char c);

    void scanToken();

public:
    Scanner(string source);
    bool isAtEnd();
    
