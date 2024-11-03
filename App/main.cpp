#include <DataStorage/DataStorage.h>
#include <Expr/Casting.h>
#include <Expr/Expr.h>
#include <Expr/SQLCommand.h>
#include <Parser/Parser.h>
#include <Parser/Lexer.h>
#include <Util/Util.h>
#include <functional>
#include <iostream>

int main(int argc, char *argv[]) {
    
    yyscan_t scanner;
    std::vector<SQLCommand*> result;         
    yylex_init(&scanner);
    yyset_in(stdin, scanner);
    
    DataStorage ds;
    ds.tables["table"] = DataStorage::Fields{"id", "name"};

    Runner runner = [&ds](SQLCommand* &arg) {
        if (auto select = dyn_cast<Select*>(arg)) {
            auto itTable = ds.tables.end();
            std::string name;
            bool hasTable = (select->fromItem_ != nullptr);
            if (hasTable) {
                name = dyn_cast<Variable*>(select->fromItem_)->name_;
                itTable = ds.tables.find(name);
            }
            if (hasTable && itTable == ds.tables.end()) {
                std::string msg = "Couldn\'t find table: " + name;
                char* dyn_msg = strdup(msg.c_str());
                int line = arg->line_;
                int column = arg->column_;
                delete arg;
                arg = new Error(line, column, dyn_msg);
            } else if (select->list_ != nullptr){
                for (int i = 0; i < select->list_->size(); i++) {
                    if (auto* expr = dynamic_cast<SelectExpressionExpr*>(
                                select->list_->at(i))) {
                        if (auto* var = dyn_cast<Variable*>(expr->expr_)) {
                           if (!hasTable || 
                                   itTable->second.find(var->name_) == itTable->second.end()) {
                               std::string msg = "Couldn\'t find field: " + var->name_; 
                               char* dyn_msg = strdup(msg.c_str());
                               int line = arg->line_;
                               int column = arg->column_;
                               delete arg;
                               arg = new Error(line, column, dyn_msg);
                               break;
                           }

                        }
                    }
                }
            }
        }
        auto* json = convertToJson(arg);
        std::cout << (json->toString(0)) << "\n";
        delete json;            
    };
    yyparse(result, runner, scanner);
    yylex_destroy(scanner);
    //yylex();
    return 0;
}
