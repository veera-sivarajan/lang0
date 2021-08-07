# pragma once

class Resolver: public ExprVisitor, public StmtVisitor {
private:
    Interpreter& interpreter;

public:
    Resolver(Interpreter& interpreter);
