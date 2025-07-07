/**
 * This file contains the definitions of the Expression class methods,
 * mainly its type checking routines.
*/

#include "semantics.hpp"

Expression::Expression(BaseType t)
: type(new TypeInfo(t))
{}

Expression::Expression(TypeInfo* type, bool ok, const std::string& gen)
: type(type)
{
    this->type_ok = ok;
    this->Generate(gen);
}

Expression::Expression(State* St, std::string name) {
    Symbol* sym = St->Table()->lookup(name);

    if (sym == nullptr) {
        std::cerr << "[ERROR] Invalid struct instantiation: `"
            << name
            << "` is not a declared struct in this scope.\n";

        this->type_ok = false;
        St->FlagError();
        this->type = new TypeInfo(BaseType::NONE);
        return;
    }

    this->type_ok = true;
    this->type = new TypeInfo(sym->type);
    this->Generate(name + "{}");
}

Expression::Expression(Operator op, Expression* operand) {
    switch (op) {
        // If its a `not`, the operand must be of type `bool`,
        // else we got ourselves a type error.
        case Operator::NOT:
            this->type_ok = operand->type->b_type == BaseType::BOOL;

            if (this->type_ok)
                this->type = new TypeInfo(BaseType::BOOL);
            else
                this->type = new TypeInfo(BaseType::NONE);

            break;

        // In case of an unary minus, the operand must be
        // either one of the numeric types.
        case Operator::NEGATE:
            this->type_ok = operand->type->b_type == BaseType::INT
                or operand->type->b_type == BaseType::FLOAT;

            if (this->type_ok)
                this->type = new TypeInfo(operand->type->b_type);
            else
                this->type = new TypeInfo(BaseType::NONE);

            break;

        // Safe fallback, never really gets here.
        default:
            std::cerr << "[ERROR] Unsupported unary operator\n";
            this->type_ok = false;
            this->type = new TypeInfo(BaseType::NONE);
    }

    this->Generate(this->op_toString_Gen(op) + operand->Gen());
}

Expression::Expression(Expression* left, Operator op, Expression* right) {
    switch (op) {
        case Operator::AND:
        case Operator::OR:
            this->typeCheck_Logical(left->type, op, right->type);
            break;
        case Operator::PLUS:
        case Operator::MINUS:
        case Operator::DIVIDES:
        case Operator::TIMES:
        case Operator::POW:
            this->typeCheck_Arithmetic(left->type, op, right->type);
            break;
        case Operator::LT:
        case Operator::GT:
        case Operator::LEQ:
        case Operator::GEQ:
            this->typeCheck_Relational(left->type, op, right->type);
            break;
        case Operator::EQ:
        case Operator::NEQ:
            this->typeCheck_Equality(left->type, op, right->type);
            break;
        default:
            break;
    }

    switch (op) {
        case Operator::POW:
            this->Generate("pow(" + left->Gen() + ", " + right->Gen() + ")");
            break;
        default:
            this->Generate(left->Gen() + op_toString_Gen(op) + right->Gen());
            break;
    }
}

// ---- Private Methods: Internal Mechanisms ----

void Expression::typeCheck_Logical(TypeInfo* left, Operator op, TypeInfo* right) {
    // The type is only correct if both operands are booleans.
    this->type_ok = left->b_type == BaseType::BOOL
        && right->b_type == BaseType::BOOL;

    if (this->type_ok)
        this->type = new TypeInfo(BaseType::BOOL);
    else {
        std::cerr << "[ERROR] Invalid operands to "
            << this->op_toString(op)
            << ": cannot apply to `"
            << *left
            << "` and `"
            << *right
            << "`. " << this->op_toString(op)
            << " is only supported for `bool` operands.\n";

        // We can't synthetize any sane type in this case.
        this->type = new TypeInfo(BaseType::NONE);
    }
}

void Expression::typeCheck_Arithmetic(TypeInfo* left, Operator op, TypeInfo* right) {
    // Handling the pow operator, since it defaults to float, disregarding its
    // operand's types.
    if (op == Operator::POW) {
        if ((left->b_type == BaseType::INT || left->b_type == BaseType::FLOAT) &&
            (right->b_type == BaseType::INT || right->b_type == BaseType::FLOAT)) {
            this->type_ok = true;
            this->type = new TypeInfo(BaseType::FLOAT);
        } else {
            std::cerr << "[ERROR] Invalid operands to (^): cannot apply to `"
                << *left
                << "` and `"
                << *right
                << "`. (^) is only supported for `int` or `float`.\n";

            this->type_ok = false;
            this->type = new TypeInfo(BaseType::NONE);
        }
        return;
    }

    // Are both operands of numeric type?
    bool left_valid  = (left->b_type == BaseType::INT || left->b_type == BaseType::FLOAT);
    bool right_valid = (right->b_type == BaseType::INT || right->b_type == BaseType::FLOAT);

    // If both are numeric, we can proceed to setting the final type.
    if (left_valid && right_valid) {
        BaseType result_type = (left->b_type == BaseType::FLOAT || right->b_type == BaseType::FLOAT) ? BaseType::FLOAT : BaseType::INT;

        this->type_ok = true;
        this->type = new TypeInfo(result_type);
    } else {
        std::cerr << "[ERROR] Invalid operands to "
            << this->op_toString(op)
            << ": cannot apply to `"
            << *left
            << "` and `"
            << *right
            << "`. " << this->op_toString(op)
            << " is only supported for `int` and `float`.\n";

        this->type_ok = false;
        this->type = new TypeInfo(BaseType::NONE);
    }
}

void Expression::typeCheck_Relational(TypeInfo* left, Operator op, TypeInfo* right) {
    if ((left->b_type == BaseType::INT or left->b_type == BaseType::FLOAT) and
        (right->b_type == BaseType::INT or right->b_type == BaseType::FLOAT)) {

        this->type_ok = true;
        this->type = new TypeInfo(BaseType::BOOL);
    } else {
        std::cerr << "[ERROR] Invalid operands to "
            << this->op_toString(op)
            << ": cannot compare `"
            << *left
            << "` and `"
            << *right
            << "`. " << this->op_toString(op)
            << " is only supported for `int` and `float` operands.\n";

        this->type_ok = false;
        this->type = new TypeInfo(BaseType::NONE);
    }
}

void Expression::typeCheck_Equality(TypeInfo* left, Operator op, TypeInfo* right) {
    // Numeric types are ok even if they are different.
    if ((left->b_type == BaseType::INT or left->b_type == BaseType::FLOAT) and
        (right->b_type == BaseType::INT or right->b_type == BaseType::FLOAT))
        this->type_ok = true;
    else
        // Else we have to check if the types are equal.
        this->type_ok = *left == *right;

    if (type_ok)
        this->type = new TypeInfo(BaseType::BOOL);
    else {
        std::cerr << "[ERROR] Invalid operands to "
            << this->op_toString(op)
            << ": cannot compare `"
            << *left
            << "` and `"
            << *right
            << "`. " << this->op_toString(op)
            << " is only supported for operands of the same type (or numeric types).\n";
        this->type = new TypeInfo(BaseType::NONE);
    }
}
