/**
 * This file contains the definitions handling all kinds of assignment
 * statements in the language, namely assigning to a regular variable
 * and dereferencing a name before assigning.
*/

#include "semantics.hpp"

AssignStatement::AssignStatement(State* St, Variable* var, Expression* exp)
: var(var), deref(nullptr), exp(exp)
{
    if (*var->type != *exp->type) {
        if (not is_ValidCoercion(*var->type, *exp->type)) {
            std::cerr << "[ERROR] Type error on assignment: expected `"
                << *var->type
                << "`, got `"
                << *exp->type
                << "`.\n";
            this->type_ok = false;
            St->FlagError();
            return;
        }
    }

    this->type_ok = var->Ok() and exp->Ok();

    if (not this->type_ok)
        St->FlagError();
}

AssignStatement::AssignStatement(State* St, Dereference* deref, Expression* exp)
: var(nullptr), deref(deref), exp(exp)
{
    if (*deref->type != *exp->type) {
        if (not is_ValidCoercion(*deref->type, *exp->type)) {
            std::cerr << "[ERROR] Type error on assignment: expected `"
                << *deref->type
                << "`, got `"
                << *exp->type
                << "`.\n";
            this->type_ok = false;
            St->FlagError();
            return;
        }
    }

    this->type_ok = deref->Ok() and exp->Ok();

    if (not this->type_ok)
        St->FlagError();
}
