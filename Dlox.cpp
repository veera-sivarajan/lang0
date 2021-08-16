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

# include <readline/readline.h>
# include <readline/history.h>

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
    std::ostringstream builder;
    builder << BOLDBLUE << "Dlox> " << RESET;
    std::string prompt = builder.str();
    while (1) {
        input = readline(prompt.c_str());
        if (input.empty()) {
            continue;
        } else {
            add_history(input.c_str());
        }
        if (input == "exit") std::exit(0);
        run(input);
        Error::hadError = false;
    }
}

void Dlox::loadRunPrompt(string path) {
    Dlox::runFile(path);
    Dlox::runPrompt();
}

int main(void) {
    // Dlox::runFile("./test.dlox");
    // Dlox::runPrompt();
    Dlox::loadRunPrompt("./primitives.dlox");
    return 0;
}
