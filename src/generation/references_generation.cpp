/**
 * This file contains the definitions for the code generation method
 * for references and dereferences.
*/

#include "semantics.hpp"

/*
 * The code generation for a reference is pretty simple.
 * 
 * An well-formed reference always contains a variable,
 * so we just generate the `&` along with what the variable
 * has for generated code.
 */
void Reference::Generate(State* St) {
    St->Emit("&");
    this->var->Generate(St);
}

void Reference::Internal_Generation(State* St) {
    this->var->Internal_Generation(St);
    this->Set_Repr("&" + this->var->Repr());
}

/*
 * This generates the code for a dereferencing.
 *
 * There are only two possible cases for an well-formed
 * dereferencing:
 *
 *      * Either it's dereferencing a variable,
 *      * Or it's dereferencing another dereference, recursively,
 *        which in turn must stop the chain with a variable.
 *
 * So both cases are covered here.
*/
void Dereference::Generate(State* St) {
    St->Emit_OnLine("*");

    if (var == nullptr)
        this->deref->Generate(St);
    else
        this->var->Generate(St);
}

void Dereference::Internal_Generation(State* St) {
    if (var == nullptr) {
        this->deref->Internal_Generation(St);
        this->Set_Repr("*" + this->deref->Repr());
    }

    else {
        this->var->Internal_Generation(St);
        this->Set_Repr("*" + this->var->Repr());
    }
}
