/**
 * This file contains all the definitions for the handling of control flow statements in
 * the language, namely: `if`, `while`, `for` and `do-until`.
*/

#include "semantics.hpp"

// ---- If Statement ----

IfStatement::IfStatement(State* St, Expression* condition, StatementList* body, StatementList* else_body) {
    this->type_ok = condition->Ok();
    this->has_return = body->has_return;
    this->return_type = body->return_type;

    if (not this->type_ok)
        St->FlagError();

    auto if_body = *body;

    if (not else_body->statements.empty()) {
        for (auto statement : else_body->statements)
        body->add(St, statement);
    }

    if (condition->type->b_type != BaseType::BOOL) {
        std::cerr << "[ERROR] The condition that defines the if statement branching must be of type `bool`."
            << " Received `" << *condition->type << "`.\n";

        this->type_ok = false;
        St->FlagError();
    }

    // Code generation
    this->then_label = St->Next_Label();
    this->else_label = not else_body->statements.empty() ? St->Next_Label() : "";
    this->end_label  = St->Next_Label();

    std::ostringstream gen;
    gen << "if ("
        << condition->Gen()
        << ") goto "
        << this->then_label
        << ";\n";

    if (not else_label.empty())
        gen << "goto "
            << this->else_label
            << ";\n";
    else
        gen << "\tgoto "
            << this->end_label
            << ";\n";

    gen << this->then_label
        << ":\n"
        << if_body.Gen();

    if (not else_label.empty())
        gen << "\tgoto "
            << this->end_label
            << ";\n"
            << this->else_label
            << ":\n"
            << else_body->Gen();

    gen << this->end_label
        << ":\n";

    this->Generate(gen.str());
}

// ---- While Loop ----

WhileStatement::WhileStatement(State* St, Expression* condition, StatementList* body) {
    this->type_ok = condition->Ok();
    this->has_return = body->has_return;
    this->return_type = body->return_type;

    if (condition->type->b_type != BaseType::BOOL) {

        std::cerr << "[ERROR] The condition that defines the while loop stop must be of type `bool`."
            << " Received `" << *condition->type << "`.\n";

        this->type_ok = false;
        St->FlagError();
    }

    this->start_label = St->Next_Label();
    this->end_label   = St->Next_Label();

    std::ostringstream gen;
    gen << this->start_label
        << ":\n";
}

// ---- For Loop ----

ForStatement::ForStatement(State* St, std::string name, Expression* eq, Expression* to, Expression* step, StatementList* body) {
    this->type_ok = true;
    this->has_return = body->has_return; // If the body has return, this statement also has.
    this->return_type = body->return_type; // The return type of this statement is the return type of its body.

    Symbol* sym = St->Table()->lookup(name);

    // Check if the counter variable exists.
    if (sym == nullptr) {
        std::cerr << "[ERROR] Name `"
            << name
            << "` is not declared in this scope.\n";
    }

    // Check the types of the stepping logic.
    if (sym->type.b_type != BaseType::INT
        or eq->type->b_type != BaseType::INT
        or to->type->b_type != BaseType::INT
        or step->type->b_type != BaseType::INT) {
        std::cerr << "[ERROR] Expressions that define the loop stepping logic for must be of type `int`.\n";
        this->type_ok = false;
        St->FlagError();
    }
}

// ---- Do-Until Loop ----

DoUntilStatement::DoUntilStatement(State* St, Expression* condition, StatementList* body) {
    this->type_ok = condition->Ok();
    this->has_return = body->has_return;
    this->return_type = body->return_type;

    if (condition->type->b_type != BaseType::BOOL) {

        std::cerr << "[ERROR] The condition that defines the do-until loop stop must be of type `bool`."
            << " Received `" << *condition->type << "`.\n";

        this->type_ok = false;
        St->FlagError();
    }
}
