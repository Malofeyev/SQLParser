#include "UnaryOperator.h"

UnaryOperator::UnaryOperator(UnaryOp uo, Expr* arg) 
    : Expr(Expr::Kind::unaryOperator)
    , uo_(uo), arg_(arg) {}

bool UnaryOperator::classof(const Expr* expr) {
    return expr->type_ == Expr::Kind::unaryOperator;
}

UnaryOperator::~UnaryOperator() {
    delete arg_;
}
