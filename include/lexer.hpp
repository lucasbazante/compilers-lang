#ifndef LEXER_HPP
#define LEXER_HPP

#include <string>
#include <variant>

typedef std::variant<
  std::monostate, // nothing
  int,		// integers
  float,		// floats
  std::string,	// strings and identifiers
  bool		// booleans
> TokenData;

extern int yylex(void);
extern int yylineno;
extern int yycolumn;
extern char* yytext;

#endif
