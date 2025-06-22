#pragma once

#include <iostream>
#include <cstdlib>
#include <memory>
#include <unordered_map>
#include <string>

#include <vector>

enum class BaseType {
	FLOAT, INT, STRING, BOOL, STRUCT, REFERENCE, NONE
};

inline std::ostream& operator<<(std::ostream& os, BaseType type) {
    switch (type) {
        case BaseType::FLOAT: return os << "float";
        case BaseType::INT: return os << "int";
        case BaseType::STRING: return os << "string";
        case BaseType::BOOL: return os << "bool";
        case BaseType::STRUCT: return os << "struct";
        case BaseType::REFERENCE: return os << "reference";
        default: return os << "error";
    }
}

enum class SymbolKind {
	VARIABLE, FUNCTION, PARAMETER, STRUCT
};

inline std::ostream& operator<<(std::ostream& os, SymbolKind kind) {
    switch (kind) {
        case SymbolKind::VARIABLE: return os << "variable";
        case SymbolKind::FUNCTION: return os << "function";
        case SymbolKind::PARAMETER: return os << "parameter";
        case SymbolKind::STRUCT: return os << "struct";
        default: return os << "unknown";
    }
}

struct TypeInfo {
	BaseType type;
	std::string struct_name;
	std::shared_ptr<TypeInfo> ref_base;

	TypeInfo(BaseType type)
		: type(type)
	{}

	TypeInfo(BaseType type, std::string struct_name)
		: type(type), struct_name(struct_name)
	{}
	
	TypeInfo(BaseType type, TypeInfo ref_base)
		: type(type), ref_base(std::make_shared<TypeInfo>(ref_base))
	{}
};

struct Symbol {
	std::string name;
	SymbolKind kind;
	TypeInfo type;

	std::vector<std::pair<std::string, TypeInfo>> parameters;

	Symbol(std::string name, SymbolKind kind, TypeInfo type)
		: name(name), kind(kind), type(type)
	{}
};

class Scope {
public:
	Scope* parent;
	std::unordered_map<std::string, Symbol> table;

	Scope(Scope* parent = nullptr) : parent(parent) {}

	bool insert(const Symbol& sym) {
		return table.emplace(sym.name, sym).second;
	}

	Symbol* lookup(const std::string& name) {
		Scope* current = this;

		while (current) {
			auto it = current->table.find(name);

			if (it != current->table.end())
				return &it->second;
			
			// Move up the scope to the enclosing
			current = current->parent;
		}

		return nullptr;
	}

};

class SymbolTable {
private:
	std::vector<std::unique_ptr<Scope>> scopes;

public:
	SymbolTable() {
		push();
	}

	void push() {
		Scope* parent = scopes.empty() ? nullptr : scopes.back().get();
		scopes.push_back(std::make_unique<Scope>(parent));
	}

	void pop() {
		if (not scopes.empty())
			scopes.pop_back();
	}

	Scope* current() {
		return scopes.back().get();
	}

	bool insert(const Symbol& sym) {
		return current()->insert(sym);
	}

	Symbol* lookup(const std::string& name) {
		return current()->lookup(name);
	}
};
