/**
 * This file contains the definitions handling blocks and its associated returns.
 * 
 * Simply put, it handles the definitions for the `StatementList` and `ReturnStatement`
 * semantic classes.
 *
 * Since blocks (statements lists) ultimately have a "return type" in the semantic rules,
 * which is used for type checking, it makes sense to group them together in a file.
*/

#include "semantics.hpp"

// ---- Blocks (Statement Lists) ----

void StatementList::add(State* St, Statement* statement) {
    if (statement->has_return) {
        // If this body has no return, set.
        if (not this->has_return) {
            this->has_return = statement->has_return;
            this->return_type = new TypeInfo(*statement->return_type);
        }

        // If it has, compare.
        else if (*statement->return_type != *this->return_type) {
            if (not is_ValidCoercion(*statement->return_type, *this->return_type)) {
                std::cerr << "[ERROR] Inconsistent return types in function `"
                    << St->Table()->current()->name
                    << "`. Previously got `"
                    << *this->return_type
                    << "`, now got `"
                    << *statement->return_type
                    << "`.\n";

                // Keep the return type as it was.
                this->type_ok = false;
                St->FlagError();
            }
        }
    }

    statements.push_back(statement);
}

void StatementList::verify_return(State* St, ProcedureDecl* declaration) {
    this->type_ok = true;

    // If the function is non-void and the body has no returns.
    if (declaration->return_type->b_type != BaseType::NONE and not this->has_return) {
        std::cerr << "[ERROR] Function `"
            << declaration->name
            << "` is expected to return a value of type `"
            << *declaration->return_type
            << "` but no return statement was found.\n";

        this->type_ok = false;
        St->FlagError();
        return;
    }

    // Checks every type of every return.
    for (auto statement: statements)
    if (statement->has_return) {
        if (*statement->return_type != *declaration->return_type) {
            if (not is_ValidCoercion(*declaration->return_type, *statement->return_type)) {
                std::cerr << "[ERROR] Function `"
                    << declaration->name
                    << "` is declared to return `"
                    << *declaration->return_type
                    << "`, but returns `"
                    << *statement->return_type
                    << "`.\n";

                this->type_ok = false;
                St->FlagError();
                continue;
            }
        }
    }
}

// ---- Return ----

ReturnStatement::ReturnStatement() {
    this->type_ok = true;
    this->has_return = true;
    this->return_type = new TypeInfo(BaseType::NONE);
}

ReturnStatement::ReturnStatement(State* St, Expression* exp) {
    this->type_ok = exp->Ok();

    if (not this->type_ok)
        St->FlagError();

    this->has_return = true;
    this->return_type = new TypeInfo(*exp->type);
}
