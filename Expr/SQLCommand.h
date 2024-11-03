#pragma once

#include "Expr.h"

class SQLCommand {
public:
    enum Kind {
        select,
        error
    };
    SQLCommand(Kind type, int line, int column);
    Kind type_;
    int column_;
    int line_;
    virtual ~SQLCommand();
};

class Error : public SQLCommand {
public:
    Error(int line, int column, char* msg);
    char* msg_;
    static bool classof(SQLCommand* cmd);
    ~Error();
};

class SelectExpression {
public:
    SelectExpression();
    virtual ~SelectExpression();
};

class SelectExpressionExpr : public SelectExpression {
public:
    SelectExpressionExpr(Expr* expr, char* alias);
    Expr* expr_;
    char* alias_;
    ~SelectExpressionExpr();
};

class SelectExpressionAllColumns : public SelectExpression {
public:
    SelectExpressionAllColumns(char* namespaces);
    char* namespaces_;
    ~SelectExpressionAllColumns();
};

class SelectExpressionList {
public:
    SelectExpressionList();
    void push(SelectExpression* selExpr);
    SelectExpression* at(int index) const;
    size_t size() const;
    ~SelectExpressionList();
private:
    std::vector<SelectExpression*> selExprs_;
};

class Select : public Expr, public SQLCommand {
public:
    using FromItem = Expr*;
    enum DuplicateRowsOpt {
        all,
        distinct
    };
    Select(int column, int line, DuplicateRowsOpt duplicateOpt, 
            SelectExpressionList* list, 
            FromItem fromItem, Expr* whereCond);
    DuplicateRowsOpt duplicateOpt_;
    SelectExpressionList* list_;
    FromItem fromItem_;
    Expr* whereCond_;
    static bool classof(const Expr* expr);
    static bool classof(SQLCommand* cmd);
    ~Select();
};

