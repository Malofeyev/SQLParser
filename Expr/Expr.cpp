#include "Expr.h"

Expr::Expr(Kind type) : type_(type) {}

Expr::~Expr() = default;

Variable::Variable(char* namespaces, std::string name) 
    : Expr(Expr::Kind::variable)
    , name_(std::move(name))
    , namespaces_(namespaces) {}

Variable::Variable(std::string name) 
    : Expr(Expr::Kind::variable)
    , name_(std::move(name)) {}

bool Variable::classof(const Expr* expr) {
    return expr->type_ == Expr::Kind::variable;
}

Variable::~Variable() {
    free(namespaces_);
}

ParenExpr::ParenExpr(Expr* subExpr) 
    : Expr(Expr::Kind::parenExpr)
    , subExpr_(subExpr) {}

bool ParenExpr::classof(const Expr* expr) {
    return expr->type_ == Expr::Kind::parenExpr;
}

ParenExpr::~ParenExpr() = default;

IntegerLiteral::IntegerLiteral(int value) 
    : Expr(Expr::Kind::integerLiteral)
    , value_(value) {}

bool IntegerLiteral::classof(const Expr* expr) {
    return expr->type_ == Expr::Kind::integerLiteral;
}

IntegerLiteral::~IntegerLiteral() = default;

FloatingLiteral::FloatingLiteral(float value) 
    : Expr(Expr::Kind::floatingLiteral)
    , value_(value) {}

bool FloatingLiteral::classof(const Expr* expr) {
    return expr->type_ == Expr::Kind::floatingLiteral;
}

FloatingLiteral::~FloatingLiteral() = default;

BooleanLiteral::BooleanLiteral(bool value) 
    : Expr(Expr::Kind::booleanLiteral)
    , value_(value) {}

bool BooleanLiteral::classof(const Expr* expr) {
    return expr->type_ == Expr::Kind::booleanLiteral;
}

BooleanLiteral::~BooleanLiteral() = default;

StringLiteral::StringLiteral(std::string value) 
    : Expr(Expr::Kind::stringLiteral)
    , value_(std::move(value)) {}

bool StringLiteral::classof(const Expr* expr) {
    return expr->type_ == Expr::Kind::stringLiteral;
}

StringLiteral::~StringLiteral() = default;
