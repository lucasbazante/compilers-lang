%option noyywrap
%option yylineno

%{
	#include <string>
	#include <stdlib.h>
	#include <variant>

	#include "lr_lexer.hpp"
	#include "lr_parser.hpp"

	#define YYSTYPE TokenData
	#define YYSTYPE_IS_DECLARED 1

	extern YYSTYPE yylval;

	SymbolTable sb;

	int yycolumn = 1,
	    prev_col = 1;

	#define YY_USER_ACTION                                         \
		yycolumn = prev_col;                                   \
		for (int i = 0; i < yyleng; ++i) {                     \
			if (yytext[i] == '\t') {                       \
				int spaces = 4 - ((prev_col - 1) % 4); \
				prev_col += spaces;                    \
			} else {				       \
				prev_col += 1;			       \
			}                                              \
		}
%}

D  [0-9]
L  ([a-zA-Z])
L_ ([a-zA-Z]|[_])

%%

"//".*              { yycolumn = 1; prev_col = 1; }
"(*"(.*(\n)?)*"*)"  { yycolumn = 1; prev_col = 1; }

program   { return yytokentype::Program; }
begin     { return yytokentype::Begin; }
in        { return yytokentype::In; }
end       { return yytokentype::End; }
var       { return yytokentype::Var; }
procedure { return yytokentype::Procedure; }
struct    { return yytokentype::Struct; }
new       { return yytokentype::New; }

int    { return yytokentype::Int; }
float  { return yytokentype::Float; }
bool   { return yytokentype::Bool; }
string { return yytokentype::String; }

ref   { return yytokentype::Ref; }
deref { return yytokentype::Deref; }

"+" { return yytokentype::Plus; }
"-" { return yytokentype::Minus; }
"*" { return yytokentype::Times; }
"/" { return yytokentype::Divides; }
"^" { return yytokentype::Pow; }

"&&"  { return yytokentype::And; }
"||"  { return yytokentype::Or; }
"not" { return yytokentype::Not; }
"="   { return yytokentype::Eq; }
"<>"  { return yytokentype::Neq; }
"<="  { return yytokentype::Leq; }
"<"   { return yytokentype::Lt; }
">="  { return yytokentype::Geq; }
">"   { return yytokentype::Gt; }

if     { return yytokentype::If; }
then   { return yytokentype::Then; }
else   { return yytokentype::Else; }
fi     { return yytokentype::Fi; }
while  { return yytokentype::While; }
do     { return yytokentype::Do; }
until  { return yytokentype::Until; }
od     { return yytokentype::Od; }
for    { return yytokentype::For; }
to     { return yytokentype::To; }
step   { return yytokentype::Step; }
return { return yytokentype::Return; }

";"  { return yytokentype::Semicolon; }
":"  { return yytokentype::Colon; }
":=" { return yytokentype::Assign; }
"("  { return yytokentype::L_Paren; }
")"  { return yytokentype::R_Paren; }
"{"  { return yytokentype::L_Bracket; }
"}"  { return yytokentype::R_Bracket; }
"."  { return yytokentype::Dot; }
","  { return yytokentype::Comma; }

{D}+			    { yylval = std::stoi(yytext); return yytokentype::Int_L; }
({D}+"."{D}+)(e[+-]{D}{D})? { yylval = std::stof(yytext); return yytokentype::Float_L; }
\".\"			    { yylval = yytext; return yytokentype::String_L; }
true|false		    { yylval = yytext == "true" ? true : false; return yytokentype::Bool_L; }
null			    { return yytokentype::Null; }

{L}({L_}|{D})* { yylval = yytext; sb.insert_symbol(yytext, yytext); return yytokentype::Identifier; }

\n        { yycolumn = 1; prev_col = 1; }
.	  { }

%%
