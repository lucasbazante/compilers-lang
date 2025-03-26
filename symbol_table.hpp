#include <cstdlib>
#include <iostream>
#include <unordered_map>
#include <string>

#include "tokens.hpp"


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
};
