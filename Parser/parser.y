%{

#include <Expr/Expr.h>
#include <Expr/BinaryOperator.h>
#include <Expr/UnaryOperator.h>
#include <Expr/SQLCommand.h>

#include "Parser.h"
#include "Lexer.h"
void yyerror(YYLTYPE * llocp, std::vector<SQLCommand*>& cmds, const Runner& runner, 
    yyscan_t scanner, const char* msg);

%}

%code requires {
    #include <Expr/Expr.h>
    #include <Expr/SQLCommand.h>
    #include <functional>
    #include <vector>

    #ifndef YYtypeDEF_YY_SCANNER_T
    #define YYtypeDEF_YY_SCANNER_T
    typedef void* yyscan_t;
    #endif
    
    typedef std::function<void(SQLCommand*&)> Runner;

    #define YY_USER_ACTION                         \
        yylloc->first_line = yylloc->last_line;     \
        yylloc->first_column = yylloc->last_column; \
        for (int i = 0; yytext[i] != '\0'; i++) {  \
            if (yytext[i] == '\n') {               \
                yylloc->last_line++;                \
                yylloc->last_column = 0;            \
            } else {                               \
                yylloc->last_column++;              \
            }                                      \
        }
    
}

%union {
    char* strval; 
    int intval;
    float floatval;
    bool boolval;
    Select::DuplicateRowsOpt droval;
    Expr* exprval;
    SQLCommand* cmdval;
    SelectExpressionList* vecval;
    SelectExpression* seval;
}
%destructor {} <intval> <floatval> <boolval>
%destructor { free($$); } <strval> 
%destructor { delete $$; } <exprval> <seval> <vecval>

%initial-action {
    @$.first_column = 0;
    @$.last_column = 0;
    @$.first_line = 0;
    @$.last_line = 0;
};


%defines "Parser.h"
%output "Parser.cpp"

%define parse.error verbose
/* %parse-param { std::vector<Expr*>& result } 
%parse-param { std::function<void(Expr*)> runCommand } */
%locations
%define api.pure full
%lex-param { yyscan_t scanner }
%parse-param { std::vector<SQLCommand*>& outCommands }
%parse-param { const Runner& runner }
%parse-param { yyscan_t scanner }


%token AS 
%token SELECT FROM WHERE
%token ALL DISTINCT
%type <exprval> expr
%type <cmdval> command
%type <cmdval> select_stmt
%type <exprval> variable
%type <droval> duplicate_rows_opt
%type <vecval> select_expression_list_opt
%type <vecval> select_expression_list
%type <seval> select_expression
%type <exprval> from_item
%type <exprval> where_opt
%token <strval> NAME NAME_WITH_NAMESPACE ALL_COLUMNS
%token <strval> STRING
%token <intval> INTNUM 
%token <floatval> FLOATNUM
%token <boolval> BOOLVAL
%left OR 
%left AND 
%nonassoc '<' '='
%left '+' '-'
%nonassoc UMINUS UPLUS

%start input

%%

input : %empty 
      | commands
;

commands : command ';' { runner($1); outCommands.push_back($1); }
         | commands command ';' { runner($2); outCommands.push_back($2); }
         | commands error ';' { yyerrok; yyclearin; }
         | error ';' { yyerrok; yyclearin; }

/*outCommand.push_back(new Error(@1.first_column, @1.first_line, ""))}*/
;

command : %empty { $$ = nullptr; }
        | select_stmt { $$ = $1; } 
;

select_stmt : 
            SELECT duplicate_rows_opt select_expression_list_opt { 
                $$ = new Select(@1.first_line, @1.first_column, $2, $3, nullptr, nullptr); 
            }
            | SELECT duplicate_rows_opt select_expression_list_opt FROM from_item where_opt { 
                $$ = new Select(@1.first_line, @2.first_column, $2, $3, $5, $6);
            }
            
;


duplicate_rows_opt : %empty { $$ = Select::DuplicateRowsOpt::all; }
                   | ALL { $$ = Select::DuplicateRowsOpt::all; }
                   | DISTINCT { $$ = Select::DuplicateRowsOpt::distinct; }

select_expression_list_opt : %empty { $$ = nullptr; }
                           | select_expression_list { $$ = $1; }
;

select_expression_list : select_expression { 
                       $$ = new SelectExpressionList(); $$->push($1); 
                       }
                       | select_expression_list ',' select_expression {
                       $$ = $1; $$->push($3);
                       }
;

select_expression : expr { $$ = new SelectExpressionExpr($1, nullptr); }
                  | expr AS NAME { $$ = new SelectExpressionExpr($1, strdup($3)); }
                  | ALL_COLUMNS { $$ = new SelectExpressionAllColumns($1); }
;

from_item : variable { $$ = $1; }
;

where_opt : %empty { $$ = nullptr; }
          | WHERE expr { $$ = $2; }

expr : expr '+' expr { $$ = new BinaryOperator(BinaryOperator::BinaryOp::BO_add, $1, $3); }
     | expr '-' expr { $$ = new BinaryOperator(BinaryOperator::BinaryOp::BO_sub, $1, $3); }
     | expr '<' expr { $$ = new BinaryOperator(BinaryOperator::BinaryOp::BO_lt, $1, $3); }
     | expr '=' expr { $$ = new BinaryOperator(BinaryOperator::BinaryOp::BO_eq, $1, $3); }
     | expr AND expr { $$ = new BinaryOperator(BinaryOperator::BinaryOp::BO_land, $1, $3); }
     | expr OR expr { $$ = new BinaryOperator(BinaryOperator::BinaryOp::BO_lor, $1, $3); }
     | '-' expr %prec UMINUS { $$ = new UnaryOperator(UnaryOperator::UnaryOp::UO_minus, $2); }
     | '+' expr %prec UPLUS { $$ = new UnaryOperator(UnaryOperator::UnaryOp::UO_plus, $2); }
     | '('expr')' { $$ = new ParenExpr($2); }
     | variable { $$ = $1; }
     | INTNUM { $$ = new IntegerLiteral($1); }
     | FLOATNUM { $$ = new FloatingLiteral($1); }
     | BOOLVAL { $$ = new BooleanLiteral($1); }
     | STRING { $$ = new StringLiteral($1); free($1); }
;

variable : NAME_WITH_NAMESPACE { 
         char* name = strrchr($1, '.') + 1; 
         char* ns = strndup($1, strlen($1) - strlen(name)); $$ = new Variable(ns, name);
         free($1);
         }
         | NAME { $$ = new Variable($1); free($1); }
;
%%
void yyerror(YYLTYPE * llocp, std::vector<SQLCommand*>& cmds, const Runner& runner,
    yyscan_t scanner, const char* msg) {

    SQLCommand* err = new Error(llocp->first_line, llocp->first_column, strdup(msg));
    runner(err);
    cmds.push_back(err);
}

