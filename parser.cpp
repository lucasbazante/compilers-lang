#include <cstdlib>
#include <iostream>
#include <stack>
#include <variant>

#include "parse_table.hpp"
#include "lexer.hpp"

int main() {
	std::stack<Symbol> parse_stack;
	parse_stack.push(Token::TEOF);
	parse_stack.push(Rule::S);
	
	ParseTable table = build_parse_table();

	auto current_token = yylex();

}
