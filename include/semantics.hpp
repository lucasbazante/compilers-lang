#pragma once

#include <iostream>
#include <string>

#include "symbol_table.hpp"

class SemanticAction {
public:
  bool type_ok;
};

class Program : public SemanticAction {
};

class VarDecl : public SemanticAction {
public:
  VarDecl(SymbolTable* sb, std::string name, TypeInfo type) {
    Symbol sym(name, SymbolKind::VARIABLE, type);

    if (not sb->insert(sym)) {
      std::cout << "Symbol " << name << " is already declared.\n";
    }

    this->type_ok = true;
  }

  VarDecl() {

  }
};

class Expression : public SemanticAction {
public:
  TypeInfo* type;

  enum class Operator {
    AND, OR, NOT,
    LT, GT, LEQ, GEQ, EQ, NEQ,
    PLUS, MINUS, DIVIDES, TIMES, POW
  };

  Expression() {}

  // literal
  Expression(TypeInfo* type)
    : type(type) {
    this->type_ok = true;
  }

  // Unary: NOT
  Expression(Operator op, TypeInfo* operand) {
    switch (op) {
        case Operator::NOT:
          type_ok = operand->type == BaseType::BOOL;

          if (type_ok)
            type = new TypeInfo(BaseType::BOOL);

          break;
        default:
            std::cerr << "Unsupported unary operator\n";
            type_ok = false;
    }
  }

  // Binary operations
  Expression(TypeInfo* left, Operator op, TypeInfo* right) {
    switch (op) {
      case Operator::AND:
      case Operator::OR:
        this->typeCheck_Logical(left, right);
        break;
      case Operator::PLUS:
      case Operator::MINUS:
      case Operator::DIVIDES:
      case Operator::TIMES:
      case Operator::POW:
        this->typeCheck_Arithmetic(left, op, right);
        break;
      case Operator::LT:
      case Operator::GT:
      case Operator::LEQ:
      case Operator::GEQ:
        this->typeCheck_Relational(left, right);
        break;
      case Operator::EQ:
      case Operator::NEQ:
        this->typeCheck_Equality(left, right);
        break;
      default:
        break;
    }
  }

private:
  void typeCheck_Logical(TypeInfo* left, TypeInfo* right) {
    this->type_ok = left->type == BaseType::BOOL && right->type == BaseType::BOOL;

    if (this->type_ok)
      this->type = new TypeInfo(BaseType::BOOL);
  }

  
  void typeCheck_Arithmetic(TypeInfo* left, Operator op, TypeInfo* right) {
    if (op == Operator::POW) {
        if ((left->type == BaseType::INT || left->type == BaseType::FLOAT) &&
            (right->type == BaseType::INT || right->type == BaseType::FLOAT)) {
            this->type_ok = true;
            this->type = new TypeInfo(BaseType::FLOAT);
        } else {
            this->type_ok = false;
        }
        return;
    }

    bool left_valid  = (left->type == BaseType::INT || left->type == BaseType::FLOAT);
    bool right_valid = (right->type == BaseType::INT || right->type == BaseType::FLOAT);

    if (left_valid && right_valid) {
        BaseType result_type = (left->type == BaseType::FLOAT || right->type == BaseType::FLOAT)
                             ? BaseType::FLOAT
                             : BaseType::INT;
        this->type = new TypeInfo(result_type);
        this->type_ok = true;
    } else {
        this->type_ok = false;
    }
  }

  void typeCheck_Relational(TypeInfo* left, TypeInfo* right) {
    if ((left->type == BaseType::INT || left->type == BaseType::FLOAT) &&
        left->type == right->type) {
        type_ok = true;
        type = new TypeInfo(BaseType::BOOL);
    } else {
        type_ok = false;
    }
  }

  void typeCheck_Equality(TypeInfo* left, TypeInfo* right) {
    type_ok = left->type == right->type;

    if (type_ok)
      type = new TypeInfo(BaseType::BOOL);
  }
};

class Var : public SemanticAction {
  TypeInfo* type;
};
