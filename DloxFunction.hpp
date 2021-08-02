# pragma once
# include <string>
# include <memory>

# include "./Interpreter.hpp"
# include "./Statement.hpp"
# include "./DloxCallable.hpp"
# include "./Environment.hpp"

class Function;

class DloxFunction: public DloxCallable {
private:
    std::shared_ptr<Function> declaration;

public:
    DloxFunction(std::shared_ptr<Function> declaration);
    int arity();
    std::any call(Interpreter &interpreter, std::vector<std::any> arguments);
    std::string toString();
};
    
