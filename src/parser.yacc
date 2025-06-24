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
    SemanticAction* action;
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

%type <action> var_decl
%type <action> rec_decl
%type <action> exp
%type <action> var
%type <action> ref_var
%type <action> deref_var
%type <action> paramfield_decl
%type <action> paramfield_decl_list
%type <action> paramfield_decl_list_opt
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
        auto actual_type = *static_cast<Expression*>($6)->type;
        $$ = new VarDecl(&symtab, *$2, *$4, actual_type);
      }
    | Var Identifier Assign exp {
        auto exp_type = *static_cast<Expression*>($4)->type;
        $$ = new VarDecl(&symtab, *$2, exp_type);
      }
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
    Struct Identifier L_Bracket paramfield_decl_list_opt R_Bracket {
        $$ = new StructDecl(&symtab, *$2, static_cast<ParameterField*>($4));
    }
    ;

paramfield_decl_list_opt:
    /* empty */ { $$ = new ParameterField(); }
    | paramfield_decl_list { $$ = $1; }
    ;

paramfield_decl_list:
    paramfield_decl {
        auto list = new ParameterField();
        list->add(static_cast<ParameterDecl*>($1));
        $$ = list;
      }
    | paramfield_decl_list Semicolon paramfield_decl {
        auto list = static_cast<ParameterField*>($1);
        list->add(static_cast<ParameterDecl*>($3));
        $$ = list;
      }
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
    exp And exp {
          auto lhs = static_cast<Expression*>($1);
          auto rhs = static_cast<Expression*>($3);
          $$ = new Expression(lhs->type, Expression::Operator::AND, rhs->type);
      }
    | exp Or exp {
          auto lhs = static_cast<Expression*>($1);
          auto rhs = static_cast<Expression*>($3);
          $$ = new Expression(lhs->type, Expression::Operator::OR, rhs->type);
      }
    | Not exp {
          auto expr = static_cast<Expression*>($2);
          $$ = new Expression(Expression::Operator::NOT, expr->type);
      }
    | exp Lt exp {
          auto lhs = static_cast<Expression*>($1);
          auto rhs = static_cast<Expression*>($3);
          $$ = new Expression(lhs->type, Expression::Operator::LT, rhs->type);
      }
    | exp Gt exp {
          auto lhs = static_cast<Expression*>($1);
          auto rhs = static_cast<Expression*>($3);
          $$ = new Expression(lhs->type, Expression::Operator::GT, rhs->type);
      }
    | exp Leq exp {
          auto lhs = static_cast<Expression*>($1);
          auto rhs = static_cast<Expression*>($3);
          $$ = new Expression(lhs->type, Expression::Operator::LEQ, rhs->type);
      }
    | exp Geq exp {
          auto lhs = static_cast<Expression*>($1);
          auto rhs = static_cast<Expression*>($3);
          $$ = new Expression(lhs->type, Expression::Operator::GEQ, rhs->type);
      }
    | exp Eq exp {
          auto lhs = static_cast<Expression*>($1);
          auto rhs = static_cast<Expression*>($3);
          $$ = new Expression(lhs->type, Expression::Operator::EQ, rhs->type);
      }
    | exp Neq exp {
          auto lhs = static_cast<Expression*>($1);
          auto rhs = static_cast<Expression*>($3);
          $$ = new Expression(lhs->type, Expression::Operator::NEQ, rhs->type);
      }
    | Minus exp %prec UMINUS {
        $$ = new Expression(BaseType::INT);
      }
    | exp Plus exp {
          auto lhs = static_cast<Expression*>($1);
          auto rhs = static_cast<Expression*>($3);
          $$ = new Expression(lhs->type, Expression::Operator::PLUS, rhs->type);
      }
    | exp Minus exp {
          auto lhs = static_cast<Expression*>($1);
          auto rhs = static_cast<Expression*>($3);
          $$ = new Expression(lhs->type, Expression::Operator::MINUS, rhs->type);
      }
    | exp Divides exp {
          auto lhs = static_cast<Expression*>($1);
          auto rhs = static_cast<Expression*>($3);
          $$ = new Expression(lhs->type, Expression::Operator::DIVIDES, rhs->type);
      }
    | exp Times exp {
          auto lhs = static_cast<Expression*>($1);
          auto rhs = static_cast<Expression*>($3);
          $$ = new Expression(lhs->type, Expression::Operator::TIMES, rhs->type);
      }
    | exp Pow exp {
          auto lhs = static_cast<Expression*>($1);
          auto rhs = static_cast<Expression*>($3);
          $$ = new Expression(lhs->type, Expression::Operator::POW, rhs->type);
      }
    | literal             { $$ = new Expression($1); }
    | call_stmt           { $$ = new Expression(); }
    | New Identifier      { $$ = new Expression(&symtab, *$2); }
    | var                 { auto v = static_cast<Variable*>($1); $$ = new Expression(v->type); }
    | ref_var             { auto ref = static_cast<Reference*>($1); $$ = new Expression(ref->type); }
    | deref_var           { auto deref = static_cast<Dereference*>($1); $$ = new Expression(deref->type); }
    | L_Paren exp R_Paren { $$ = static_cast<Expression*>($2); }
    ;

var:
    Identifier           { $$ = new Variable(&symtab, *$1); }
    | exp Dot Identifier { $$ = new Variable(&symtab, static_cast<Expression*>($1), *$3); }
    ;

ref_var:
    Ref L_Paren var R_Paren { $$ = new Reference(static_cast<Variable*>($3)->type); }
    ;

deref_var:
    Deref L_Paren var R_Paren {
        auto v = static_cast<Variable*>($3);
        $$ = new Dereference(v->type);
      }
    | Deref L_Paren deref_var R_Paren {
        auto deref = static_cast<Dereference*>($3);
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
