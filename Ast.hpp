# pragma once
# include <string>
# include <sstream>
# include "./Expression.hpp"

using std::string;
using std::any;

class Ast: public ExprVisitor {
private:
    template<class... E>
    string parenthesize(std::string_view name, E... expr) {
        assert((... && std::is_same_v<E, std::shared_ptr<Expr>>));
        std::ostringstream buffer;
        buffer << "(" << name;
        ((buffer << " " << print(expr)), ...);
        buffer << ")";
        return buffer.str();
    }
public:
    string print(std::shared_ptr<Expr> expr);

    any visitBinaryExpr(std::shared_ptr<Binary> expr);
    any visitGroupingExpr(std::shared_ptr<Grouping> expr);
    any visitLiteralExpr(std::shared_ptr<Literal> expr);
    any visitUnaryExpr(std::shared_ptr<Unary> expr);
};


    
