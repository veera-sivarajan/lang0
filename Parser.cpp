# include "./Parser.hpp"


Expr Parser::expression() {
    return equality();
}
