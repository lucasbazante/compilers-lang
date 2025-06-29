%option noyywrap
%option yylineno

%{
	#include <stdlib.h>

	#include "lexer.hpp"
	#include "parser.hpp"

	// #define YYSTYPE TokenData
	// #define YYSTYPE_IS_DECLARED 1

	// extern YYSTYPE yylval;

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

{D}+			    { return yytokentype::Int_L; }
({D}+"."{D}+)(e[+-]{D}{D})? { return yytokentype::Float_L; }
\".*\"			    { return yytokentype::String_L; }
true|false		    { return yytokentype::Bool_L; }
null			    { return yytokentype::Null; }

{L}({L_}|{D})* { yylval.name = new std::string(yytext); return yytokentype::Identifier; }


[ \t\r\b]+ { }
\n	   { yycolumn = 1; prev_col = 1; }

. { char c = yytext[0];
    if (isprint(c)) {
	std::cerr << "Lexical error: invalid character '" << c
		  << "' at line " << yylineno
		  << ", column " << yycolumn
		  << ".\n";
    } else {
	std::cerr << "Lexical error: invalid character (ASCII "
		  << (int)(unsigned char)c
		  << ") at line " << yylineno
		  << ", column " << yycolumn
		  << ".\n";
    }
  }

%%
