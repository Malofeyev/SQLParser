#include "Util.h"

#include <Expr/UnaryOperator.h>
#include <Expr/BinaryOperator.h>
#include <Expr/SQLCommand.h>
#include <Expr/Casting.h>

#include <cassert>
#include <string>

std::string getExprName(Expr* expr) {
    if (isa<IntegerLiteral>(expr) || isa<FloatingLiteral>(expr) ||
            isa<BooleanLiteral>(expr) || isa<StringLiteral>(expr)) {
        return "value";
    } else if (isa<Variable>(expr)) {
        return "field";
    } else if (isa<Select>(expr)) {
        return "command";
    } else {
        return "expression";
    }
}

JsonNode* convertToJson(Expr* expr) {
    if (auto literal = dyn_cast<IntegerLiteral*>(expr)) {
        return new JsonString(std::to_string(literal->value_));
    } else if (auto literal = dyn_cast<FloatingLiteral*>(expr)) {
        return new JsonString(std::to_string(literal->value_));
    } else if (auto literal = dyn_cast<BooleanLiteral*>(expr)) {
        return new JsonString(literal->value_?"True":"False");
    } else if (auto literal = dyn_cast<StringLiteral*>(expr)) {
        return new JsonString(literal->value_);
    } else if (auto variable = dyn_cast<Variable*>(expr)) {
        return new JsonString(variable->name_);
    } else if (auto parenExpr = dyn_cast<ParenExpr*>(expr)) {
        return convertToJson(parenExpr->subExpr_);
    } else if (auto binOp = dyn_cast<BinaryOperator*>(expr)) {
        JsonMap* map = new JsonMap();
        JsonList* list;
        Expr* left = binOp->leftArg_;
        Expr* right = binOp->rightArg_;
        switch (binOp->bo_) {
            case BinaryOperator::BinaryOp::BO_add:
                map->push({getExprName(left), convertToJson(left)});            
                map->push({"operation", new JsonString("+")});
                map->push({getExprName(right), convertToJson(right)});            
                return map;
            case BinaryOperator::BinaryOp::BO_sub:
                map->push({getExprName(left), convertToJson(left)});            
                map->push({"operation", new JsonString("-")});
                map->push({getExprName(right), convertToJson(right)});            
                return map;
            case BinaryOperator::BinaryOp::BO_lt:
                map->push({getExprName(left), convertToJson(left)});            
                map->push({"comparation", new JsonString("<")});
                map->push({getExprName(right), convertToJson(right)});            
                return map;
            case BinaryOperator::BinaryOp::BO_eq:
                map->push({getExprName(left), convertToJson(left)});            
                map->push({"comparation", new JsonString("=")});
                map->push({getExprName(right), convertToJson(right)});            
                return map;
            case BinaryOperator::BinaryOp::BO_land:
                map->push({"logical", new JsonString("AND")});
                list = new JsonList();
                list->push(convertToJson(left));
                list->push(convertToJson(right));
                map->push({"clauses", list});
                return map;
            case BinaryOperator::BinaryOp::BO_lor:
                map->push({"logical", new JsonString("OR")});
                list = new JsonList();
                list->push(convertToJson(left));
                list->push(convertToJson(right));
                map->push({"clauses", list});
                return map;
            default:
                assert(false && "Doesn\'t cover all binary operations");
        }
    } else if (auto unaryOp = dyn_cast<UnaryOperator*>(expr)) {
        std::string strOp;
        switch (unaryOp->uo_) {
            case UnaryOperator::UnaryOp::UO_minus:
                strOp = "-";
                break;
            case UnaryOperator::UnaryOp::UO_plus:
                strOp = "+";
                break;
            default:
                assert(false && "Doesn\'t cover all unary operations");
        }
        JsonMap* map = new JsonMap();
        map->push({"unary operator", new JsonString(strOp)});
        map->push({getExprName(unaryOp->arg_), convertToJson(unaryOp->arg_)});
        return map;
        return map;
    } else {
        assert(false && "Doesn\'t cover all Exprs");
    }
                
}


JsonNode* convertToJson(SQLCommand* cmd) {
    if (auto select = dyn_cast<Select*>(cmd)) {
        JsonMap* map = new JsonMap();
        map->push({"operation", new JsonString("SELECT")});
        JsonList* selectExprList = new JsonList();
        if (select->list_ != nullptr) {
            for (int i = 0; i < select->list_->size(); i++) {
                if (auto selExpr = dynamic_cast<SelectExpressionExpr*>(select->list_->at(i))) {
                    auto expr = selExpr->expr_;
                    if (selExpr->alias_ != nullptr) {
                        JsonMap* alias = new JsonMap();
                        alias->push({"expression", convertToJson(expr)});
                        alias->push({"alias", new JsonString(selExpr->alias_)});
                        selectExprList->push(alias);
                    } else {
                        selectExprList->push(convertToJson(expr));
                    }
                } else if (auto allColumns = dynamic_cast<SelectExpressionAllColumns*>(select->list_->at(i))) {
                    JsonMap* asterisk = new JsonMap();
                    asterisk->push({"operation", new JsonString("all columns")});
                    if (allColumns->namespaces_ != nullptr) {
                        asterisk->push({"reference", new JsonString(allColumns->namespaces_)});
                    }
                    selectExprList->push(asterisk);
                }
            }
        }
        map->push({"fields", selectExprList});
        
        if (select->fromItem_ != nullptr) {
            map->push({"from item", convertToJson(select->fromItem_)});
        }
        if (select->whereCond_ != nullptr) {
            map->push({"condition", convertToJson(select->whereCond_)});
        }
        return map;

    } else if (auto error = dyn_cast<Error*>(cmd)) {
        JsonString* msg = new JsonString("("+ std::to_string(error->line_) + ", " + 
                std::to_string(error->column_) + ") " + 
                std::string(error->msg_));
        JsonMap* map = new JsonMap();
        map->push({"error", msg});
        return map;
    } else {
        assert(false && "Doesn\'t cover all SQL commands");
    }
}
