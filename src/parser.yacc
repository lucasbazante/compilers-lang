%{

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "lexer.hpp"
#include "state.hpp"

void yyerror(const char *s);
int yylex(void);

State St;

%}

%define parse.error verbose
%code requires {
    #include "semantics.hpp"
}

%union {
    // Complex types: necessary actions
    VarDecl* var_decl;
    ParameterDecl* parameter_decl;
    ParameterField* parameter_field;
    StructDecl* struct_decl;
    ProcedureDecl* procedure_decl;
    Expression* expression;
    ExpressionList* expression_list;
    Variable* variable;
    Reference* reference;
    Dereference* dereference;
    Call* call;
    AssignStatement* assign_stmt;
    ForStatement* for_stmt;
    WhileStatement* while_stmt;
    DoUntilStatement* do_until_stmt;
    IfStatement* if_stmt;
    ReturnStatement* return_stmt;
    Statement* stmt;
    StatementList* stmt_list;

    // Less complicated types: no action besides the basic
    std::string* name;
    TypeInfo* type;
}

%token Int Float Bool String
%token Program Begin In End Var Procedure Struct New
%token Ref Deref Int_L Float_L Bool_L String_L Null
%token If Then Else Fi While Do Until Od For To Step Return
%token Semicolon Colon Assign L_Paren R_Paren L_Bracket R_Bracket Comma Dot
%token <name> Identifier
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
%left UMINUS

%type <var_decl> var_decl
%type <parameter_decl> paramfield_decl
%type <parameter_field> paramfield_decl_list
%type <parameter_field> paramfield_decl_list_opt
%type <parameter_field> paramfield_list
%type <parameter_field> paramfield_list_opt
%type <procedure_decl> proc_decl_signature
%type <struct_decl> rec_decl

%type <expression> exp
%type <expression_list> exp_list
%type <expression_list> exp_list_opt
%type <variable> var
%type <reference> ref_var
%type <dereference> deref_var

%type <call> call_stmt
%type <assign_stmt> assign_stmt
%type <for_stmt> for_stmt
%type <while_stmt> while_stmt
%type <do_until_stmt> do_until_stmt
%type <if_stmt> if_stmt
%type <stmt_list> else_opt
%type <return_stmt> return_stmt
%type <stmt_list> proc_body
%type <stmt_list> stmt_list
%type <stmt> stmt

%type <type> type
%type <type> literal
%type <type> return_type_opt

%start program

%%

program:
    Program Identifier Begin {
    } decl_list_opt End {
        std::cout << St.Output();
    }
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
    Var Identifier Colon type {
        $$ = new VarDecl(&St, *$2, $4);
        $$->Generate(&St);
      }
    | Var Identifier Colon type Assign exp {
        $$ = new VarDecl(&St, *$2, $4, $6);
        $$->Generate(&St);
      }
    | Var Identifier Assign exp {
        $$ = new VarDecl(&St, *$2, $4);
        $$->Generate(&St);
      }
    ;

proc_decl:
    proc_decl_signature Begin proc_body End {
        $3->verify_return(&St, $1);
        St.Table()->pop();
        $3->Generate(&St);
      }
    ;

proc_decl_signature:
    Procedure Identifier L_Paren paramfield_list_opt R_Paren return_type_opt {
        $$ = new ProcedureDecl(&St, *$2, $4, $6);
        St.Table()->push(*$2);
        $$->declare_params_in_scope(&St, $4);
      }
    ;

proc_body:
    decl_block_opt stmt_list {
        $$ = $2;
      }
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
    Struct Identifier L_Bracket paramfield_decl_list_opt R_Bracket {
        $$ = new StructDecl(&St, *$2, $4);
        $$->Generate(&St);
    }
    ;

paramfield_decl_list_opt:
    /* empty */ { $$ = new ParameterField(); }
    | paramfield_decl_list { $$ = $1; }
    ;

