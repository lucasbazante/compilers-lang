#include <cstdlib>
#include <iostream>
#include <unordered_map>
#include <string>

#include "tokens.hpp"
#include <ostream>


class SymbolTable {
private:
	std::unordered_map<std::string, Token> table;

public:

	void insert_symbol(std::string name, Token token) {
		table.insert({name, token});
	}

	Token get_symbol(std::string name) {
		if (table.find(name) == table.end()) {
			std::cerr << "No symbol with given name: [" << name << "]" << std::endl;
			exit(EXIT_FAILURE);
		}
		return table[name];
	}

	friend std::ostream& operator<<(std::ostream& os, SymbolTable& sb) {

		for (auto [chave, valor] : sb.table) {
			os << " " << chave << " " << valor << '\n';
		}

		return os;
	}
};
