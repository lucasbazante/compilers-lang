/**
 * This file contains the definitions for handling references
 * and its dereferencing actions.
*/

#include "semantics.hpp"

// ---- References ----

Reference::Reference(State* St, Variable* var) {
    // If anything went wrong already with the argument.
    if (var->type->b_type == BaseType::NONE) {
        std::cerr << "[ERROR] Cannot create reference to an invalid type.\n";
        this->type_ok = false;
        St->FlagError();
        this->type = new TypeInfo(BaseType::NONE);
        return;
    }

    this->type_ok = true;
    this->type = new TypeInfo(BaseType::REFERENCE, *var->type);
    this->Generate("&" + var->Gen());
}

// ---- Dereference ----

Dereference::Dereference(State* St, Variable* var) {
    if (var->type->b_type != BaseType::REFERENCE) {
        std::cerr << "[ERROR] Cannot dereference a type that isn't a reference.\n";
        this->type_ok = false;
        St->FlagError();
        this->type = new TypeInfo(BaseType::NONE);
        return;
    }

    this->type_ok = true;
    this->type = type->ref_base.get();
    this->Generate("*" + var->Gen());
}

Dereference::Dereference(State* St, Dereference* deref) {
    if (deref->type->b_type != BaseType::REFERENCE) {
        std::cerr << "[ERROR] Cannot dereference a type that isn't a reference.\n";
        this->type_ok = false;
        St->FlagError();
        this->type = new TypeInfo(BaseType::NONE);
        return;
    }

    this->type_ok = true;
    this->type = type->ref_base.get();
    this->Generate("*" + deref->Gen());
}
