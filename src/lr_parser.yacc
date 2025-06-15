%{

#include <stdio.h>
#include <stdlib.h>

void yyerror(const char *s);
int yylex(void);

%}

%define parse.error verbose
%code requires {
    #include "lr_lexer.hpp"
}

%define api.value.type { TokenData }

%token Int Float Bool String
%token Program Begin In End Var Procedure Struct New
%token Ref Deref Int_L Float_L Bool_L String_L Null
%token If Then Else Fi While Do Until Od For To Step Return
%token Semicolon Colon Assign L_Paren R_Paren L_Bracket R_Bracket Comma Dot
%token Identifier
%token Plus Minus Times Divides Pow
%token And Or Not Lt Gt Leq Geq Eq Neq

%left Or
%left And
%nonassoc Not
%nonassoc Lt Gt Leq Geq Eq Neq
%left Plus Minus
%left Times Divides
%right Pow
%left Dot

%start program

%%

program:
    Program Identifier Begin decl_list_opt End
    ;

decl_list_opt:
    /* empty */
    | decl_list
    ;

decl_list:
    decl
    | decl_list Semicolon decl
    ;

decl:
    var_decl
    | proc_decl
    | rec_decl
    ;

var_decl:
    Var Identifier Colon type
    | Var Identifier Colon type Assign exp
    | Var Identifier Assign exp
    ;

proc_decl:
    Procedure Identifier L_Paren paramfield_list_opt R_Paren return_type_opt Begin proc_body End
    ;

proc_body:
    decl_block_opt stmt_list
    ;

decl_block_opt:
    /* empty */
    | decl_block
    ;

decl_block:
    In
    | decl_list In
    ;

rec_decl:
    Struct Identifier L_Bracket paramfield_decl_list_opt R_Bracket
    ;

paramfield_decl_list_opt:
    /* empty */
    | paramfield_decl_list
    ;

paramfield_decl_list:
    paramfield_decl
    | paramfield_decl_list Semicolon paramfield_decl
    ;

paramfield_list_opt:
    /* empty */
    | paramfield_list
    ;

paramfield_list:
    paramfield_decl
    | paramfield_list Comma paramfield_decl
    ;

paramfield_decl:
    Identifier Colon type
    ;

stmt_list:
    /* empty */
    | stmt
    | stmt_list Semicolon stmt
    ;

stmt:
    assign_stmt
    | if_stmt
    | while_stmt
    | for_stmt
    | do_until_stmt
    | return_stmt
    | call_stmt
    ;

assign_stmt:
    var Assign exp
    | deref_var Assign exp
    ;

if_stmt:
    If exp Then stmt_list else_opt Fi
    ;

else_opt:
    /* empty */
    | Else stmt_list
    ;

while_stmt:
    While exp Do stmt_list Od
    ;

for_stmt:
    For Identifier Eq exp To exp Step exp Do stmt_list Od
    ;

do_until_stmt:
    Do stmt_list Until exp Od
    ;

return_stmt:
    Return
    | Return exp
    ;

call_stmt:
    Identifier L_Paren exp_list_opt R_Paren
    ;

exp_list_opt:
    /* empty */
    | exp_list
    ;

exp_list:
    exp
    | exp_list Comma exp
    ;

exp:
    exp And exp
    | exp Or exp
    | Not exp
    | exp Lt exp
    | exp Gt exp
    | exp Leq exp
    | exp Geq exp
    | exp Eq exp
    | exp Neq exp
    | exp Plus exp
    | exp Minus exp
    | exp Divides exp
    | exp Times exp
    | exp Pow exp
    | literal
    | call_stmt
    | New Identifier
    | var
    | ref_var
    | deref_var
    | L_Paren exp R_Paren
    ;

ref_var:
    Ref L_Paren var R_Paren
    ;

deref_var:
    Deref L_Paren var R_Paren
    | Deref L_Paren deref_var R_Paren
    ;

var:
    Identifier
    | exp Dot Identifier
    ;

literal:
    Int_L
    | Float_L
    | String_L
    | Bool_L
    | Null
    ;

type:
    Int
    | Float
    | String
    | Bool
    | Identifier
    | Ref L_Paren type R_Paren
    ;

return_type_opt:
    /* empty */
    | Colon type
    ;

%%

int main(void) {
  return yyparse();
}

void yyerror(const char *msg) {
    fprintf(stderr, "Syntax error: %s at '%s' (line %d, column %d)\n",
            msg, yytext, yylineno, yycolumn);
}
