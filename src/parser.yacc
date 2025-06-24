%{

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

#include "lexer.hpp"
#include "symbol_table.hpp"

void yyerror(const char *s);
int yylex(void);

SymbolTable symtab;

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

%type <type> return_type_opt
%type <type> type
%type <type> literal

%start program

%%

program:
    Program Identifier Begin {
        symtab = SymbolTable();
    } decl_list_opt End {
        symtab.pop();
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
    Var Identifier Colon type { $$ = new VarDecl(&symtab, *$2, *$4); }
    | Var Identifier Colon type Assign exp {
        auto actual_type = *$6->type;
        $$ = new VarDecl(&symtab, *$2, *$4, actual_type);
      }
    | Var Identifier Assign exp {
        auto exp_type = *$4->type;
        $$ = new VarDecl(&symtab, *$2, exp_type);
      }
    ;

proc_decl:
    proc_decl_signature Begin proc_body End {
        symtab.pop();
      }
    ;

proc_decl_signature:
    Procedure Identifier L_Paren paramfield_list_opt R_Paren return_type_opt {
        auto name = *$2;
        auto params = $4;
        auto return_type = $6;

        $$ = new ProcedureDecl(&symtab, name, params, return_type);
        symtab.push();
        $$->declare_params_in_scope(&symtab, params);
      }
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
    Struct Identifier L_Bracket paramfield_decl_list_opt R_Bracket {
        $$ = new StructDecl(&symtab, *$2, $4);
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
        auto list = new ParameterField();
        list->add($1);
        $$ = list;
      }
    | paramfield_list Comma paramfield_decl {
        auto list = $1;
        list->add($3);
        $$ = list;
      }
    ;

paramfield_decl:
    Identifier Colon type { $$ = new ParameterDecl(*$1, $3); }
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
    var Assign exp { $$ = new AssignStatement($1, $3); }
    | deref_var Assign exp { $$ = new AssignStatement($1, $3); }
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
    For Identifier Eq exp To exp Step exp Do stmt_list Od { $$ = new ForStatement(&symtab, *$2, $4, $6, $8); }
    ;

do_until_stmt:
    Do stmt_list Until exp Od
    ;

return_stmt:
    Return
    | Return exp
    ;

call_stmt:
    Identifier L_Paren exp_list_opt R_Paren { $$ = new Call(&symtab, *$1, $3); }
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
          $$ = new Expression(lhs->type, Expression::Operator::AND, rhs->type);
      }
    | exp Or exp {
          auto lhs = $1;
          auto rhs = $3;
          $$ = new Expression(lhs->type, Expression::Operator::OR, rhs->type);
      }
    | Not exp {
          auto expr = $2;
          $$ = new Expression(Expression::Operator::NOT, expr->type);
      }
    | exp Lt exp {
          auto lhs = $1;
          auto rhs = $3;
          $$ = new Expression(lhs->type, Expression::Operator::LT, rhs->type);
      }
    | exp Gt exp {
          auto lhs = $1;
          auto rhs = $3;
          $$ = new Expression(lhs->type, Expression::Operator::GT, rhs->type);
      }
    | exp Leq exp {
          auto lhs = $1;
          auto rhs = $3;
          $$ = new Expression(lhs->type, Expression::Operator::LEQ, rhs->type);
      }
    | exp Geq exp {
          auto lhs = $1;
          auto rhs = $3;
          $$ = new Expression(lhs->type, Expression::Operator::GEQ, rhs->type);
      }
    | exp Eq exp {
          auto lhs = $1;
          auto rhs = $3;
          $$ = new Expression(lhs->type, Expression::Operator::EQ, rhs->type);
      }
    | exp Neq exp {
          auto lhs = $1;
          auto rhs = $3;
          $$ = new Expression(lhs->type, Expression::Operator::NEQ, rhs->type);
      }
    | Minus exp %prec UMINUS {
        auto expr = $2;
        $$ = new Expression(Expression::Operator::NEGATE, expr->type);
      }
    | exp Plus exp {
          auto lhs = $1;
          auto rhs = $3;
          $$ = new Expression(lhs->type, Expression::Operator::PLUS, rhs->type);
      }
    | exp Minus exp {
          auto lhs = $1;
          auto rhs = $3;
          $$ = new Expression(lhs->type, Expression::Operator::MINUS, rhs->type);
      }
    | exp Divides exp {
          auto lhs = $1;
          auto rhs = $3;
          $$ = new Expression(lhs->type, Expression::Operator::DIVIDES, rhs->type);
      }
    | exp Times exp {
          auto lhs = $1;
          auto rhs = $3;
          $$ = new Expression(lhs->type, Expression::Operator::TIMES, rhs->type);
      }
    | exp Pow exp {
          auto lhs = $1;
          auto rhs = $3;
          $$ = new Expression(lhs->type, Expression::Operator::POW, rhs->type);
      }
    | literal             { $$ = new Expression($1); }
    | call_stmt           { auto call = $1; $$ = new Expression(call->type); }
    | New Identifier      { $$ = new Expression(&symtab, *$2); }
    | var                 { auto v = $1; $$ = new Expression(v->type); }
    | ref_var             { auto ref = $1; $$ = new Expression(ref->type); }
    | deref_var           { auto deref = $1; $$ = new Expression(deref->type); }
    | L_Paren exp R_Paren { $$ = $2; }
    ;

var:
    Identifier           { $$ = new Variable(&symtab, *$1); }
    | exp Dot Identifier { $$ = new Variable(&symtab, $1, *$3); }
    ;

ref_var:
    Ref L_Paren var R_Paren { $$ = new Reference($3->type); }
    ;

deref_var:
    Deref L_Paren var R_Paren {
        auto v = $3;
        $$ = new Dereference(v->type);
      }
    | Deref L_Paren deref_var R_Paren {
        auto deref = $3;
        $$ = new Dereference(deref->type);
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
