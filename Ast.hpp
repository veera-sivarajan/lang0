# pragma once
# include <string>
# include <sstream>
# include "./Expression.hpp"

using std::string;
using std::any;

class Ast: public ExprVisitor {
private:
    template<class... E>
    string parenthesize(string name, E... expr);
public:
    string print(std::shared_ptr<Expr> expr);

    any visitBinaryExpr(std::shared_ptr<Binary> expr);
    any visitGroupingExpr(std::shared_ptr<Grouping> expr);
    any visitLiteralExpr(std::shared_ptr<Literal> expr);
    any visitUnaryExpr(std::shared_ptr<Unary> expr);
};


    
