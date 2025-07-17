/**
 * This file contains the definitions for generating code
 * for all kinds of declarations.
*/

#include "semantics.hpp"

void VarDecl::Generate(State* St) {
    if (expression == nullptr)
        St->Emit_Decl(this->name, this->type);
    else {
        this->expression->Generate(St);
        St->Emit_Decl(this->name, this->type, this->expression->Repr());
    }
}

void StructDecl::Generate(State* St) {
    std::ostringstream fields;

    for (auto field : this->paramfield->fields) {
        fields << "\t" << St->Scoped_Type(field->type) << " " << field->name << ";\n";
    }

    St->Emit_StructDecl(this->name, fields.str());
}

void ProcedureDecl::Generate(State* St) {
    for (auto f : this->params->fields)
        St->Emit_Param(f->name, f->type);

    if (this->return_type->b_type != BaseType::NONE)
        St->Emit_Return_Var(this->name, this->return_type);
}
