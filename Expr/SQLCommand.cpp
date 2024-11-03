#include "SQLCommand.h"

SQLCommand::SQLCommand(Kind type, int line, int column) 
    : type_(type), column_(column), line_(line) {}

SQLCommand::~SQLCommand() = default;

Error::Error(int line, int column, char* msg) 
    : SQLCommand(SQLCommand::error, line, column)
    , msg_(msg) {}

bool Error::classof(SQLCommand* cmd) {
    return cmd->type_ == SQLCommand::Kind::error;
}

Error::~Error() {
    free(msg_);
}

SelectExpression::SelectExpression() = default;

SelectExpression::~SelectExpression() = default;

SelectExpressionExpr::SelectExpressionExpr(Expr* expr, char* alias) 
    : expr_(expr), alias_(alias) {}

SelectExpressionExpr::~SelectExpressionExpr() {
    delete expr_;
    free(alias_);
}

SelectExpressionAllColumns::SelectExpressionAllColumns(char* namespaces)
    : namespaces_(namespaces) {}

SelectExpressionAllColumns::~SelectExpressionAllColumns() {
    free(namespaces_);
}

SelectExpressionList::SelectExpressionList() = default;

void SelectExpressionList::push(SelectExpression* selExpr) {
    selExprs_.push_back(selExpr);
}

SelectExpression* SelectExpressionList::at(int index) const {
    return selExprs_.at(index);
}

size_t SelectExpressionList::size() const {
    return selExprs_.size();
}

SelectExpressionList::~SelectExpressionList() {
    for (auto e : selExprs_) {
        delete e;
    }
}

Select::Select(int line, int column, DuplicateRowsOpt duplicateOpt, 
        SelectExpressionList* list,
        FromItem fromItem, Expr* whereCond)
    : Expr(Expr::Kind::select)
    , SQLCommand(SQLCommand::Kind::select, line, column)
    , duplicateOpt_(duplicateOpt)
    , list_(list)
    , fromItem_(fromItem)
    , whereCond_(whereCond) {}

bool Select::classof(const Expr* expr) {
    return expr->type_ == Expr::Kind::select;
}

bool Select::classof(SQLCommand* cmd) {
    return cmd->type_ == SQLCommand::Kind::select;
}

Select::~Select() {
    delete list_;
    delete fromItem_;
    delete whereCond_;
}
