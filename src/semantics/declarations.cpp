/**
 * This file contains the definitions for the semantics of all
 * kinds of declarations in the language: variables, structs and procedures.
*/

#include "semantics.hpp"

// ---- Variable Declaration ----

VarDecl::VarDecl(State* St, std::string name, TypeInfo* decl_type)
: type(decl_type), expression(nullptr), name(name)
{
    this->type_ok = true;

    // Checking if type exists, in case of an invalid struct.
    if (not decl_type->struct_name.empty() and not St->Table()->lookup(decl_type->struct_name)) {
        std::cout << "[ERROR] In variable ´"
            << name << "` declaration: `"
            << decl_type->struct_name << "´ is not a declared type in the current scope.\n";
        this->type_ok = false;
        St->FlagError();
        decl_type->b_type = BaseType::NONE;
    }

    Symbol sym(name, SymbolKind::VARIABLE, *decl_type);

    // If we could not insert, it means that there is already a symbol declared
    // in the same scope.
    if (not St->Table()->insert(sym)) {
        std::cout << "[ERROR] In variable ´"
            << name << "´ declaration: symbol `"
            << name << "` is already declared in the current scope.\n";
        this->type_ok = false;
        St->FlagError();
    }
}

VarDecl::VarDecl(State* St, std::string name, Expression* expression)
: type(expression->type), expression(expression), name(name)
{
    this->type_ok = true;
    auto decl_type = expression->type;

    // Checking if type exists, in case of an invalid struct.
    if (not decl_type->struct_name.empty() and not St->Table()->lookup(decl_type->struct_name)) {
        std::cout << "[ERROR] In variable ´"
            << name << "` declaration: `"
            << decl_type->struct_name << "´ is not a declared type in the current scope.\n";
        this->type_ok = false;
        St->FlagError();
        decl_type->b_type = BaseType::NONE;
    }

    Symbol sym(name, SymbolKind::VARIABLE, *decl_type);

    // If we could not insert, it means that there is already a symbol declared
    // in the same scope.
    if (not St->Table()->insert(sym)) {
        std::cout << "[ERROR] In variable ´"
            << name << "´ declaration: symbol `"
            << name << "` is already declared in the current scope.\n";
        this->type_ok = false;
        St->FlagError();
    }
}

VarDecl::VarDecl(State* St, std::string name, TypeInfo* decl_type, Expression* expression)
: type(decl_type), expression(expression), name(name)
{
    this->type_ok = true;

    // Checking if type is a valid type in case of it being a struct type.
    if (not decl_type->struct_name.empty() and not St->Table()->lookup(decl_type->struct_name)) {
        std::cout << "[ERROR] In variable ´"
            << name << "` declaration: `"
            << decl_type->struct_name << "´ is not a declared type in the current scope.\n";
        this->type_ok = false;
        St->FlagError();
        decl_type->b_type = BaseType::NONE;
    }

    auto actual_type = expression->type;
    // Type checking the expected type agains the actual type, considering the possibilty
    // of a valid coercion.
    if (decl_type->b_type != BaseType::NONE and *decl_type != *actual_type) {
        if (not is_ValidCoercion(*decl_type, *actual_type)) {
            std::cout << "[ERROR] Expected ´"
                << *decl_type
                << "´, received ´"
                << *actual_type
                << "´.\n";

            this->type_ok = false;
            St->FlagError();
        }
    }

    Symbol sym(name, SymbolKind::VARIABLE, *decl_type);

    // If we could not insert the symbol, it is already declared under the current scope.
    if (not St->Table()->insert(sym)) {
        std::cout << "[ERROR] In variable ´"
            << name << "´ declaration: symbol `"
            << name << "` is already declared in the current scope.\n";
        this->type_ok = false;
        St->FlagError();
    }
}

// ---- Struct Declaration ----

StructDecl::StructDecl(State* St, std::string name, ParameterField* paramfield)
: name(name), paramfield(paramfield)
{
    this->type_ok = true;

    TypeInfo struct_type(BaseType::STRUCT, name);

    Symbol symbol(name, SymbolKind::STRUCT, struct_type);

    // Adding the parameters to the type information.
    for (const auto& field : paramfield->fields) {
        symbol.parameters.push_back({field->name, *field->type});
    }

    // Try to add, flag error if it already exists.
    if (not St->Table()->insert(symbol)) {
        std::cerr << "[ERROR] In struct `"
            << name
            << "` declaration: the symbol `" << name << "` is already declared in the current scope.\n";
        this->type_ok = false;
        St->FlagError();
    }
}

// ---- Procedure Declaration ----

ProcedureDecl::ProcedureDecl(State* St, std::string name, ParameterField* params, TypeInfo* return_type) {
    this->type_ok = true;

    Symbol sym(name, SymbolKind::FUNCTION, *return_type);

    // Saving its parameters.
    for (auto param : params->fields)
    sym.parameters.push_back({param->name, *param->type});

    if (not St->Table()->insert(sym)) {
        std::cerr << "[ERROR] In declaration of procedure `"
            << name
            << "`: symbol `"
            << name << "` is already declared in this scope.\n";
        this->type_ok = false;
        St->FlagError();
    }

    this->name = name;
    this->return_type = new TypeInfo(*return_type);
}

void ProcedureDecl::declare_params_in_scope(State* St, ParameterField* params) {
    for (auto param : params->fields) {
        Symbol sym(param->name, SymbolKind::PARAMETER, *param->type);

        // If we can't insert the symbol, it means there are two
        // or more parameters with the same name, since its a fresh scope
        // with no other symbols other than the parameters at this point.
        if (not St->Table()->insert(sym)) {
            std::cerr << "[ERROR] Redeclaration of parameter `"
                << sym.name
                << "` in procedure `"
                << this->name
                << "` declaration.\n";
            this->type_ok = false;
            St->FlagError();
        }
    }
}
