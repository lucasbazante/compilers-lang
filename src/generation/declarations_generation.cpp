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
        fields << "\t" << field->type->Gen() << " " << field->name << ";\n";
    }

    St->Emit_StructDecl(this->name, fields.str());
}

void ProcedureDecl::Generate(State* St) {
    std::ostringstream proc_label;
    proc_label << "LABEL_" << this->name;
    St->Emit_Label(proc_label.str());
}

