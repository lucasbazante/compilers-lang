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

void DoUntilStatement::Generate(State* St) {
    std::string loop_label = St->Next_Label();

    St->Emit_Label(loop_label);

    this->body->Generate(St);
    this->condition->Generate(St);

    St->Emit_DoUntil_Header(this->condition->Repr(), loop_label);
}

void ForStatement::Generate(State* St) {
    std::string start_label = St->Next_Label(),
                end_label   = St->Next_Label();

    // i = exp
    auto initialize = new AssignStatement(St, this->var, this->eq);
    initialize->Generate(St);

    // i <= to
    St->Emit_Label(start_label);

    auto to_exp = new Expression(
        new Expression(this->var),
        Expression::Operator::LEQ,
        this->to
    );
    to_exp->Generate(St);

    St->Emit_While_Header(to_exp->Repr(), end_label);

    // body
    this->body->Generate(St);

    // i += step
    auto step_exp = new Expression(
        new Expression(this->var),
        Expression::Operator::PLUS,
        this->step
    );

    auto assign = new AssignStatement(St, this->var, step_exp);
    assign->Generate(St);

    St->Emit(
        "goto "     +
        start_label +
        ";"
    );
    St->Emit_Label(end_label);
}
