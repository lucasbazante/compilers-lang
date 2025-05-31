%{

#include <stdio.h>
#include <stdlib.h>
#include "tokens.hpp"
void yyerror(const char *s);
int yylex(void);

%}

%define parse.error verbose
%define api.value.type {TokenData}
%code requires {
    #include "lexer.hpp"
}

%token Int 0 Float 1 Bool 2 String 3
%token Program 4 Begin 5 In 6 End 7 Var 8 Procedure 9 Struct 10 New 11
%token Ref 12 Deref 13 Int_L 14 Float_L 15 Bool_L 16 String_L 17 Null 18
%token If 19 Then 20 Else 21 Fi 22 While 23 Do 24 Until 25 Od 26 For 27 To 28 Step 29 Return 30
%token Semicolon 31 Colon 32 Assign 33 L_Paren 34 R_Paren 35 L_Bracket 36 R_Bracket 37 Comma 38
%token Identifier 40

%left Or 48
%left And 47
%nonassoc Not 1001
%nonassoc Comparison 49
%left Plus 41 Minus 42
%left Times 43 Divides 44
%right Pow 45
%left UNARYMINUS 1002
%left Dot 39

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
    For var Assign exp To exp Step exp Do stmt_list Od
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
    | exp Comparison exp
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