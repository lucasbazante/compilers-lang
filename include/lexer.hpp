#ifndef LEXER_HPP
#define LEXER_HPP

#include <variant>

#include "symbol_table.hpp"

typedef std::variant<
  std::monostate, // nothing
  int,		// integers
  float,		// floats
  std::string,	// strings and identifiers
  bool		// booleans
> TokenData;

extern SymbolTable sb;
extern int yylex(void);
extern int yylineno;
extern int yycolumn;
extern char* yytext;

#endif
