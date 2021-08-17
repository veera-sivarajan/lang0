# pragma once

# include <vector>
# include <any>

class Interpreter;

class DloxCallable {
public:
    virtual int arity() = 0;
    virtual std::any call(Interpreter &interpreter,
                          std::vector<std::any> arguments) = 0; 
    virtual std::string toString() = 0;
    virtual ~DloxCallable() = default;
};
