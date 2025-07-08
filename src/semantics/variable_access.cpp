/**
 * This file contains the definitions for all kinds of variable access in the language,
 * namely the regular access and struct field access.
*/

#include "semantics.hpp"

Variable::Variable(State* St, std::string name)
: name(name), struct_exp(nullptr)
{
    Symbol* sym = St->Table()->lookup(name);

    // Is it declared in any way?
    if (sym == nullptr) {
        std::cerr << "[ERROR] The name ´"
            << name
            << "´ isn't declared anywhere in this scope.\n";
        this->type = new TypeInfo(BaseType::NONE);
        this->type_ok = false;
        St->FlagError();
        return;
    }

    // If it's declared, is it a variable or parameter?
    // We could have recovered a function symbol, for example.
    if (sym->kind != SymbolKind::VARIABLE and sym->kind != SymbolKind::PARAMETER) {
        std::cerr << "[ERROR] The name `"
            << name
            << "` doesn't refer to a variable or a parameter.\n";
        this->type_ok = false;
        St->FlagError();
        this->type = new TypeInfo(BaseType::NONE);
        return;
    }

    this->type_ok = true;
    this->type = &sym->type;
}

Variable::Variable(State* St, Expression* exp, std::string name)
: name(name), struct_exp(exp)
{
    if (exp->type->b_type != BaseType::STRUCT) {
        std::cerr << "[ERROR] Trying to use dot notation on a non-struct object.\n";

        this->type_ok = false;
        St->FlagError();
        this->type = new TypeInfo(BaseType::NONE);

        return;
    }

    Symbol* sym = St->Table()->lookup(exp->type->struct_name);

    if (sym == nullptr) {
        std::cerr << "[ERROR] Invalid struct: ´"
            << exp->type->struct_name
            << "´ isn't declared as a struct anywhere in this scope.\n";

        this->type_ok = false;
        St->FlagError();
        this->type = new TypeInfo(BaseType::NONE);

        return;
    }

    for (auto field : sym->parameters) {
        if (name == field.first) {
            this->type_ok = true;
            this->type = new TypeInfo(field.second);
            return;
        }
    }

    std::cerr << "[ERROR] Invalid access to struct field: the field `"
        << name 
        << "` doesn't exist in the struct `"
        << exp->type->struct_name << "`.\n";

    this->type_ok = false;
    St->FlagError();
    this->type = new TypeInfo(BaseType::NONE);
}
