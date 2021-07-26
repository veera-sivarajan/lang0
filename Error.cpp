# include "./Error.hpp"

# define RESET "\033[0m"
# define RED   "\033[31m"      /* Red color */

// inline bool Error::hadError = false;

void Error::report(int line, string where, string message) {
    hadError = true;
    std::cerr << RED << "[line " << line << "] Error" << RESET
              << where << ": " << message << "\n";
}

void Error::log(int line, string message) {
    report(line, "", message);
}

void Error::log(Token token, string message) {
    if (token.type == TokenType::EOF_TOKEN) {
        report(token.line, " at end", message);
    } else {
        report(token.line, " at '"+ token.text + "'", message);
    }
}

void Error::runtimeError(const RuntimeError &error) {
    std::cerr << RED << "[line " << error.token.line
              << "] Error: " << RESET << error.what() << "\n";
    hadRuntimeError = true;
}

