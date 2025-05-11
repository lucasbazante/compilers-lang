#include <cstdlib>
#include <iostream>
#include <iterator>
#include <optional>
#include <stack>
#include <variant>

#include "parse_table.hpp"
#include "lexer.hpp"
#include "utils.hpp"

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

	log_expected_another_token_error(tk);

	return false;
}


bool match_rule(std::optional<SymbolSequence> derivation, ParseStack* stack, Rule rule, ParseTable& table) {
	if (derivation.has_value()) {
		stack->pop();
		
		for (auto it = derivation->rbegin(); it != derivation->rend(); ++it)
			stack->push(*it);

		return true;	
	}

    log_unexpected_token_error(rule, curr_token, table);

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
				return match_rule(derivation, &parse_stack, symbol, table);
			}

			return false;
		}, top);

		if (not parse_motion) return EXIT_FAILURE;
	}

	if (curr_token != Token::TEOF) {
		std::cerr << "[SYNTAX ERROR]\nInput not fully consumed!\n";
	}
}
