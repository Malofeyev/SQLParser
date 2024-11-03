/* A Bison parser, made by GNU Bison 3.5.1.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2020 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* Undocumented macros, especially those whose name start with YY_,
   are private implementation details.  Do not rely on them.  */

#ifndef YY_YY_PARSER_H_INCLUDED
# define YY_YY_PARSER_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif
/* "%code requires" blocks.  */
#line 15 "/home/imalofeev98/projects/SQLParser/Parser/parser.y"

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
    

#line 75 "Parser.h"

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    AS = 258,
    SELECT = 259,
    FROM = 260,
    WHERE = 261,
    ALL = 262,
    DISTINCT = 263,
    NAME = 264,
    NAME_WITH_NAMESPACE = 265,
    ALL_COLUMNS = 266,
    STRING = 267,
    INTNUM = 268,
    FLOATNUM = 269,
    BOOLVAL = 270,
    OR = 271,
    AND = 272,
    UMINUS = 273,
    UPLUS = 274
  };
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 42 "/home/imalofeev98/projects/SQLParser/Parser/parser.y"

    char* strval; 
    int intval;
    float floatval;
    bool boolval;
    Select::DuplicateRowsOpt droval;
    Expr* exprval;
    SQLCommand* cmdval;
    SelectExpressionList* vecval;
    SelectExpression* seval;

#line 118 "Parser.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif

/* Location type.  */
#if ! defined YYLTYPE && ! defined YYLTYPE_IS_DECLARED
typedef struct YYLTYPE YYLTYPE;
struct YYLTYPE
{
  int first_line;
  int first_column;
  int last_line;
  int last_column;
};
# define YYLTYPE_IS_DECLARED 1
# define YYLTYPE_IS_TRIVIAL 1
#endif



int yyparse (std::vector<SQLCommand*>& outCommands, const Runner& runner, yyscan_t scanner);

#endif /* !YY_YY_PARSER_H_INCLUDED  */
