#include <gtest/gtest.h>

#include <Expr/Casting.h>
#include <Expr/SQLCommand.h>
#include <Parser/Parser.h>
#include <Parser/Lexer.h>

class ParserTest : public ::testing::Test {
protected:
    yyscan_t scanner;
    std::vector<SQLCommand*> commands;
    Runner runner = [](SQLCommand* &arg) {};
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

TEST_F(ParserTest, Empty) {
    yy_scan_string("", scanner);
    int res = yyparse(commands, runner, scanner);
    ASSERT_EQ(res, 0);
    ASSERT_TRUE(commands.empty());
}

TEST_F(ParserTest, EmptySelect) {
    yy_scan_string("select;", scanner);
    int res = yyparse(commands, runner, scanner);
    ASSERT_EQ(res, 0);
    ASSERT_EQ(commands.size(), 1);
    ASSERT_TRUE(isa<Select>(commands[0]));
}

TEST_F(ParserTest, 3Command) {
    yy_scan_string("select; select ;\nselect;", scanner);
    int res = yyparse(commands, runner, scanner);
    ASSERT_EQ(res, 0);
    ASSERT_EQ(commands.size(), 3);
    for (auto cmd : commands) {
        ASSERT_TRUE(isa<Select>(cmd));
    }
}

TEST_F(ParserTest, Error) {
    yy_scan_string("error; select; \nselect \nwhere;", scanner);
    int res = yyparse(commands, runner, scanner);
    ASSERT_EQ(res, 0);
    ASSERT_EQ(commands.size(), 3);
    ASSERT_TRUE(isa<Error>(commands[0]));
    ASSERT_TRUE(isa<Select>(commands[1]));
    ASSERT_TRUE(isa<Error>(commands[2]));

}

TEST_F(ParserTest, SelectExprList) {
    yy_scan_string("select 1, 2 AS Name;", scanner);
    int res = yyparse(commands, runner, scanner);
    ASSERT_EQ(res, 0);
    ASSERT_EQ(commands.size(), 1);
    ASSERT_TRUE(isa<Select>(commands[0]));
    Select* select = dyn_cast<Select*>(commands[0]);
    auto* list = select->list_;
    ASSERT_NE(list, nullptr);
    ASSERT_EQ(list->size(), 2);
}

TEST_F(ParserTest, 3FromWhere) {
    const char* stmts = "select from table; \
                         select from table where TRUE and FALSE; \
                         select 5 where TRUE;";
    yy_scan_string(stmts, scanner);
    int res = yyparse(commands, runner, scanner);
    ASSERT_EQ(res, 0);
    ASSERT_EQ(commands.size(), 3);
    
    Select* select;
    ASSERT_TRUE(isa<Select>(commands[0]));
    select = dyn_cast<Select*>(commands[0]);
    ASSERT_NE(select->fromItem_, nullptr);
    ASSERT_EQ(select->whereCond_, nullptr);
    
    ASSERT_TRUE(isa<Select>(commands[1]));
    select = dyn_cast<Select*>(commands[1]);
    ASSERT_NE(select->fromItem_, nullptr);
    ASSERT_NE(select->whereCond_, nullptr);

    ASSERT_TRUE(isa<Error>(commands[2]));
}



TEST_F(ParserTest, AllColumns) {
    yy_scan_string("select table.*, * from table;", scanner);
    int res = yyparse(commands, runner, scanner);
    ASSERT_EQ(res, 0);
    ASSERT_EQ(commands.size(), 1);

    ASSERT_TRUE(isa<Select>(commands[0]));
    Select* select = dyn_cast<Select*>(commands[0]);
    ASSERT_EQ(select->list_->size(), 2);

    if (auto* allColumns = 
            dynamic_cast<SelectExpressionAllColumns*>(select->list_->at(0))) {
        ASSERT_NE(allColumns->namespaces_, nullptr);
        ASSERT_EQ(strcmp(allColumns->namespaces_, "table."), 0); 
    } else {
        FAIL() << "Couldn\'t cast to the select expression wildcard";
    }

    if (auto* allColumns = 
            dynamic_cast<SelectExpressionAllColumns*>(select->list_->at(1))) {
        ASSERT_EQ(allColumns->namespaces_, nullptr);
    } else {
        FAIL() << "Couldn\'t cast to the select expression wildcard";
    }
}
