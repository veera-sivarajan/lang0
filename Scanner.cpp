# include "./Scanner.hpp"
# include "./Error.hpp"
# include <string>
# include <iostream>
# include <vector>            
# include <map>

using std::string;
using std::vector;
using std::map;
using std::stod;
using std::any;

void Scanner::setSource(string source) {
    this->source = source;
}

vector<Token> Scanner::scanTokens() {
    while(!isAtEnd()) {
        // Beginning of next lexeme
        start = current;
        scanToken();
    }
    // Token token(TokenType::EOF_TOKEN, line, current - start, "");
    // tokens.push_back(token);
    tokens.emplace_back(TokenType::EOF_TOKEN, line, "", nullptr);
    return tokens;
}

bool Scanner::isAtEnd() {
    return current >= static_cast<int>(source.length());
}

void Scanner::scanToken() {
    char c = advance();

    switch(c) {
    case '(': addToken(TokenType::LEFT_PAREN); break;
    case ')': addToken(TokenType::RIGHT_PAREN); break;
    case '{': addToken(TokenType::LEFT_BRACE); break;
    case '}': addToken(TokenType::RIGHT_BRACE); break;
    case ',': addToken(TokenType::COMMA); break;
    case '.': addToken(TokenType::DOT); break;
    case '-': addToken(TokenType::MINUS); break;
    case '+': addToken(TokenType::PLUS); break;
    case ';': addToken(TokenType::SEMICOLON); break;
    case '*': addToken(TokenType::STAR); break;

    case '!':
        addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG); break;
    case '=':
        addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL); break;
    case '<':
        addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS); break;
    case '>':
        addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
        break;

    case '/':
        if (match('/')) {
            // Comments are ignored until end of line
            while (peek() != '\n' && !isAtEnd()) {
                advance();
            }
        } else {
            addToken(TokenType::SLASH);
        }
        break;

        // Ignore white spaces
    case ' ':
    case '\r':
    case '\t':
        break;

    case '\n':
        line += 1;
        break;

    case '"':
        makeString();
        break;

    default:
        if (isDigit(c)) {
            makeNumber();
        } else if (isAlpha(c)) {
            makeIdentifier();
        } else {
            std::cout << "Unexpected character\n";
        }
        break;
    } 
} 

char Scanner::advance() {
    current += 1; // index to character that will be evaluated next
    return source.at(current - 1); // returns character under consideration
}

void Scanner::addToken(TokenType type, any literal) {
    // .substr(first_character, number of character after first)
    // Token token(type, line, current - start, literal);
    // tokens.push_back(token);
    string text = source.substr(start, current - start);
    tokens.emplace_back(type, line, text, literal);
}

void Scanner::addToken(TokenType type) {
    addToken(type, "");
}

bool Scanner::match(char expected) {
    if (isAtEnd() || source.at(current) != expected) return false;

    // consume character only if it is what we're looking for
    current += 1;
    return true;
}

char Scanner::peek() {
    // look ahead at current unconsumed character
    if (isAtEnd()) return '\0';
    return source.at(current);
}
    
void Scanner::makeString() {
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') line += 1;
        advance();
    }

    // Unterminated String 
    if (isAtEnd()) {
        // Print error message
        Error::log(line, "Unterminated string.");
        return;
    }

    // Consume closing "
    advance();

    // String without surrounding quotes
    string value = source.substr(start + 1, current - start - 2);
    addToken(TokenType::STRING, value);
}

bool Scanner::isDigit(char c) {
    return c >= '0' && c <= '9';
}

char Scanner::peekNext() {
    if (current + 1 > static_cast<int>(source.length())) return '\0';
    return source.at(current + 1);
}

void Scanner::makeNumber() {
    while (isDigit(peek())) advance();

    if (peek() == '.' && isDigit(peekNext())) {
        // Consume the .
        advance();

        while (isDigit(peek())) advance();
    }

    int number = std::stod(source.substr(start, current - start));
    addToken(TokenType::NUMBER, number);
}

bool Scanner::isAlpha(char c) {
    // Any character starting with a letter or underscore
    return (c >= 'a' && c <= 'z') ||
        (c >= 'A' && c <= 'Z') ||
        c == '_';
}

bool Scanner::isAlphaNumeric(char c) {
    return isAlpha(c) || isDigit(c);
}

void Scanner::makeIdentifier() {
    while (isAlphaNumeric(peek())) advance();
    string text = source.substr(start, current - start);
    auto found = keywords.find(text); // found is an iterator
    TokenType type;
    if (found != keywords.end()) {
        type = found->second; // returns the value for key text
    } else {
        type = TokenType::IDENTIFIER;
    }
    addToken(type);
}
