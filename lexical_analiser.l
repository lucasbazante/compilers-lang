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
L [a-zA-Z]

%%
{D}+  {num_chars += yyleng; num_column += yyleng;printf("%s, %d, %d, %d\n", yytext, INT, num_lines, num_column);}
\n    ++num_lines; ++num_chars; num_column = 0;
.     ++num_chars; ++num_column;

%%
int main() {
    yylex();
    printf( "# of lines = %d, # of chars = %d",
            num_lines, num_chars );
}

/* posicao no codigo (linha e coluna, tipo de token, valor do token)
 {struct TOKEN tk;tk.tokenType=INT;tk.column=num_column;tk.row=num_lines;return tk;}
 */