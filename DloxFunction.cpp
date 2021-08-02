# include "./DloxFunction.hpp"

DloxFunction::DloxFunction(std::shared_ptr<Function> declaration) :
    declaration{std::move(declaration)} {}

int DloxFunction::arity() {
    return declaration->params.size();
}

std::any DloxFunction::call(Interpreter &interpreter,
                            std::vector<std::any> arguments) {
    auto newEnv = std::make_shared<Env>(interpreter.global);
    int size = static_cast<int>(declaration->params.size());
    for (int i = 0; i < size; ++i) {
        newEnv->define(declaration->params[i].text, arguments[i]);
    }
    interpreter.executeBlock(declaration->body, newEnv);
    return nullptr;
}

std::string DloxFunction::toString() {
    return "<fn " + declaration->name.text + ">";
}
    
