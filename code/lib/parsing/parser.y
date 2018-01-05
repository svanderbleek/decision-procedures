%{
#include <string>
#include <sstream>
#include <stack>
#include <assert.h>

#include "parsing_context.h"

void yyerror (const char *);
int yylex();

using namespace parsing_context;
%}

%error-verbose
//%expect 8 // quantifier introduces shift reduce conflict, resolved by shifting!

%token N_IDENTIFIER "identifier"

%token N_INTEGER "integer number"
%token N_RATIONAL "rational number"

%token N_BOOLEAN "boolean"
%token N_UNSIGNED "unsigned"
%token N_SIGNED "signed"
%token N_REAL "real"
%token N_INT "int"
%token N_BV "bv"

%token N_AXIOM "axiom"
%token N_FORMULA "formula"
%token N_REGRESSION "regression"
%token N_THEOREM "theorem"
%token N_DECLARATION "declaration"
%token N_FORMULA_DEFINITION "formula definition"
%token N_TERM_DEFINITION "term definition"

%token N_CONJUNCTION "&"
%token N_DISJUNCTION "|"
%token N_NEGATION "!"
%token N_BIIMPLICATION "<=>"
%token N_IMPLICATION "=>"

%token N_TRUE "true"
%token N_FALSE "false"

%token N_ADDITION "+"
%token N_SUBTRACTION "-"
%token N_MULTIPLICATION "*"
%token N_DIVISION "/"
%token N_MODULO "%"

%token N_UMINUS "U-"

%token N_LOWER "<"
%token N_GREATER ">"
%token N_LOWEREQUAL "<="
%token N_GREATEREQUAL ">="
%token N_EQUAL "="
%token N_NOTEQUAL "!="
%token N_IF "IF"
%token N_THEN "THEN"
%token N_ELSE "ELSE"
%token N_ENDIF "ENDIF"

%token N_FORALL "forall"
%token N_EXISTS "exists"

%token END 0 "end of file"

%token N_LIST "list"
%token N_PREDICATE "predicate"
%token N_FUNCTION "function"

%nonassoc N_FORALL N_EXISTS

%left N_IMPLICATION N_BIIMPLICATION
%left N_DISJUNCTION
%left N_CONJUNCTION

%left N_LOWER N_GREATER N_LOWEREQUAL N_GREATEREQUAL N_EQUAL

%left N_ADDITION N_SUBTRACTION
%left N_MULTIPLICATION N_DIVISION N_MODULO

%nonassoc N_UMINUS
%nonassoc N_NEGATION

%start all

%% /* Grammar rules and actions follow */

all:       declarations
         | declarations ';'
         ;

declarations:
           declaration
         | declarations ';' declaration
           { $$=parsing_ast.new_node(N_LIST, $1, $3); }
         ;

identifier: N_IDENTIFIER | N_PREDICATE
         ;
             
identifiers:
           identifier
         | identifiers ',' identifier
           { $$=parsing_ast.new_node(N_LIST, $1, $3); }
         ;

declaration:
           identifiers ':' type
           { parsing_ast.set_type($1, $3);
             $$=parsing_ast.new_node(N_DECLARATION, $1, $3);
           }
         | identifiers ':' type N_EQUAL formula
           { parsing_ast.set_type($1, $3);
             $$=parsing_ast.new_node(N_FORMULA_DEFINITION,
                  parsing_ast.new_node(N_DECLARATION, $1, $3), $5);
           }
         | identifiers ':' type N_EQUAL term
           { parsing_ast.set_type($1, $3);
             $$=parsing_ast.new_node(N_TERM_DEFINITION,
                  parsing_ast.new_node(N_DECLARATION, $1, $3), $5);
           }
         | identifiers ':' N_THEOREM formula
           { $$=parsing_ast.new_node(N_THEOREM, $1, $4); }
         | N_THEOREM formula
           { $$=parsing_ast.new_node(N_THEOREM, 0, $2); }
         | formula
           { $$=parsing_ast.new_node(N_THEOREM, 0, $1); }
         | identifiers ':' N_AXIOM formula
           { $$=parsing_ast.new_node(N_AXIOM, $1, $4); }
         | N_AXIOM formula
           { $$=parsing_ast.new_node(N_AXIOM, 0, $2); }
         | identifiers ':' N_FORMULA formula
           { $$=parsing_ast.new_node(N_FORMULA, $1, $4); }
         | N_FORMULA formula
           { $$=parsing_ast.new_node(N_FORMULA, 0, $2); }
         | identifiers ':' N_REGRESSION formula
           { $$=parsing_ast.new_node(N_REGRESSION, $1, $4); }
         | N_REGRESSION formula
           { $$=parsing_ast.new_node(N_REGRESSION, 0, $2); }
         ;

