# include <iostream>
# include <string>

using std::string;

enum TokenType {
    // Single Character Tokens
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE, COMMA, DOT, MINUS,
    PLUS, SEMICOLON, SLASH, STAR,
    // One or two character tokens
    BANG, BANG_EQUAL, EQUAL, EQUAL_EQUAL, GREATER, GREATER_EQUAL, LESS,
    LESS_EQUAL,
    // Literals
    IDENTIFIER, STRING, NUMBER,
    // Keywords
    AND, CLASS, ELSE, FALSE, FUN, FOR, IF, NIL, OR, PRINT, RETURN, SUPER,
    THIS, TRUE, VAR, WHILE,

    EOF_TOKEN                // Using this instead of EOF becuase EOF is keyword
};

class Object {
public:
    typedef enum {
        Object_str,
        Object_num,
        Object_bool,
        Object_nil,
        Object_fun,
        Object_instance,
        Object_class
    } Object_type;

    Object_type type;

    string str;
    double number;
    bool boolean;
    int *nil;

    string toString();

    static Object make_num_obj(double number);
    static Object make_str_obj(string str);
    static Object make_bool_obj(bool boolean);
    static Object make_nil_obj();
};
        

class Token {
private:
    TokenType type;
    string lexeme;
    Object literal;
    int line; 

public:
    Token (TokenType type, string lexeme, Object literal, int line);
    string toString();
};
