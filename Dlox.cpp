# include "./Dlox.hpp"
# include "./Parser.hpp"
# include "./Ast.hpp"
# include "./Interpreter.hpp"

# include <string>
# include <iostream>
# include <vector>
# include <fstream>
# include <sstream>
# define BOLDBLUE "\033[1m\033[34m"      /* Bold Blue */
# define RESET    "\033[0m"

using std::string;
using std::vector;
using std::cout;

Interpreter interpreter{};

void Dlox::run(string source) {
    Scanner scanner;
    scanner.setSource(source);
    vector<Token> tokens = scanner.scanTokens();
    
    Parser parser{tokens};
    std::vector<std::shared_ptr<Statement::Stmt>> statements = parser.parse();
    if (Error::hadError) return;

    Resolver resolver{interpreter};
    resolver.resolve(statements);

    if (Error::hadError) return;
    interpreter.interpret(statements);
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

    if (Error::hadError) std::exit(65);
    if (Error::hadRuntimeError) std::exit(70);
}

void Dlox::runPrompt() {
    string input;
    while (1) {
        cout << BOLDBLUE << "Dlox> " << RESET;
        std::getline(std::cin, input);
        if (input.empty()) continue;
        if (input == "exit") std::exit(0);
        run(input);
        Error::hadError = false;
    }
}

int main(void) {
    // Dlox::runFile("./test.dlox");
    Dlox::runPrompt();
    return 0;
}
