# include "./Environment.hpp"

Env::Env() :
    previous{nullptr} {}

Env::Env(std::shared_ptr<Env> previous) :
    previous{std::move(previous)} {}

void Env::define(const std::string &name, std::any value) {
    values[name] = std::move(value);
}

std::any Env::get(const Token &name) {
    auto elem = values.find(name.text);
    if (elem != values.end()) {
        return elem->second;
    }
    // If variable not found in current env, check previous env
    if (previous != nullptr) return previous->get(name);

    throw RuntimeError(name, "Undefined variable '" + name.text + "'.");
}

std::any Env::getAt(int distance, std::string& name) {
    return anchestor(distance)->values[name];
}

void Env::assign(const Token &name, std::any value) {
    auto elem = values.find(name.text);
    if (elem != values.end()) {
        elem->second = std::move(value);
        return;
    }
    if (previous != nullptr) {
        previous->assign(name, std::move(value));
        return;
    }
    throw RuntimeError(name, "Undefined variable '" + name.text + "'.");
}

void Env::assignAt(int distance, Token& name, std::any value) {
    anchestor(distance)->values[name.text] = std::move(value);
}

std::shared_ptr<Env> Env::anchestor(int distance) {
    std::shared_ptr<Env> currentEnv = shared_from_this();
    for (int i = 0; i < distance; ++i) {
        currentEnv = currentEnv->previous;
    }
    return currentEnv;
}

void Env::printKeys() {
    for (auto const& [key, val] : shared_from_this()->values)  {
        if (val.type() == typeid(double)) {
            double num = std::any_cast<double>(val);
            std::cout << key << " : " << num << std::endl;
        } else {
            std::cout << key << std::endl;
        }
    }
}
