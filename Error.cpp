# include "./Error.hpp"

# define RESET       "\033[0m"
# define BOLDYELLOW  "\033[1m\033[33m"      /* Bold Yellow */
# define BOLDRED     "\033[1m\033[31m"      /* Bold Red */
// inline bool Error::hadError = false;

void Error::report(int line, string where, string message) {
    hadError = true;
    std::cerr << BOLDRED << "[line " << line << "] Error" << RESET
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
    std::cerr << BOLDRED << "[line " << error.token.line
              << "] Error: " << RESET << error.what() << "\n";
    hadRuntimeError = true;
}

void Error::warn(Token token, string message) {
    std::cerr << BOLDYELLOW << "[line " << token.line << "] Warning" << RESET
              << " at '" + token.text + "'" << ": " << message << "\n";
}

