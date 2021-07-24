# include "./Error.hpp"

using std::cout;

bool Error::hadError = false;

void Error::report(int line, string where, string message) {
    hadError = true;
    cout << "[line " << line << "] Error" << where << ": " << message;
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
