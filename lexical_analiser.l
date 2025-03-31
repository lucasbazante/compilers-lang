%{
int num_lines = 0, num_chars = 0, num_column = 0;
enum TokenType {
    INT,
    FLOAT,
    ID,
    STRING
};
struct TOKEN {
    enum TokenType tokenType;
    int column, row;
};
%}

D [0-9]
L ([a-zA-Z]|[_])
L2 ([a-zA-Z])

%%
"program"   { printf("PROGRAM %s\n", yytext); }
"begin"     { printf("BEGIN %s\n", yytext); }
"end"       { printf("END %s\n", yytext); }
";"         { printf("SEMICOLON %s\n", yytext); }
"var"       { printf("VAR %s\n", yytext); }
":"         { printf("COLON %s\n", yytext); }
":="        { printf("ASSIGN %s\n", yytext); }
"procedure" { printf("PROCEDURE %s\n", yytext); }
"("         { printf("LPAREN %s\n", yytext); }
")"         { printf("RPAREN %s\n", yytext); }
","         { printf("COMMA %s\n", yytext); }
"in"        { printf("IN %s\n", yytext); }
"struct"    { printf("STRUCT %s\n", yytext); }
"{"         { printf("LBRACE %s\n", yytext); }
"}"         { printf("RBRACE %s\n", yytext); }
"not"       { printf("NOT %s\n", yytext); }
"new"       { printf("NEW %s\n", yytext); }
"ref"       { printf("REF %s\n", yytext); }
"deref"     { printf("DEREF %s\n", yytext); }
"."         { printf("DOT %s\n", yytext); }
"&&"        { printf("AND %s\n", yytext); }
"||"        { printf("OR %s\n", yytext); }
"=="        { printf("EQUAL %s\n", yytext); }
"<="        { printf("LEQ %s\n", yytext); }
">="        { printf("GEQ %s\n", yytext); }
"<>"        { printf("NEQ %s\n", yytext); }
"+"         { printf("PLUS %s\n", yytext); }
"-"         { printf("MINUS %s\n", yytext); }
"*"         { printf("MULTIPLY %s\n", yytext); }
"/"         { printf("DIVIDE %s\n", yytext); }
"^"         { printf("POWER %s\n", yytext); }
"null"      { printf("NULL %s\n", yytext); }
"true"      { printf("TRUE %s\n", yytext); }
"false"     { printf("FALSE %s\n", yytext); }
"if"        { printf("IF %s\n", yytext); }
"then"      { printf("THEN %s\n", yytext); }
"else"      { printf("ELSE %s\n", yytext); }
"fi"        { printf("FI %s\n", yytext); }
"while"     { printf("WHILE %s\n", yytext); }
"do"        { printf("DO %s\n", yytext); }
"od"        { printf("OD %s\n", yytext); }
"return"    { printf("RETURN %s\n", yytext); }
"for"       { printf("FOR %s\n", yytext); }
"to"        { printf("TO %s\n", yytext); }
"step"      { printf("STEP %s\n", yytext); }
"until"     { printf("UNTIL %s\n", yytext); }
"//".*      {}
{D}+                                        {printf("INT %s\n", yytext);}
({L}({L}|{D})*{L2})|{L2}                    {printf("ID %s\n", yytext);}
({D}+[.]{D}+)|({D}+[.]{D}+)(e([+]|-){D}{D}) {printf("FLOAT %s\n", yytext);}
["].*["]                                    {printf("STRING %s\n", yytext);}
\n    ++num_lines; ++num_chars; num_column = 0;
.     ++num_chars; ++num_column;

%%
int main() {
    yylex();
    printf( "# of lines = %d, # of chars = %d\n",
            num_lines, num_chars );
}



/* recuperar mais informações:
num_chars += yyleng; num_column += yyleng;printf("%s, %d, %d, %d\n", yytext, INT, num_lines, num_column);]

posicao no codigo (linha e coluna, tipo de token, valor do token)
{struct TOKEN tk;tk.tokenType=INT;tk.column=num_column;tk.row=num_lines;return tk;}

*/
