/**
 * This file contains the definitions for the code generation methods
 * for all kinds of control flow commands, namely `if`, `while`, `do-until` and `for`.
*/

#include "semantics.hpp"

void IfStatement::Generate(State* St) {
    std::string then_label = St->Next_Label(),
                else_label = not else_body->statements.empty() ? St->Next_Label() : "",
                end_label  = St->Next_Label();

    this->condition->Generate(St);

    St->Emit_If_Header(
        this->condition->Repr(),
        then_label,
        end_label,
        else_label
    );

    St->Emit_Label(then_label);
    this->then_body->Generate(St);

    if (not else_label.empty()) {
        St->Emit(
            "goto "   +
            end_label +
            ";"
        );
        St->Emit_Label(else_label);
        this->else_body->Generate(St);
    }

    St->Emit_Label(end_label);
}

void WhileStatement::Generate(State* St) {
    std::string start_label = St->Next_Label(),
                end_label   = St->Next_Label();

    St->Emit_Label(start_label);

    this->condition->Generate(St);

    St->Emit_While_Header(this->condition->Repr(), end_label);

    this->body->Generate(St);

    St->Emit(
        "goto "     +
        start_label +
        ";"
    );

    St->Emit_Label(end_label);
}
