# pragma once
# include <string>
# include <memory>

# include "./Interpreter.hpp"
# include "./Statement.hpp"
# include "./DloxCallable.hpp"
# include "./Environment.hpp"

// class Function;
// class Lambda;

class DloxFunction: public DloxCallable {
private:
    std::shared_ptr<Function> declaration;
    std::shared_ptr<Env> closure;

public:
    DloxFunction(std::shared_ptr<Function> declaration,
                 std::shared_ptr<Env> closure);
    int arity();
    std::any call(Interpreter &interpreter, std::vector<std::any> arguments);
    std::string toString();
};

class LambdaFunction: public DloxCallable {
private:
    std::shared_ptr<Lambda> declaration;
    std::shared_ptr<Env> closure;

public:
    LambdaFunction(std::shared_ptr<Lambda> declaration,
                   std::shared_ptr<Env> closure);
    int arity();
    std::any call(Interpreter &interpreter, std::vector<std::any> arguments);
    std::string toString();
};

    
