#pragma once

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "symbol_table.hpp"

class SemanticAction {
public:
  bool type_ok;
};

class Program : public SemanticAction {
};

// ---- DECLARATIONS ----

class VarDecl : public SemanticAction {
public:
  /*
   * Just a declaration, no expression.
   * Or it could be the case of a type inference.
  */
  VarDecl(SymbolTable* sb, std::string name, TypeInfo type) {
    Symbol sym(name, SymbolKind::VARIABLE, type);

    if (not sb->insert(sym)) {
      std::cout << "Symbol " << name << " is already declared.\n";
    }

    this->type_ok = true;
  }

  /*
   * Declaration and expression as value, must type-check.
  */
  VarDecl(SymbolTable* sb, std::string name, TypeInfo decl_type, TypeInfo actual_type) {
    if (decl_type.type != actual_type.type) {
      std::cout << "Expected " << decl_type.type << ", received " << actual_type.type << "." << std::endl;
      this->type_ok = false;
    }

    Symbol sym(name, SymbolKind::VARIABLE, decl_type);

    if (not sb->insert(sym)) {
      std::cout << "Symbol " << name << " is already declared.\n";
    }

    this->type_ok = true;
  }
};

class ParameterDecl : public SemanticAction {
public:
  std::string name;
  TypeInfo* type;

  ParameterDecl(std::string name, TypeInfo* type)
    : name(name), type(type)
  {}
};

class ParameterField : public SemanticAction {
public:
  std::vector<ParameterDecl*> fields;

  ParameterField() = default;
  
  void add(ParameterDecl* param) {
    fields.push_back(param);
  }

  ~ParameterField() {
    for (auto f : fields) delete f;
  }
};

class StructDecl : public SemanticAction {
public:
  std::string name;
  ParameterField* fields;

  StructDecl(SymbolTable* symtab, std::string name, ParameterField* fields) {
    TypeInfo struct_type(BaseType::STRUCT, name);

    Symbol symbol(name, SymbolKind::STRUCT, struct_type);

    for (const auto& field : fields->fields) {
      symbol.parameters.push_back({field->name, *field->type});
    }

    if (not symtab->insert(symbol)) {
      std::cerr << "Error: Struct `" << name << "` is already defined.\n";
      std::exit(1);
    }
  }
};

// ---- Expressions ----

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

  /*
   * Defining struct with `new name`.
   */
  Expression(SymbolTable* symtab, std::string name) {
    Symbol* sym = symtab->lookup(name);
    
    if (sym == nullptr) {
      std::cerr << "Invalid struct instantiation: `" << name << "` is not a declared struct in this scope.";
      std::exit(1);
    }

    this->type = new TypeInfo(BaseType::STRUCT, name);
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

class Variable : public SemanticAction {
public:
  TypeInfo* type;

  Variable(SymbolTable* symtab, std::string name) {
    Symbol* sym = symtab->lookup(name);

    if (sym == nullptr) {
      std::cerr << "Name not found." << std::endl;
      this->type = new TypeInfo(BaseType::NONE);
      return;
    }

    this->type = &sym->type;
  }

  /*
  * Handling the case of accessing a field from a struct.
  * The struct may come by an expression, so we receive it.
  */
  Variable(SymbolTable* symtab, Expression* exp, std::string name) {
    if (exp->type->type != BaseType::STRUCT) {
      std::cerr << "Trying to use dot notation on a non-struct object." << std::endl;
      std::exit(1);
    }
    
    Symbol* sym = symtab->lookup(exp->type->struct_name);

    if (sym == nullptr) {
      std::cerr << "Name not found." << std::endl;
      this->type = new TypeInfo(BaseType::NONE);
      return;
    }

    for (auto field : sym->parameters) {
      if (name == field.first) {
        this->type_ok = true;
        this->type = &sym->type;
        return;
      }
    }

    std::cerr << "Invalid access to struct field: non-existent field `" << name << "`" << std::endl;
    std::exit(1);
  }
};
