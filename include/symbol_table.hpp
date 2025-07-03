#pragma once

#include <iostream>
#include <memory>
#include <ostream>
#include <sstream>
#include <unordered_map>
#include <string>

#include <vector>

/*
 * A simple enum to use in simple type checks. It works
 * whenever we don't have nested information to check,
 * like comparing primitive types.
 */
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
        case BaseType::REFERENCE: return os << "ref";
	case BaseType::NONE: return os << "none";
        default: return os << "error";
    }
}

inline std::string BaseType_toString(BaseType b_type) {
	switch (b_type) {
		case BaseType::FLOAT: return "float";
		case BaseType::INT: return "int";
		case BaseType::STRING: return "std::string";
		case BaseType::BOOL: return "bool";
		default: return "";
	}
}

/*
 * Each symbol must be differentiated regarding what it "points to".
 * Symbols of variables are different than symbols of functions, and depending
 * on the context, one might be accepted while the other might not.
 */
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

/*
 * This contains all the information we need to fully represent every possible type
 * in our programming language.
 *
 * The `b_type` indicates, at a primitive level, what is the type of what
 * we're dealing with. It suffices to simple checks and comparisons, but it
 * isn't enough to fully compare structs and references.
 *
 * The `struct_name` is a field that is only set when the type is indeed a struct.
 * It helps differentiating between structs.
 *
 * The `ref_base` shared pointer lets us represent the type of references. It points
 * to a type information about the base of the reference. For example, `ref(int)`
 * will set the pointer to a type information about `int`.
 */
struct TypeInfo {
	BaseType b_type;
	std::string struct_name;
	std::shared_ptr<TypeInfo> ref_base;

	/*
	 * Basic constructor, represents primitive information.
	*/
	TypeInfo(BaseType b_type)
		: b_type(b_type)
	{}

	/*
	 * Constructor for representing structs.
	*/
	TypeInfo(BaseType b_type, std::string struct_name)
		: b_type(b_type), struct_name(struct_name)
	{}
	
	/*
	 * Constructor for representing references.
	*/
	TypeInfo(BaseType b_type, TypeInfo ref_base)
		: b_type(b_type), ref_base(std::make_shared<TypeInfo>(ref_base))
	{}
	
	/*
	 * Copy constructor, creating another identical.
	 * We'll use this one a lot on the code.
	*/
	TypeInfo(const TypeInfo& other)
		: b_type(other.b_type),
		  struct_name(other.struct_name),
		  ref_base(other.ref_base ? std::make_shared<TypeInfo>(*other.ref_base) : nullptr)
	{}

	std::string Gen() {
		std::ostringstream gen;
		if (this->b_type == BaseType::REFERENCE)
			gen << this->ref_base->Gen() << "*";
		else if (this->b_type == BaseType::STRUCT)
			gen << this->struct_name;
		else
			gen << BaseType_toString(this->b_type);

		return gen.str();
	}
};

inline std::ostream& operator<<(std::ostream& os, const TypeInfo& type) {
    if (type.b_type == BaseType::REFERENCE) {
        os << "ref(";
        if (type.ref_base)
            os << *type.ref_base;
        else
            os << "???";
        os << ")";
    }
    else if (type.b_type == BaseType::STRUCT) {
        os << "struct(" << type.struct_name << ")";
    }
    else {
        os << type.b_type;
    }

    return os;
}

/*
 * This is so we can compare types at the deepest level.
 * It compares recursively into references so we can really check if they're equal.
*/
inline bool operator==(const TypeInfo& lhs, const TypeInfo& rhs) {
    if (lhs.b_type != rhs.b_type)
        return false;

    // If struct, compare struct names
    if (lhs.b_type == BaseType::STRUCT && lhs.struct_name != rhs.struct_name)
        return false;

    // If reference, compare the base types recursively
    if (lhs.b_type == BaseType::REFERENCE) {
        if (!lhs.ref_base || !rhs.ref_base)
            return false; // mismatched presence of base

        return *lhs.ref_base == *rhs.ref_base; // recursive comparison
    }

    return true;
}

inline bool operator!=(const TypeInfo& lhs, const TypeInfo& rhs) {
    return !(lhs == rhs);
}

/*
 * This is what we store as symbols of our symbol table.
 *
 * Each symbol has a name, a kind and a type.
 *
 * The attribute `parameters` is a vector only populated in the case
 * of structs (its fields) and functions (its formal parameters).
 * It tells us what are the valid parameters we can use in both of these kind of symbols.
*/
struct Symbol {
	std::string name;
	SymbolKind kind;
	TypeInfo type;

	std::vector<std::pair<std::string, TypeInfo>> parameters;

	Symbol(std::string name, SymbolKind kind, TypeInfo type)
		: name(name), kind(kind), type(type)
	{}
};

/*
 * This is a class to represent a scope block in our symbol table.
 *
 * The attribute `name` refers to what procedure the scope refers to.
 * We use this to properly set up an error log.
 *
 * The attribute `parent` is a pointer to another scope, the enclosing
 * scope. Thanks to this, we can access variables defined in enclosing
 * scopes.
 *
 * The attribute `table` is a symbol table belonging to this scope, associating
 * names with symbols.
*/
class Scope {
public:
	std::string name;
	Scope* parent;
	std::unordered_map<std::string, Symbol> table;

	/*
	 * Default constructor, the name is set to program and it has no parent.
	*/
	Scope(Scope* parent = nullptr, std::string name = "program") : parent(parent), name(name) {}

	/*
	 * Inserting a symbol into the table.
	*/
	bool insert(const Symbol& sym) {
		return table.emplace(sym.name, sym).second;
	}

	/*
	 * Look for a symbol in the table.
	 * If it's not present in the current scope, search its parent.
	*/
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

/*
 * This is the symbol table. All in all, its just a collection of scopes.
 *
 * It controls the access to the scopes, and certifies that we'll be declaring
 * symbols in the correct scope being analyzed, as well as search symbols using the
 * current and parent logic defined above.
*/
class SymbolTable {
private:
	std::vector<std::unique_ptr<Scope>> scopes;

public:
	SymbolTable() {
		push("program");
	}

	/*
	 * Push a scope into the vector.
	 * If there are no scopes, push one with no parent, the global.
	*/
	void push(std::string name) {
		Scope* parent = scopes.empty() ? nullptr : scopes.back().get();
		scopes.push_back(std::make_unique<Scope>(parent, name));
	}

	/*
	 * Pop the current scope.
	 * This is only called when we've finished analyzing a scope.
	*/
	void pop() {
		if (not scopes.empty())
			scopes.pop_back();
	}

	/*
	* Returns the current scope
	*/
	Scope* current() {
		return scopes.back().get();
	}

	/*
	* Inserts a symbol into the correct scope.
	*/
	bool insert(const Symbol& sym) {
		return current()->insert(sym);
	}

	/*
	 * Looks up a symbol in the correct scope.
	*/
	Symbol* lookup(const std::string& name) {
		return current()->lookup(name);
	}
};
