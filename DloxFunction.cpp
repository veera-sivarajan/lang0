# include "./DloxFunction.hpp"

DloxFunction::DloxFunction(std::shared_ptr<Statement::Function> declaration,
                           std::shared_ptr<Env> closure) :
    declaration{std::move(declaration)}, closure{std::move(closure)} {}

int DloxFunction::arity() {
    return declaration->params.size();
}

std::any DloxFunction::call(Interpreter &interpreter,
                            std::vector<std::any> arguments) {
    auto newEnv = std::make_shared<Env>(closure);
    int size = static_cast<int>(declaration->params.size());
    for (int i = 0; i < size; ++i) {
        newEnv->define(declaration->params[i].text, arguments[i]);
    }
    try {
        interpreter.executeBlock(declaration->body, newEnv);
    } catch (DloxReturn returnObject) {
        return returnObject.value;
    }
    return nullptr;
}

std::string DloxFunction::toString() {
    return "<fn " + declaration->name.text + ">";
}

LambdaFunction::LambdaFunction(std::shared_ptr<Lambda> declaration,
                               std::shared_ptr<Env> closure) :
    declaration{std::move(declaration)}, closure{std::move(closure)} {}

int LambdaFunction::arity() {
    return declaration->params.size();
}

std::any LambdaFunction::call(Interpreter &interpreter,
                            std::vector<std::any> arguments) {
    // newEnv's parent environment is closure
    auto newEnv = std::make_shared<Env>(closure);
    int size = static_cast<int>(declaration->params.size());
    for (int i = 0; i < size; ++i) {
        newEnv->define(declaration->params[i].text, arguments[i]);
    }
    try {
        interpreter.executeBlock(declaration->body, newEnv);
    } catch (DloxReturn returnObject) {
        return returnObject.value;
    }
    return nullptr;
}

std::string LambdaFunction::toString() {
    return "<lamdba fn>";
}
