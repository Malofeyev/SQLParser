#pragma once

#include "Expr.h"

class BinaryOperator : public Expr {
public:
    enum BinaryOp {
       BO_add,
       BO_sub,
       BO_lt,
       BO_eq,
       BO_land,
       BO_lor
    };
    BinaryOperator(BinaryOp bo, Expr* leftArg, Expr* rightArg);
    BinaryOp bo_;
    Expr* leftArg_;
    Expr* rightArg_;
    static bool classof(const Expr* expr);
    ~BinaryOperator();
};
