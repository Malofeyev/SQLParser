#pragma once

#include "Expr.h"

class UnaryOperator : public Expr {
public:
    enum UnaryOp {
       UO_minus,
       UO_plus  
    };
    UnaryOperator(UnaryOp uo, Expr* arg);
    UnaryOp uo_;
    Expr* arg_;
    static bool classof(const Expr* expr);
    ~UnaryOperator();

};
