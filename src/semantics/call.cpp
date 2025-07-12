/**
 * This file contains the definitions for handling function calls in our language,
 * be it as a statement or as an expression.
*/

#include "semantics.hpp"

Call::Call(State* St, std::string f_name, ExpressionList* exp_list)
: f_name(f_name), exp_list(exp_list)
{
    Symbol* fun = St->Table()->lookup(f_name);

    // Check if the callee exists.
    if (fun == nullptr) {
        std::cerr << "[ERROR] Call to non-declared function `"
            << f_name
            << "`.\n";

        this->type_ok = false;
        St->FlagError();
        this->type = new TypeInfo(BaseType::NONE);
        return;
    }

    // Check if the callee is a function.
    if (fun->kind != SymbolKind::FUNCTION) {
        std::cerr << "[ERROR] `"
            << f_name
            << "` is not a function.\n";

        this->type_ok = false;
        St->FlagError();
        this->type = new TypeInfo(BaseType::NONE);
    }

    const auto& param_types = fun->parameters;
    const auto& arg_exprs = exp_list->exp_list;

    // Check if the actual parameters were all passed.
    if (param_types.size() != arg_exprs.size()) {
        std::cerr << "[ERROR] Function `" << f_name << "` expects " << param_types.size()
            << " arguments, but got " << arg_exprs.size() << ".\n";
        this->type_ok = false;
        St->FlagError();
        this->type = new TypeInfo(BaseType::NONE);
        return;
    }

    // Checks the types of the actual parameters against the formal ones.
    for (size_t i = 0; i < param_types.size(); ++i) {
        if (!(*arg_exprs[i]->type == param_types[i].second)) {
            if (not is_ValidCoercion(*arg_exprs[i]->type, param_types[i].second)) {
                std::cerr << "[ERROR] Argument " << i + 1 << " of call to `" << f_name
                    << "` has type `" << *arg_exprs[i]->type << "`, expected `"
                    << param_types[i].second << "`.\n";
                this->type_ok = false;
                St->FlagError();
                this->type = new TypeInfo(BaseType::NONE);
                return;
            }
        }
    }

    // If we reached here, the call is valid
    this->type_ok = true;
    this->type = new TypeInfo(fun->type);  // Copy return type
}