paramfield_decl_list:
    paramfield_decl {
        auto list = new ParameterField();
        list->add($1);
        $$ = list;
      }
    | paramfield_decl_list Semicolon paramfield_decl {
        auto list = $1;
        list->add($3);
        $$ = list;
      }
    ;

paramfield_list_opt:
    /* empty */ {
        $$ = new ParameterField();
      }
    | paramfield_list {
        $$ = $1;
      }
    ;

paramfield_list:
    paramfield_decl {
        $$ = new ParameterField();
        $$->add($1);
      }
    | paramfield_list Comma paramfield_decl {
        $1->add($3);
        $$ = $1;
      }
    ;

paramfield_decl:
    Identifier Colon type { $$ = new ParameterDecl(*$1, $3); }
    ;

stmt_list:
    /* empty */ {
        $$ = new StatementList();
      }
    | stmt {
        $$ = new StatementList();
        $$->add(&St, $1);
      }
    | stmt_list Semicolon stmt {
        $1->add(&St, $3);
        $$ = $1;
      }
    ;

stmt:
    assign_stmt     { $$ = $1; }
    | if_stmt       { $$ = $1; }
    | while_stmt    { $$ = $1; }
    | for_stmt      { $$ = $1; }
    | do_until_stmt { $$ = $1; }
    | return_stmt   { $$ = $1; }
    | call_stmt     { $$ = $1; }
    ;

assign_stmt:
    var Assign exp {
        $$ = new AssignStatement(&St, $1, $3);
      }
    | deref_var Assign exp {
        $$ = new AssignStatement(&St, $1, $3);
      }
    ;

if_stmt:
    If exp Then stmt_list else_opt Fi {
        $$ = new IfStatement(&St, $2, $4, $5);
      }
    ;

else_opt:
    /* empty */      { $$ = new StatementList(); }
    | Else stmt_list { $$ = $2; }
    ;

while_stmt:
    While exp Do stmt_list Od {
        $$ = new WhileStatement(&St, $2, $4);
      }
    ;

for_stmt:
    For Identifier Eq exp To exp Step exp Do stmt_list Od { $$ = new ForStatement(&St, *$2, $4, $6, $8, $10); }
    ;

do_until_stmt:
    Do stmt_list Until exp Od { $$ = new DoUntilStatement(&St, $4, $2); }
    ;

return_stmt:
    Return       { $$ = new ReturnStatement(); }
    | Return exp { $$ = new ReturnStatement(&St, $2); }
    ;

call_stmt:
    Identifier L_Paren exp_list_opt R_Paren { $$ = new Call(&St, *$1, $3); }
    ;

exp_list_opt:
    /* empty */ { $$ = new ExpressionList(); }
    | exp_list { $$ = $1; }
    ;

exp_list:
    exp {
        auto list = new ExpressionList();
        list->add($1);
        $$ = list;
      }
    | exp_list Comma exp {
        auto list = $1;
        list->add($3);
        $$ = list;
      }
    ;

