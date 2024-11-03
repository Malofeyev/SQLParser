#include <gtest/gtest.h>

#include <Expr/BinaryOperator.h>
#include <Expr/Casting.h>
#include <Expr/SQLCommand.h>
#include <Expr/UnaryOperator.h>
#include <Parser/Parser.h>
#include <Parser/Lexer.h>
#include <Util/Util.h>

#include <fstream>

namespace {
    std::string readFile(std::ifstream& file) {
        std::string line;
        std::string result;
        bool firstLine = true;
        while (getline(file, line)) {
            result += (!firstLine?"\n":"") + line;
            firstLine = false;
        }
        return result;;
    }
}
class UtilTest : public ::testing::Test {
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

TEST(UtilTestNoParse, Int) {
    IntegerLiteral* lit = new IntegerLiteral(5);
    auto* json = convertToJson(lit);
    ASSERT_EQ(json->toString(0), "5");
    
    delete json;
    delete lit;
}

TEST(UtilTestNoParse, Variable) {
    Variable* lit = new Variable("var");
    auto* json = convertToJson(lit);
    ASSERT_EQ(json->toString(0), "var");

    delete lit;
    delete json;
}

TEST(UtilTestNoParse, BinOp) {
    IntegerLiteral* lit = new IntegerLiteral(5);
    Variable* var = new Variable("var");
    BinaryOperator* bo = new BinaryOperator(BinaryOperator::BO_add, var, lit);
    std::string ans = "{\n field : var,\n operation : +,\n value : 5\n}";
    auto* json = convertToJson(bo);
    
    ASSERT_EQ(json->toString(0), ans);
    
    delete bo;
    delete json;
}

TEST(UtilTestNoParse, Select) {
    SQLCommand* select = new Select(0, 0, Select::DuplicateRowsOpt::all, nullptr, nullptr, nullptr);
    std::string ans = "{\n operation : SELECT,\n fields : [  ]\n}";
    auto* json = convertToJson(select);
    
    ASSERT_EQ(json->toString(0), ans);
    
    delete select;
    delete json;
}

TEST_F(UtilTest, selectFromWhere) {
    std::ifstream file("json/selectFromWhere.json");
    ASSERT_TRUE(file.is_open());
    yy_scan_string("select a, b, c from table where True;", scanner);
    int res = yyparse(commands, runner, scanner);
    ASSERT_EQ(res, 0);
    ASSERT_EQ(commands.size(), 1);
    
    auto* json = convertToJson(commands[0]);
    std::string result = json->toString(0);
    ASSERT_EQ(result, readFile(file));
    
    delete json;
    file.close();

}


TEST_F(UtilTest, SelectExpr) {
    std::ifstream file("json/selectExpr.json");
    ASSERT_TRUE(file.is_open());
    yy_scan_string("select 1 as Two, a, b+c+1, c as D, * from table where True;", scanner);
    int res = yyparse(commands, runner, scanner);
    ASSERT_EQ(res, 0);
    ASSERT_EQ(commands.size(), 1);
    
    auto* json = convertToJson(commands[0]);
    std::string result = json->toString(0);
    ASSERT_EQ(result, readFile(file));
    
    delete json;
    file.close();

}

TEST_F(UtilTest, Where) {
    std::ifstream file("json/where.json");
    ASSERT_TRUE(file.is_open());
    yy_scan_string("select from table where True AND (x+1<y OR FALSE) OR 'A'='A';", scanner);
    int res = yyparse(commands, runner, scanner);
    ASSERT_EQ(res, 0);
    ASSERT_EQ(commands.size(), 1);
    
    auto* json = convertToJson(commands[0]);
    std::string result = json->toString(0);
    ASSERT_EQ(result, readFile(file));
    
    delete json;
    file.close();

}

