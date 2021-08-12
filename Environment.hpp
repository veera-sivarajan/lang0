# pragma once

# include <map>
# include <any>
# include <memory>
# include "./Error.hpp"
# include "./Token.hpp"

// TODO: Add return statements to functions
class Env: public std::enable_shared_from_this<Env> {
private:
    std::shared_ptr<Env> previous;
    std::map<std::string, std::any> values;

public:
    Env();
    Env(std::shared_ptr<Env> previous);
    void assign(const Token &name, std::any value);
    void define(const std::string &name, std::any value);
    void assignAt(int distance, Token& name, std::any value);
    void printKeys();
    
    std::any get(const Token &name);
    std::any getAt(int distance, std::string& name);

    std::shared_ptr<Env> anchestor(int distance);
};
