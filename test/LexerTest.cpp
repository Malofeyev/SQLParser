#include <gtest/gtest.h>

#include <Parser/Parser.h>
#include <Parser/Lexer.h>

class LexerTest : public ::testing::Test {
protected:
    yyscan_t scanner;
    YYSTYPE yylval;
    YYLTYPE yylloc;
    virtual void SetUp() {
        yylex_init(&scanner);
    }
    virtual void TearDown() {
        yylex_destroy(scanner);
    }

};

TEST_F(LexerTest, Select) {
    yy_scan_string("SELECT", scanner);
    auto selectVal = yylex(&yylval, &yylloc, scanner);
    ASSERT_EQ(selectVal, SELECT);
}

TEST_F(LexerTest, 3Terminal) {
    yytokentype res[] = { SELECT, FROM, WHERE };
    yy_scan_string("Select  From  Where", scanner);
   
    for (auto e : res) {
        ASSERT_EQ(yylex(&yylval, &yylloc, scanner), e);
    }
}
