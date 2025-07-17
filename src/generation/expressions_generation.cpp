/**
 * This file defines the code generation for expressions.
*/

#include "semantics.hpp"

void Expression::Generate(State* St) {
    if (lhs != nullptr and rhs == nullptr)
        this->GenerateUnary(St);
    else if (lhs != nullptr and rhs != nullptr)
        this->GenerateBinary(St);
    else
        this->GenerateOperand(St);
}

void Expression::GenerateOperand(State* St) {
    if (var != nullptr)
        this->var->Internal_Generation(St), this->Set_Repr(this->var->Repr());
    else if (ref != nullptr)
        this->ref->Internal_Generation(St), this->Set_Repr(this->ref->Repr());
    else if (deref != nullptr)
        this->deref->Internal_Generation(St), this->Set_Repr(this->deref->Repr());
    else if (call != nullptr)
        this->call->Internal_Generation(St), this->Set_Repr(this->call->Repr());
    else if (not struct_name.empty())
        St->Emit_Expr(St->Scoped_Name(struct_name) + "{}", this->type), this-> Set_Repr(St->Current_TempVar());
}

void Expression::GenerateUnary(State* St) {
    this->lhs->Generate(St);

    St->Emit_Expr(
        this->op_toString_Gen(this->op) + this->lhs->Repr(),
        this->type
    );

    this->Set_Repr(St->Current_TempVar());
}

void Expression::GenerateBinary(State* St) {
    this->lhs->Generate(St);
    this->rhs->Generate(St);

    switch (op) {
        case Operator::POW:
            St->Emit_Expr(
                "pow(" + lhs->Repr() + ", " + rhs->Repr() + ")",
                this->type
            );
            break;
        default:
            St->Emit_Expr(
                lhs->Repr() + this->op_toString_Gen(op) + rhs->Repr(),
                this->type
            );
            break;
    }

    this->Set_Repr(St->Current_TempVar());
}
