# include "./Dlox.hpp"
# include "./Parser.hpp"
# include "./Ast.hpp"
# include "./Interpreter.hpp"

# include <string>
# include <iostream>
# include <vector>
# include <fstream>
# include <sstream>
# define BLUE    "\033[34m"    
# define RESET   "\033[0m"

using std::string;
using std::vector;
using std::cout;

Interpreter interpreter{};

void Dlox::run(string source) {
    Scanner scanner;
    scanner.setSource(source);
    vector<Token> tokens = scanner.scanTokens();
    // for (auto token : tokens) {
    //     token.print();
    // }
    Parser parser{tokens};
    // std::shared_ptr<Expr> expr = parser.parse();
    std::vector<std::shared_ptr<Stmt>> statements = parser.parse();
    if (Error::hadError) return;

    // std::cout << Ast{}.print(expr) << "\n";
    // interpreter.interpret(expr);
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
        cout << BLUE << "Dlox> " << RESET;
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
