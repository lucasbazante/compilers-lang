/**
 * This file contains the definitions for the code generation methods
 * for assignment statements of all kinds.
*/

#include "semantics.hpp"

/*
 * An well-formed assignment statement has either a variable or a dereference
 * to which the expression will be assigned to.
 *
 * This method deals with both of these cases: if one is null, the other must
 * hold a valid pointer, so we generate code based on one of the cases.
*/
void AssignStatement::Generate(State* St) {
    this->exp->Generate(St);

    if (this->var != nullptr)
        this->var->Generate(St);
    else
        this->deref->Generate(St);

    St->Emit_OnLine(" = ");
    St->Emit(this->exp->Repr() + ";");
}
