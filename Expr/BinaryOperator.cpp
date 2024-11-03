#include "BinaryOperator.h"

BinaryOperator::BinaryOperator(BinaryOp bo, Expr* leftArg, Expr* rightArg)
    : Expr(Expr::Kind::binaryOperator)
    , bo_(bo), leftArg_(leftArg), rightArg_(rightArg) {}

bool BinaryOperator::classof(const Expr* expr) {
    return expr->type_ == Expr::Kind::binaryOperator;
}

BinaryOperator::~BinaryOperator() {
    delete leftArg_;
    delete rightArg_;
}

