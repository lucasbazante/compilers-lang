/**
 * This file contains the definitions for the code generation method
 * for variable accessing and struct field accessing.
*/

#include "semantics.hpp"

void Variable::Generate(State* St) {
    if (this->struct_exp == nullptr)
        St->Emit_Var(this->name);
    else {
        this->struct_exp->Generate(St);
        St->Emit_Access(this->name, this->struct_exp->Repr());
    }
}

void Variable::Internal_Generation(State* St) {
    if (this->struct_exp == nullptr)
        this->Set_Repr(St->Scoped_Name(this->name));
    else {
        this->struct_exp->Generate(St);
        this->Set_Repr("(" + this->struct_exp->Repr() + ")" + "." + this->name);
    }
}
