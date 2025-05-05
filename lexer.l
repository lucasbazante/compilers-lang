%{
	#include <string>
	#include <stdlib.h>

	#include "lexer.hpp"

	#define YYSTYPE TokenData
	#define YYSTYPE_IS_DECLARED 1

	YYSTYPE yylval;
	SymbolTable sb;

	int curr_line = 1;
	int curr_column = 1;
%}

%option noyywrap

D  [0-9]
L  ([a-zA-Z])
L_ ([a-zA-Z]|[_])

%%

"//".*              { curr_line++; curr_column = 1; } 
"(*"(.*(\n)?)*"*)"  { }

program   { yylval = std::monostate{}; return (int)Token::Program; }
begin     { yylval = std::monostate{}; return (int)Token::Begin; }
in        { yylval = std::monostate{}; return (int)Token::In; }
end       { yylval = std::monostate{}; return (int)Token::End; }
var       { yylval = std::monostate{}; return (int)Token::Var; }
procedure { yylval = std::monostate{}; return (int)Token::Procedure; }
struct    { yylval = std::monostate{}; return (int)Token::Struct; }
new       { yylval = std::monostate{}; return (int)Token::New; }

int    { yylval = std::monostate{}; return (int)Token::Int; }
float  { yylval = std::monostate{}; return (int)Token::Float; }
bool   { yylval = std::monostate{}; return (int)Token::Bool; }
string { yylval = std::monostate{}; return (int)Token::String; }

ref   { yylval = std::monostate{}; return (int)Token::Ref; }
deref { yylval = std::monostate{}; return (int)Token::Deref; }

"+" { yylval = std::monostate{}; return (int)Token::Plus; }
"-" { yylval = std::monostate{}; return (int)Token::Minus; }
"*" { yylval = std::monostate{}; return (int)Token::Times; }
"/" { yylval = std::monostate{}; return (int)Token::Divides; }
"^" { yylval = std::monostate{}; return (int)Token::Pow; }
"%" { yylval = std::monostate{}; return (int)Token::Modulo; }

"&&" { yylval = std::monostate{}; return (int)Token::And; }
"||" { yylval = std::monostate{}; return (int)Token::Or; }
"==" { yylval = cmp_type::Eq; return (int)Token::Comparison; }
"<>" { yylval = cmp_type::Neq; return (int)Token::Comparison; }
"<=" { yylval = cmp_type::Leq; return (int)Token::Comparison; }
"<"  { yylval = cmp_type::Less; return (int)Token::Comparison; }
">=" { yylval = cmp_type::Geq; return (int)Token::Comparison; }
">"  { yylval = cmp_type::Greater; return (int)Token::Comparison; }

if     { yylval = std::monostate{}; return (int)Token::If; }
then   { yylval = std::monostate{}; return (int)Token::Then; }
else   { yylval = std::monostate{}; return (int)Token::Else; }
fi     { yylval = std::monostate{}; return (int)Token::Fi; }
while  { yylval = std::monostate{}; return (int)Token::While; }
do     { yylval = std::monostate{}; return (int)Token::Do; }
until  { yylval = std::monostate{}; return (int)Token::Until; }
od     { yylval = std::monostate{}; return (int)Token::Od; }
for    { yylval = std::monostate{}; return (int)Token::For; }
to     { yylval = std::monostate{}; return (int)Token::To; }
step   { yylval = std::monostate{}; return (int)Token::Step; }
return { yylval = std::monostate{}; return (int)Token::Return; }

";"  { yylval = std::monostate{}; return (int)Token::Semicolon; }
":"  { yylval = std::monostate{}; return (int)Token::Colon; }
":=" { yylval = std::monostate{}; return (int)Token::Assign; }
"("  { yylval = std::monostate{}; return (int)Token::L_Paren; }
")"  { yylval = std::monostate{}; return (int)Token::R_Paren; }
"{"  { yylval = std::monostate{}; return (int)Token::L_Bracket; }
"}"  { yylval = std::monostate{}; return (int)Token::R_Bracket; }
"."  { yylval = std::monostate{}; return (int)Token::Dot; }

{D}+			    { yylval = std::stoi(yytext); return (int)Token::Int_L; }
({D}+"."{D}+)(e[+-]{D}{D})? { yylval = std::stof(yytext); return (int)Token::Float_L; }
\".\"			    { yylval = yytext; return (int)Token::String_L; }
true|false		    { yylval = yytext == "true" ? true : false; return (int)Token::Bool_L; }
null			    { yylval = std::monostate{}; return (int)Token::Null; }

{L}({L_}|{D})* { yylval = yytext; sb.insert_symbol(yytext, Token::Identifier); return (int)Token::Identifier; }

\n        { curr_line++; curr_column = 1; }
.	  { curr_column++; }
<<EOF>>   { return (int)Token::TEOF; }

%%
