%{
#include <stdio.h>
#include <stdlib.h>

void yyerror(const char *s);
int yylex(void);
%}

%token PROGRAM NAME BEGIN END VAR PROCEDURE STRUCT ':' ASSIGN
%token '(' ')' '{' '}' ';' ','
%token IF THEN ELSE FI WHILE DO OD RETURN NEW REF DEREF IN
%token FOR TO STEP UNTIL
%token INT_LITERAL FLOAT_LITERAL STRING_LITERAL BOOL_LITERAL NULLVAL
%token INT FLOAT STRING BOOL

%left OR
%left AND
%nonassoc NOT
%nonassoc COMPARISON
%left SUM MINUS
%left MULT DIV
%right EX
%left UNARYMINUS
%left '.'

%start program

%%

program:
    PROGRAM NAME BEGIN decl_list_opt END
    ;

decl_list_opt:
    /* empty */
    | decl_list
    ;

decl_list:
    decl
    | decl_list ';' decl
    ;

decl:
    var_decl
    | proc_decl
    | rec_decl
    ;

var_decl:
    VAR NAME ':' type
    | VAR NAME ':' type ASSIGN exp
    | VAR NAME ASSIGN exp
    ;

proc_decl:
    PROCEDURE NAME '(' paramfield_list_opt ')' return_type_opt BEGIN proc_body END
    ;

proc_body:
    decl_block_opt stmt_list
    ;

decl_block_opt:
    /* empty */
    | decl_block
    ;

decl_block:
    decl_list IN
    ;

rec_decl:
    STRUCT NAME '{' paramfield_decl_list_opt '}'
    ;

paramfield_decl_list_opt:
    /* empty */
    | paramfield_decl_list
    ;

paramfield_decl_list:
    paramfield_decl
    | paramfield_decl_list ';' paramfield_decl
    ;

paramfield_list_opt:
    /* empty */
    | paramfield_list
    ;

paramfield_list:
    paramfield_decl
    | paramfield_list ',' paramfield_decl
    ;

paramfield_decl:
    NAME ':' type
    ;

stmt_list:
    /* empty */
    | stmt
    | stmt_list ';' stmt
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
    var ASSIGN exp
    | deref_var ASSIGN exp
    ;

if_stmt:
    IF exp THEN stmt_list else_opt FI
    ;

else_opt:
    /* empty */
    | ELSE stmt_list
    ;

while_stmt:
    WHILE exp DO stmt_list OD
    ;

for_stmt:
    FOR var '=' exp TO exp STEP exp DO stmt_list OD
    ;

do_until_stmt:
    DO stmt_list UNTIL exp OD
    ;

return_stmt:
    RETURN
    | RETURN exp
    ;

call_stmt:
    NAME '(' exp_list_opt ')'
    ;

exp_list_opt:
    /* empty */
    | exp_list
    ;

exp_list:
    exp
    | exp_list ',' exp
    ;

exp:
    exp AND exp
    | exp OR exp
    | NOT exp
    | exp COMPARISON exp
    | exp SUM exp
    | exp MINUS exp
    | exp DIV exp
    | exp MULT exp
    | exp EX exp
    | literal
    | call_stmt
    | NEW NAME
    | var
    | ref_var
    | deref_var
    | '(' exp ')'
    ;

ref_var:
    REF '(' var ')'
    ;

deref_var:
    DEREF '(' var ')'
    | DEREF '(' deref_var ')'
    ;

var:
    NAME
    | exp '.' NAME
    ;

literal:
    INT_LITERAL
    | FLOAT_LITERAL
    | STRING_LITERAL
    | BOOL_LITERAL
    | NULLVAL
    ;

type:
    INT
    | FLOAT
    | STRING
    | BOOL
    | NAME
    | REF '(' type ')'
    ;

return_type_opt:
    /* empty */
    | ':' type
    ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Erro: %s\n", s);
}

int main() {
    return yyparse();
}
