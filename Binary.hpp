class Binary {
private:
    Expr left;
    Token operator;
    Expr right;

public:
    Binary(Expr left, Token operator, Expr right);
};
