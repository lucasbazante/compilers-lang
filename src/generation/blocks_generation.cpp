/**
 * This file contains the definitions for the code generation methods
 * for blocks of statements and the return statement.
*/

#include "semantics.hpp"

void StatementList::Generate(State* St) {
    for (auto statement : statements)
        statement->Generate(St);
}

void ReturnStatement::Generate(State* St) {
    if (exp != nullptr) {
        this->exp->Generate(St);
        St->Emit_Return_Value(this->exp->Repr());
    }
    St->Emit_Return();
}
