%{
#include <cstdlib>
#include <string>

#include "parsing_context.h"
#include "parser.hpp"

#undef yywrap
#define yywrap() 1

#define yyterminate() return END

static YY_BUFFER_STATE buf_state;

void init_scanner (char *src) { buf_state = yy_scan_string(src); }
void close_scanner () { yy_delete_buffer (buf_state); }

using namespace parsing_context;
%}

%option noyywrap nounput noyywrap batch yylineno

id     [a-zA-Z_$][a-zA-Z_'0-9$]*
digit  [0-9]
number {digit}+
rational {number}\.{number}
blank  [ \t]

%%

%{
  
%}

{blank}+               {  }
[\n]+                  {  }

"&"                    { return N_CONJUNCTION; }
"and"                  { return N_CONJUNCTION; }
"|"                    { return N_DISJUNCTION; }
"or"                   { return N_DISJUNCTION; }
"!"                    { return N_NEGATION;  }
"not"                  { return N_NEGATION;  }
"<=>"                  { return N_BIIMPLICATION; }
"<->"                  { return N_BIIMPLICATION; }
"=>"                   { return N_IMPLICATION; }
"->"                   { return N_IMPLICATION; }
"true"                 { return N_TRUE; }
"false"                { return N_FALSE; }

"+"                    { return N_ADDITION;  }
"-"                    { return N_SUBTRACTION;  }
"*"                    { return N_MULTIPLICATION;  }
"/"                    { return N_DIVISION;  }
"mod"                  { return N_MODULO;  }

"("                    { return '(';  }
")"                    { return ')'; }
","                    { return ','; }
":"                    { return ':'; }
";"                    { return ';'; }
"["                    { return '['; }
"]"                    { return ']'; }

"<"                    { return N_LOWER; }
">"                    { return N_GREATER; }
"<="                   { return N_LOWEREQUAL; }
">="                   { return N_GREATEREQUAL; }
"="                    { return N_EQUAL; }
"!="                   { return N_NOTEQUAL; }
"if"                   { return N_IF; }
"then"                 { return N_THEN; }
"else"                 { return N_ELSE; }
"endif"                { return N_ENDIF; }

"forall"               { return N_FORALL; }
"exists"               { return N_EXISTS; }
"boolean"              { return N_BOOLEAN; }
"bv"                   { return N_BV; }
"int"                  { return N_INT; }
"real"                 { return N_REAL; }
"unsigned"             { return N_UNSIGNED; }
"signed"               { return N_SIGNED; }
{rational}             { yylval=parsing_ast.new_node(N_RATIONAL, yytext);
                         return N_RATIONAL;
                       }
{number}               { yylval=parsing_ast.new_node(N_INTEGER, yytext);
                         return N_INTEGER;
                       }

"axiom"                { return N_AXIOM; }
"theorem"              { return N_THEOREM; }
"formula"              { return N_FORMULA; }
"regression"           { return N_REGRESSION; }

"//".*\n               { /* Comment, just swallow */ }

{id}                   { dstring s=yytext;
                         int n=parsing_ast.get_identifier(s);
                         yylval=parsing_ast.new_node(n, s);
                         return n;
                       }
"$"                    { return END; }

%%
