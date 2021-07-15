# include "./Token.hpp"

# include <string>
# include <iostream>

using std::string;


string Object::toString() {
    switch(type) {
    case Object_nil:
        return "nil";
    case Object_bool:
        return boolean ? "1" : "0";
    case Object_str:
        return str;
    }
}

Object Object::make_num_obj(double number) {
    Object num_obj;
    num_obj.type = Object_num;
    num_obj.number = number;
    return num_obj;
}

Object Object::make_str_obj(string str) {
    Object str_obj;
    str_obj.type = Object_str;
    str_obj.str = str;
    return str_obj;
}

Object Object::make_bool_obj(bool boolean) {
    Object bool_obj;
    bool_obj.type = Object_bool;
    bool_obj.boolean = boolean;
    return bool_obj;
}

Object Object::make_nil_obj() {
    Object nil_obj;
    nil_obj.type = Object_nil;
    nil_obj.nil = nullptr;
    return nil_obj;
}

Token::Token (TokenType type, string lexeme, int line) {
    type = type;
    lexeme = lexeme;
    line = line;
}

string Token::toString () {
    return type + " " + lexeme;
}

