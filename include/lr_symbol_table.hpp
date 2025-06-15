#include <cstdlib>
#include <iostream>
#include <unordered_map>
#include <string>

#include <ostream>

class SymbolTable {
private:
	std::unordered_map<std::string, std::string> table;

public:

	void insert_symbol(std::string name, std::string token) {
		table.insert({name, token});
	}

	std::string get_symbol(std::string name) {
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
