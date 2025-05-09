#ifndef LEXER_HPP
#define LEXER_HPP

#include "tokens.hpp"
#include "symbol_table.hpp"

extern TokenData yylval;
extern SymbolTable sb;
extern int yylex(void);

#endif
