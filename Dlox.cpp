# include "./Dlox.hpp"
# include "./Scanner.hpp"

# include <string>
# include <iostream>
# include <vector>
# include <fstream>
# include <sstream>

using std::string;
using std::vector;
using std::cout;
using std::cin;

void Dlox::run(string source) {
    Scanner scanner;
    scanner.setSource(source);
    vector<Token> tokens = scanner.scanTokens();

    for (auto token : tokens) {
        token.print();
    }
}
    
void Dlox::runFile(string path) {
    // Copied second best answer from:
    // stackoverflow.com/questions/2602013/read-whole-ascii-file-into-c-stdstring
    std::ifstream inFile;
    inFile.open(path);
    std::stringstream strStream;
    strStream << inFile.rdbuf();
    std::string sourceStr = strStream.str();

    run(sourceStr);
}

void Dlox::runPrompt() {
    string input;
    while (1) {
        cout << "Dlox> ";
        getline(cin, input);
        run(input);
    }
}

int main(void) {
    Dlox::runFile("./test.dlox");
    return 0;
}
