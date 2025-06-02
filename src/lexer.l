%{
	#include <string>
	#include <stdlib.h>

	#include "lexer.hpp"
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

%option noyywrap
%option yylineno

D  [0-9]
L  ([a-zA-Z])
L_ ([a-zA-Z]|[_])

%%

"//".*              { yycolumn = 1; prev_col = 1; }
"(*"(.*(\n)?)*"*)"  { yycolumn = 1; prev_col = 1; }

program   { yylval = std::monostate{}; return static_cast<int>(Token::Program); }
begin     { yylval = std::monostate{}; return static_cast<int>(Token::Begin); }
in        { yylval = std::monostate{}; return static_cast<int>(Token::In); }
end       { yylval = std::monostate{}; return static_cast<int>(Token::End); }
var       { yylval = std::monostate{}; return static_cast<int>(Token::Var); }
procedure { yylval = std::monostate{}; return static_cast<int>(Token::Procedure); }
struct    { yylval = std::monostate{}; return static_cast<int>(Token::Struct); }
new       { yylval = std::monostate{}; return static_cast<int>(Token::New); }

int    { yylval = std::monostate{}; return static_cast<int>(Token::Int); }
float  { yylval = std::monostate{}; return static_cast<int>(Token::Float); }
bool   { yylval = std::monostate{}; return static_cast<int>(Token::Bool); }
string { yylval = std::monostate{}; return static_cast<int>(Token::String); }

ref   { yylval = std::monostate{}; return static_cast<int>(Token::Ref); }
deref { yylval = std::monostate{}; return static_cast<int>(Token::Deref); }

"+" { yylval = std::monostate{}; return static_cast<int>(Token::Plus); }
"-" { yylval = std::monostate{}; return static_cast<int>(Token::Minus); }
"*" { yylval = std::monostate{}; return static_cast<int>(Token::Times); }
"/" { yylval = std::monostate{}; return static_cast<int>(Token::Divides); }
"^" { yylval = std::monostate{}; return static_cast<int>(Token::Pow); }
"%" { yylval = std::monostate{}; return static_cast<int>(Token::Modulo); }

"&&" { yylval = std::monostate{}; return static_cast<int>(Token::And); }
"||" { yylval = std::monostate{}; return static_cast<int>(Token::Or); }
"="  { yylval = cmp_type::Eq; return static_cast<int>(Token::Comparison); }
"<>" { yylval = cmp_type::Neq; return static_cast<int>(Token::Comparison); }
"<=" { yylval = cmp_type::Leq; return static_cast<int>(Token::Comparison); }
"<"  { yylval = cmp_type::Less; return static_cast<int>(Token::Comparison); }
">=" { yylval = cmp_type::Geq; return static_cast<int>(Token::Comparison); }
">"  { yylval = cmp_type::Greater; return static_cast<int>(Token::Comparison); }

if     { yylval = std::monostate{}; return static_cast<int>(Token::If); }
then   { yylval = std::monostate{}; return static_cast<int>(Token::Then); }
else   { yylval = std::monostate{}; return static_cast<int>(Token::Else); }
fi     { yylval = std::monostate{}; return static_cast<int>(Token::Fi); }
while  { yylval = std::monostate{}; return static_cast<int>(Token::While); }
do     { yylval = std::monostate{}; return static_cast<int>(Token::Do); }
until  { yylval = std::monostate{}; return static_cast<int>(Token::Until); }
od     { yylval = std::monostate{}; return static_cast<int>(Token::Od); }
for    { yylval = std::monostate{}; return static_cast<int>(Token::For); }
to     { yylval = std::monostate{}; return static_cast<int>(Token::To); }
step   { yylval = std::monostate{}; return static_cast<int>(Token::Step); }
return { yylval = std::monostate{}; return static_cast<int>(Token::Return); }

";"  { yylval = std::monostate{}; return static_cast<int>(Token::Semicolon); }
":"  { yylval = std::monostate{}; return static_cast<int>(Token::Colon); }
":=" { yylval = std::monostate{}; return static_cast<int>(Token::Assign); }
"("  { yylval = std::monostate{}; return static_cast<int>(Token::L_Paren); }
")"  { yylval = std::monostate{}; return static_cast<int>(Token::R_Paren); }
"{"  { yylval = std::monostate{}; return static_cast<int>(Token::L_Bracket); }
"}"  { yylval = std::monostate{}; return static_cast<int>(Token::R_Bracket); }
"."  { yylval = std::monostate{}; return static_cast<int>(Token::Dot); }
","  { yylval = std::monostate{}; return static_cast<int>(Token::Comma); }

{D}+			    { yylval = std::stoi(yytext); return static_cast<int>(Token::Int_L); }
({D}+"."{D}+)(e[+-]{D}{D})? { yylval = std::stof(yytext); return static_cast<int>(Token::Float_L); }
\".\"			    { yylval = yytext; return static_cast<int>(Token::String_L); }
true|false		    { yylval = yytext == "true" ? true : false; return static_cast<int>(Token::Bool_L); }
null			    { yylval = std::monostate{}; return static_cast<int>(Token::Null); }

{L}({L_}|{D})* { yylval = yytext; sb.insert_symbol(yytext, Token::Identifier); return static_cast<int>(Token::Identifier); }

\n        { yycolumn = 1; prev_col = 1; }
.	  { }

%%