exp:
    exp And exp {
          auto lhs = $1;
          auto rhs = $3;
          $$ = new Expression(lhs, Expression::Operator::AND, rhs);
      }
    | exp Or exp {
          auto lhs = $1;
          auto rhs = $3;
          $$ = new Expression(lhs, Expression::Operator::OR, rhs);
      }
    | Not exp {
          auto expr = $2;
          $$ = new Expression(Expression::Operator::NOT, expr);
      }
    | exp Lt exp {
          auto lhs = $1;
          auto rhs = $3;
          $$ = new Expression(lhs, Expression::Operator::LT, rhs);
      }
    | exp Gt exp {
          auto lhs = $1;
          auto rhs = $3;
          $$ = new Expression(lhs, Expression::Operator::GT, rhs);
      }
    | exp Leq exp {
          auto lhs = $1;
          auto rhs = $3;
          $$ = new Expression(lhs, Expression::Operator::LEQ, rhs);
      }
    | exp Geq exp {
          auto lhs = $1;
          auto rhs = $3;
          $$ = new Expression(lhs, Expression::Operator::GEQ, rhs);
      }
    | exp Eq exp {
          auto lhs = $1;
          auto rhs = $3;
          $$ = new Expression(lhs, Expression::Operator::EQ, rhs);
      }
    | exp Neq exp {
          auto lhs = $1;
          auto rhs = $3;
          $$ = new Expression(lhs, Expression::Operator::NEQ, rhs);
      }
    | Minus exp %prec UMINUS {
        auto expr = $2;
        $$ = new Expression(Expression::Operator::NEGATE, expr);
      }
    | exp Plus exp {
          auto lhs = $1;
          auto rhs = $3;
          $$ = new Expression(lhs, Expression::Operator::PLUS, rhs);
      }
    | exp Minus exp {
          auto lhs = $1;
          auto rhs = $3;
          $$ = new Expression(lhs, Expression::Operator::MINUS, rhs);
      }
    | exp Divides exp {
          auto lhs = $1;
          auto rhs = $3;
          $$ = new Expression(lhs, Expression::Operator::DIVIDES, rhs);
      }
    | exp Times exp {
          auto lhs = $1;
          auto rhs = $3;
          $$ = new Expression(lhs, Expression::Operator::TIMES, rhs);
      }
    | exp Pow exp {
          auto lhs = $1;
          auto rhs = $3;
          $$ = new Expression(lhs, Expression::Operator::POW, rhs);
      } 
    | call_stmt {
        auto call = $1;
        $$ = new Expression(call->type, "call");
      }
    | var {
        $$ = new Expression($1);
      }
    | ref_var {
        $$ = new Expression($1);
      }
    | deref_var {
        $$ = new Expression($1);
      }
    | literal             { $$ = new Expression($1, yytext); }
    | New Identifier      { $$ = new Expression(&St, *$2); }
    | L_Paren exp R_Paren { $$ = $2; }
    ;

var:
    Identifier           { $$ = new Variable(&St, *$1); }
    | exp Dot Identifier { $$ = new Variable(&St, $1, *$3); }
    ;

ref_var:
    Ref L_Paren var R_Paren { $$ = new Reference(&St, $3); }
    ;

deref_var:
    Deref L_Paren var R_Paren {
        auto var = $3;
        $$ = new Dereference(&St, var);
      }
    | Deref L_Paren deref_var R_Paren {
        auto deref = $3;
        $$ = new Dereference(&St, deref);
    }
    ;

literal:
    Int_L      { $$ = new TypeInfo(BaseType::INT); }
    | Float_L  { $$ = new TypeInfo(BaseType::FLOAT); }
    | String_L { $$ = new TypeInfo(BaseType::STRING); }
    | Bool_L   { $$ = new TypeInfo(BaseType::BOOL); }
    | Null     { $$ = new TypeInfo(BaseType::REFERENCE); }
    ;

type:
    Int                        { $$ = new TypeInfo(BaseType::INT); }
    | Float                    { $$ = new TypeInfo(BaseType::FLOAT); }
    | String                   { $$ = new TypeInfo(BaseType::STRING); }
    | Bool                     { $$ = new TypeInfo(BaseType::BOOL); }
    | Identifier               { $$ = new TypeInfo(BaseType::STRUCT, *$1); }
    | Ref L_Paren type R_Paren { $$ = new TypeInfo(BaseType::REFERENCE, *$3); }
    ;

return_type_opt:
    /* empty */  { $$ = new TypeInfo(BaseType::NONE); }
    | Colon type { $$ = $2; }
    ;

%%

int main(void) {
  return yyparse();
}

void yyerror(const char *msg) {
    fprintf(stderr, "Syntax error: %s at '%s' (line %d, column %d)\n",
            msg, yytext, yylineno, yycolumn);
}
