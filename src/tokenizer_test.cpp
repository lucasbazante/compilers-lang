#include <cstdlib>
#include <iostream>
#include <variant>

#include "lexer.hpp"

int main() {
	int token;
	
	while ((token = yylex()) != static_cast<int>(Token::TEOF)) {
		std::cout << "[" 
			  << static_cast<Token>(token)
			  << "]"
			  << " (line: " << yylineno << ", "
			  << "column: " << yycolumn << ")";
		
		std::visit([](auto&& value) {
			using T = std::decay_t<decltype(value)>;
			if constexpr (std::is_same_v<T, std::monostate>) {
				std::cout << "";
			} else if constexpr (std::is_same_v<T, int>) {
				std::cout << " (int " << value << ")";
			} else if constexpr (std::is_same_v<T, std::string>) {
				std::cout << " (string " << value << ")";
			} else if constexpr (std::is_same_v<T, float>) {
				std::cout << " (float " << value << ")";
			} else if constexpr (std::is_same_v<T, bool>) {
				std::cout << " (boolean " << value << ")";
			} else if constexpr (std::is_same_v<T, bool>) {
				std::cout << " (cmp_type " << value << ")";
			}
		}, yylval);

		// If needed, print symbol table
		// if (token == (int)Token::Identifier) {
		// 	std::cout << '\n' << sb;
		// }

		std::cout << std::endl;
	}

	return 0;
};
