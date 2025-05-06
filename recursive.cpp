#include <cstdlib>
#include <iostream>
#include <variant>
#include <string>

#include "lexer.hpp"
#include "recursive_util.hpp"

Token get_next_token() {
	return static_cast<Token>(yylex());
}

bool recursive_parsing(Rule rule, Token * token, std::string back) {

	if (*token == Token::TEOF) {
		return true;
	}

	bool something_was_accepted = false;

	for (Symbol symbol : rule.rhs) {

		if (is_terminal(symbol)) {

			if (symbol == Symbol::EPSILON) {
				return true;
			}
			
			if (symbol_to_string.at(symbol) == token_to_string(*token)) {
				std::cout << back << "Accepted: " << *token << '\n';
				*token = get_next_token();
				std::cout << back << "next token is " << *token << '\n';
				something_was_accepted = true;
			}
			else {
				std::cout << back << "Symbol <" << 
					symbol_to_string.at(symbol) << "> didn't match token <" << *token << ">\n";
				return false;
			}
		}
		else {
			std::vector<Rule> possible_rules = symbol_to_rules(symbol);
			std::cout << back << "Trying " << possible_rules.size() << 
				" possible rules for non-terminal <" << (int) symbol << ">\n";
			bool accepted_rule = false;

			int i = 0;
			for (Rule possible_rule : possible_rules) {

				std::cout << back << "Trying rule: " << rule_to_string(possible_rule) << '\n';

				if (recursive_parsing(possible_rule, token, back + "--" + std::to_string(i) + "--")) {
					std::cout << back << "Accepted rule\n";
					accepted_rule = true;
					something_was_accepted = true;
					break;
				}
				i ++;
			}

			if (!accepted_rule) {
				std::cout << back << "No rule for the non-terminal <" << 
					(int) symbol << "> was satisfactory" << "\n";
				return false;
			}
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

int main() {

	std::cout << "STARTING SYNTACTIC ANALYSIS\n\n";

	Token token = get_next_token();
	if (recursive_parsing(grammar[0], &token, "")) {
		std::cout << "CONGRATS! Your input was accepted syntactically!\n";
	}
	else {
		std::cout << "NO :(...The input was not accepted syntactically for the reasons above.\n";
	}

	return 0;
};
