%{
#include "Parser.h"
#include <sstream>

static std::stringstream strbuf;
%}

%x singlequotedstring

%option header-file="Lexer.h"
%option outfile="Lexer.cpp"

%option reentrant
%option bison-bridge
%option bison-locations

%option noyywrap nodefault case-insensitive

%%

[ \t\n]+ /* skip whitespace */;


AS { return AS; }
DISTINCT { return DISTINCT; }
SELECT { return SELECT; }
FROM { return FROM; }
WHERE { return WHERE; }

TRUE { yylval->boolval = true; return BOOLVAL; }
FALSE { yylval->boolval = false; return BOOLVAL; }
AND { return AND; }
OR { return OR; }

[0-9]+\.[0-9]+ { yylval->floatval = atof(yytext); return FLOATNUM; }
[0-9]+ { yylval->intval = atoi(yytext); return INTNUM; }


([a-z]+\.)*\* { 
    int size = strlen(yytext)-1; 
    yylval->strval = (size?strndup(yytext, size):nullptr);
    return ALL_COLUMNS; 
}
([a-z]+\.)+[a-z]+ { yylval->strval = strdup(yytext); return NAME_WITH_NAMESPACE; }
[a-z]+ { yylval->strval = strdup(yytext); return NAME; }

\'                          { BEGIN singlequotedstring; strbuf.clear(); strbuf.str(""); }  
<singlequotedstring>\'\'    { strbuf << '\''; }
<singlequotedstring>[^']*   { strbuf << yytext; }
<singlequotedstring>\'      { BEGIN 0; yylval->strval = strdup(strbuf.str().c_str()); return STRING; }
<singlequotedstring><<EOF>> { fprintf(stderr, "[SQL-LEXER-ERROR] Unterminated string\n"); return 0; }

[-+,;)(<=*] { return yytext[0]; }


. { fprintf(stderr, "[SQL-LEXER-ERROR] Undefined symbol %c\n", yytext[0]); return 0; }

%%
