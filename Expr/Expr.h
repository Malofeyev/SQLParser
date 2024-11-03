#pragma once

#include <string>
#include <vector>

class Expr {
public:
    enum Kind {
        variable,
        parenExpr,
        integerLiteral,
        floatingLiteral,
        booleanLiteral,
        stringLiteral,

        unaryOperator,
        binaryOperator,
        select
    };
    Expr(Kind type);
    Kind type_;
    virtual ~Expr();
};

class Variable : public Expr {
public:
    Variable(char* namespaces, std::string name);
    Variable(std::string name);
    std::string name_;
    char* namespaces_;
    static bool classof(const Expr* expr);
    ~Variable();
};

class ParenExpr : public Expr {
public:
    ParenExpr(Expr* subExpr);
    Expr* subExpr_;
    static bool classof(const Expr* expr);
    ~ParenExpr();
};

class IntegerLiteral : public Expr {
public:
    IntegerLiteral(int value);
    int value_;
    static bool classof(const Expr* expr);
    ~IntegerLiteral();
};

class FloatingLiteral : public Expr {
public:
    FloatingLiteral(float value);
    float value_;
    static bool classof(const Expr* expr);
    ~FloatingLiteral();
};

class BooleanLiteral : public Expr {
public:
    BooleanLiteral(bool value);
    bool value_;
    static bool classof(const Expr* expr);
    ~BooleanLiteral();
};

class StringLiteral : public Expr {
public:
    StringLiteral(std::string value);
    std::string value_;
    static bool classof(const Expr* expr);
    ~StringLiteral();
};
