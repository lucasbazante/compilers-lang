#include <cstdlib>
#include <iostream>
#include <sstream>
#include <iterator>
#include <optional>
#include <stack>
#include <variant>

#include "parse_table.hpp"
#include "lexer.hpp"

using ParseStack = std::stack<Symbol>;

Token curr_token;

void next_token() {
	curr_token = static_cast<Token>(yylex());
}

bool match(Token tk, ParseStack* stack) {
	if (tk == curr_token) {
		stack->pop();
		next_token();
		return true;
	}

	std::cerr << "[SYNTAX ERROR]\nExpected token "
		  << tk
		  << " but got "
		  << curr_token << ".\n";

	return false;
}


bool match_rule(std::optional<SymbolSequence> derivation, ParseStack* stack) {
	if (derivation.has_value()) {
		stack->pop();
		
		for (auto it = derivation->rbegin(); it != derivation->rend(); ++it)
			stack->push(*it);

		return true;	
	}

	std::cerr << "[SYNTAX ERROR]\nUnexpected token "
		  << curr_token
		  << ".\n";

	return false;
}

int main() {
	ParseTable table = build_parse_table();

	ParseStack parse_stack;
	parse_stack.push(Rule::S);
	
	next_token();
	while (not parse_stack.empty()) {
		Symbol top = parse_stack.top();

		bool parse_motion = std::visit([&](auto&& symbol) -> bool {
			using T = std::decay_t<decltype(symbol)>;
			
			if constexpr (std::is_same_v<T, Token>) {
				return match(symbol, &parse_stack);
				
			} else if constexpr (std::is_same_v<T, Rule>) {
				auto derivation = get_derivation(symbol, curr_token, table);
				return match_rule(derivation, &parse_stack);
			}

			return false;
		}, top);

		if (not parse_motion) return EXIT_FAILURE;
	}

	if (curr_token != Token::TEOF) {
		std::cerr << "[SYNTAX ERROR]\nInput not fully consumed!\n";
	}
}
