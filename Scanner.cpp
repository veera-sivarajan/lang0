# include "./Token.hpp"

vector<Token> Scanner::scanTokens() {
    while(!isAtEnd()) {
        // Beginning of next lexeme
        start = current;
        scanToken();
    }
    Token token(EOF_TOKEN, "", line);
    tokens.push_back(token);
    return tokens;
}

bool Scanner::isAtEnd() {
    return current >= source.length();
}

void Scanner::scanToken() {
    char c = advance();

    switch(c) {
    case '(': addToken(LEFT_PAREN); break;
    case ')': addToken(RIGHT_PAREN); break;
    case '{': addToken(LEFT_BRACE); break;
    case '}': addToken(RIGHT_BRACE); break;
    case ',': addToken(COMMA); break;
    case '.': addToken(DOT); break;
    case '-': addToken(MINUS); break;
    case '+': addToken(PLUS); break;
    case ';': addToken(SEMICOLON); break;
    case '*': addToken(STAR); break;

    case '!': addToken(match('=') ? BANG_EQUAL : BANG); break;
    case '=': addToken(match('=') ? EQUAL_EQUAL : EQUAL); break;
    case '<': addToken(match('=') ? LESS_EQUAL : LESS); break;
    case '>': addToken(match('=') ? GREATER_EQUAL : GREAT); break;

    case '/':
        if (match('/')) {
            // Comments are ignored until end of line
            while (peek() != '\n' && !isAtEnd()) {
                advance();
            }
        } else {
            addToken(SLASH);
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
            cout << "Unexpected character\n";
        }
        break;
    } 
} 

char Scanner::advance() {
    current += 1; // index to character that will be evaluated next
    return source.at(current - 1); // returns character under consideration
}

void Scanner::addToken(TokenType type) {
    addToken(type, make_str_obj(""));
}

void Scanner::addToken(TokenType type, Object literal) {
    // .substr(first_character, number of character after first)
    string text = source.substr(start, current - start); 
    Token token(type, text, literal, line);
    tokens.push_back(token);
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
        cout << "Unterminated String\n";
        return;
    }

    // Consume closing "
    advance();

    // Strin without surrounding quotes
    string value = source.substr(start + 1, current - start - 2);
    addToken(STRING, Object::make_str_obj(value));
}

bool Scanner::isDigit(char c) {
    return c >= '0' && c <= '9';
}

char Scanner::peekNext() {
    if (current + 1 > source.length()) return '\0';
    return source.at(current + 1);
}

void Scanner::makeNumber() {
    while (isDigit(peek())) advance();

    if (peek() == '.' && isDigit(peekNext())) {
        // Consume the .
        advance();

        while (isDigit(peek())) advance();
    }

    double number = stod(soure.substr(start, current - start)); 
    addToken(NUMBER, Object::make_num_obj(number));
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
    while (isAlphaNumeic(peek())) advance();

    addToken(IDENTIFIER);
}
    
