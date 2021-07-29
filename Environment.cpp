# include "./Environment.hpp"

void Env::define(const std::string &name, std::any value) {
    values[name] = std::move(value);
}

std::any Env::get(const Token &name) {
    auto elem = values.find(name.text);
    if (elem != values.end()) {
        return elem->second;
    }

    throw RuntimeError(name, "Undefined variable '" + name.text + "'.");
}
