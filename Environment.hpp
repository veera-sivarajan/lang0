# pragma once

# include <map>
# include <any>
# include <memory>
# include "./Error.hpp"
# include "./Token.hpp"

class Env: public std::enable_shared_from_this<Env> {
private:
    std::map<std::string, std::any> values;

public:
    void assign(const Token &name, std::any value);
    void define(const std::string &name, std::any value);
    std::any get(const Token &name);
};