type:
           N_IDENTIFIER
           { $$=parsing_ast.new_node(N_IDENTIFIER); }
         | N_BOOLEAN
           { $$=parsing_ast.new_node(N_BOOLEAN); }
         | N_BV '[' N_INTEGER ']'
           { $$=parsing_ast.new_node(N_BV, $3); }
         | N_INT
           { $$=parsing_ast.new_node(N_INT); }
         | N_REAL
           { $$=parsing_ast.new_node(N_REAL); }
         | N_SIGNED '[' N_INTEGER ']'
           { $$=parsing_ast.new_node(N_SIGNED, $3); }
         | N_UNSIGNED '[' N_INTEGER ']'
           { $$=parsing_ast.new_node(N_UNSIGNED, $3); }
         ;

formula: 
           N_FORALL identifiers ':' formula %prec N_FORALL
           { $$=parsing_ast.new_node(N_FORALL, $2, $4); }
         | N_EXISTS identifiers ':' formula %prec N_EXISTS
           { $$=parsing_ast.new_node(N_EXISTS, $2, $4); }
         | formula N_IMPLICATION formula
           { $$=parsing_ast.new_node(N_IMPLICATION, $1, $3); }
         | formula N_BIIMPLICATION formula
           { $$=parsing_ast.new_node(N_BIIMPLICATION, $1, $3); }
         | formula N_DISJUNCTION formula
           { $$=parsing_ast.new_node(N_DISJUNCTION, $1, $3); }
         | formula N_CONJUNCTION formula
           { $$=parsing_ast.new_node(N_CONJUNCTION, $1, $3); }
         | N_NEGATION formula        
           { $$=parsing_ast.new_node(N_NEGATION, $2); }
         | N_TRUE        
           { $$=parsing_ast.new_node(N_TRUE); }
         | N_FALSE        
           { $$=parsing_ast.new_node(N_FALSE); }
         | '(' formula ')'
           { $$ = $2; }
         | term N_LOWER term      
           { $$=parsing_ast.new_node(N_LOWER, $1, $3); }
         | term N_GREATER term       
           { $$=parsing_ast.new_node(N_GREATER, $1, $3); }
         | term N_LOWEREQUAL term       
           { $$=parsing_ast.new_node(N_LOWEREQUAL, $1, $3); }
         | term N_GREATEREQUAL term       
           { $$=parsing_ast.new_node(N_GREATEREQUAL, $1, $3); }
         | term N_EQUAL term       
           { $$=parsing_ast.new_node(N_EQUAL, $1, $3); }
         | term N_NOTEQUAL term       
           { $$=parsing_ast.new_node(N_NOTEQUAL, $1, $3); }
         | N_PREDICATE '(' terms ')'
           { $$=parsing_ast.new_node(N_PREDICATE, $3); }
         | N_PREDICATE
         ;

terms:     term
         | terms ',' term
           { $$=parsing_ast.new_node(N_LIST, $1, $2); }
         ;

term:      term N_ADDITION term      
           { $$=parsing_ast.new_node(N_ADDITION, $1, $3); }
         | term N_SUBTRACTION term       
           { $$=parsing_ast.new_node(N_SUBTRACTION, $1, $3); }
         | term N_MULTIPLICATION term 
           { $$=parsing_ast.new_node(N_MULTIPLICATION, $1, $3); }
         | term N_DIVISION term       
           { $$=parsing_ast.new_node(N_DIVISION, $1, $3); }
         | term N_MODULO term       
           { $$=parsing_ast.new_node(N_MODULO, $1, $3); }
         | '(' term ')'
           { $$ = $2; }
         | N_SUBTRACTION term %prec N_UMINUS
           { $$=parsing_ast.new_node(N_UMINUS, $2); }
         | N_IDENTIFIER '(' terms ')'
           { $$=parsing_ast.new_node(N_FUNCTION, $3); }
         | N_IF formula N_THEN term N_ELSE term N_ENDIF
           { $$=parsing_ast.new_node(N_IF, $2,
                parsing_ast.new_node(N_THEN, $4, $6)); }
         | N_INTEGER
         | N_RATIONAL
         | N_IDENTIFIER
         ;
%%

void yyerror(const char *message)
{
  extern int yylineno;
  *err << "(line " << yylineno << ") " << message << std::endl;
}

bool parsing_context::parse_file(const std::string &file)
{
  extern FILE *yyin;

  if(!(yyin=fopen(file.c_str(), "r")))
  {
    *err << "Could not open " << file << std::endl;
    return true;
  }
  
  int result=yyparse();

  fclose(yyin);
  
  return result;
}
