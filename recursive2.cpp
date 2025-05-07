#include <cstdlib>
#include <iostream>
#include <variant>
#include <string>

#include "lexer.hpp"
#include "recursive_util2.hpp"

bool recursive_parsing(std::string back = "") {

	if (RecursiveParsing::token == Token::TEOF) {
		return true;
	}

	bool something_was_accepted = false;
	std::vector<Rule> rules = RecursiveParsing::get_state();

	if (rules.size() > 1) {
		std::cout << back << "Trying " << rules.size() << 
			" possible rules for non-terminal <" << (int) rules[0].lhs << ">\n";
		bool accepted_rule = false;

		int i = 0;
		for (Rule possible_rule : rules) {

			std::cout << back << "Trying rule: " << rule_to_string(possible_rule) << '\n';
			RecursiveParsing::save_state(possible_rule);

			if (recursive_parsing(back + "--" + std::to_string(i) + "--")) {
				std::cout << back << "Accepted rule\n";
				accepted_rule = true;
				something_was_accepted = true;
				break;
			}
			i ++;
		}

		if (!accepted_rule) {
			std::cout << back << "No rule for the non-terminal <" << 
				(int) rules[0].lhs << "> was satisfactory" << "\n";
			return false;
		}
		else {
			return true;
		}

	} else if (rules.size() == 1) {
		Rule rule = rules[0];
		for (Symbol symbol : rule.rhs) {

			if (is_terminal(symbol)) {

				if (symbol == Symbol::EPSILON) {
					return true;
				}
				
				if (symbol_to_string.at(symbol) == token_to_string(RecursiveParsing::token)) {
					std::cout << back << "Accepted: " << RecursiveParsing::token << '\n';
					RecursiveParsing::token = get_next_token();
					std::cout << back << "next token is " << RecursiveParsing::token << '\n';
					something_was_accepted = true;
				}
				else {
					std::cout << back << "Symbol <" << 
						symbol_to_string.at(symbol) << "> didn't match token <" << RecursiveParsing::token << ">\n";
					return false;
				}
			}
			else {
				RecursiveParsing::save_state(symbol, rule, symbol_to_rules(symbol));
				return true;
			}
		}

		if (!something_was_accepted) {
			std::cout << back << "ERROR: I finished all the rules but there is still input left" << "\n";
			return false;
		}
		else {
			return true;
		}
	}
}

int main() {

	std::cout << "STARTING SYNTACTIC ANALYSIS\n\n";
	RecursiveParsing::start();

	while (!RecursiveParsing::eof()) {
		std::cout << "call\n";
		if (recursive_parsing()) {
		}
		else {
			std::cout << "NO :(...The input was not accepted syntactically for the reasons above.\n";
		}
	}

	if (RecursiveParsing::eof()) {
		std::cout << "CONGRATS! Your input was accepted syntactically!\n";
	}

	return 0;
};
