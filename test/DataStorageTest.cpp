#include <gtest/gtest.h>

#include <Expr/Casting.h>
#include <Expr/SQLCommand.h>
#include <Parser/Parser.h>
#include <Parser/Lexer.h>
#include <DataStorage/DataStorage.h>

class DataStorageTest : public ::testing::Test {
protected:
    yyscan_t scanner;
    std::vector<SQLCommand*> commands;
    DataStorageTest() {
    }
    

    virtual void SetUp() {
        yylex_init(&scanner);
        commands.clear();
    }
    virtual void TearDown() {
        yylex_destroy(scanner);
        for (auto& command : commands) {
            delete command;
        }
    }

};

TEST_F(DataStorageTest, checkTables) {
    yy_scan_string("select f from table; select f from Undefined;", scanner);
    DataStorage ds;
    ds.tables["table"] = DataStorage::Fields{"f", "ff"};
    Runner runner = [&ds](SQLCommand* &arg) {
        if (auto select = dyn_cast<Select*>(arg)) {
            auto name = dyn_cast<Variable*>(select->fromItem_)->name_;
            if (ds.tables.find(name) == ds.tables.end()) {
                std::string msg = "Couldn\'t find table: " + name;
                char* dyn_msg = strdup(msg.c_str());
                auto old_arg = arg;
                arg = new Error(arg->line_, arg->column_, dyn_msg);
                delete old_arg;
            } 
        }            
    };
    int res = yyparse(commands, runner, scanner);
    ASSERT_EQ(res, 0);
    
    ASSERT_EQ(commands.size(), 2);
    ASSERT_TRUE(isa<Select>(commands[0]));
    ASSERT_TRUE(isa<Error>(commands[1]));
}

TEST_F(DataStorageTest, checkFields) {
    yy_scan_string("select f from table; select x from table;", scanner);
    DataStorage ds;
    ds.tables["table"] = DataStorage::Fields{"f", "ff"};
    Runner runner = [&ds](SQLCommand* &arg) {
        if (auto select = dyn_cast<Select*>(arg)) {
            auto name = dyn_cast<Variable*>(select->fromItem_)->name_;
            auto itTable = ds.tables.find(name); 
            if (itTable == ds.tables.end()) {
                std::string msg = "Couldn\'t find table: " + name;
                char* dyn_msg = strdup(msg.c_str());
                auto old_arg = arg;
                arg = new Error(arg->line_, arg->column_, dyn_msg);
                delete old_arg;
            } else {
                for (int i = 0; i < select->list_->size(); i++) {
                    if (auto* expr = dynamic_cast<SelectExpressionExpr*>(
                                select->list_->at(i))) {
                        if (auto* var = dyn_cast<Variable*>(expr->expr_)) {
                           auto itField = itTable->second.find(var->name_); 
                           if (itField == itTable->second.end()) {
                               std::string msg = "Couldn\'t find field: " + var->name_; 
                               char* dyn_msg = strdup(msg.c_str());
                               auto old_arg = arg;
                               arg = new Error(arg->line_, arg->column_, dyn_msg);
                               delete old_arg;
                               break;
                           }

                        }
                    }
                }
            }
        }            
    };
    int res = yyparse(commands, runner, scanner);
    ASSERT_EQ(res, 0);
    
    ASSERT_EQ(commands.size(), 2);
    ASSERT_TRUE(isa<Select>(commands[0]));
    ASSERT_TRUE(isa<Error>(commands[1]));
}
