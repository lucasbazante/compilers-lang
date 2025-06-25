#pragma once

#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

#include "symbol_table.hpp"

class SemanticAction {
public:
  bool type_ok;

  bool is_ValidCoercion(TypeInfo l, TypeInfo r) {
    return (l.b_type == BaseType::FLOAT and r.b_type == BaseType::INT);
  }
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
  VarDecl(SymbolTable* sb, std::string name, TypeInfo decl_type) {
    this->type_ok = true;

    // Checking if type exists.
    if (not decl_type.struct_name.empty() and not sb->lookup(decl_type.struct_name)) {
        std::cout << "[ERROR] ´" << decl_type.struct_name << "´ is not a type.\n";
        this->type_ok = false;
        decl_type.b_type = BaseType::NONE;
    }

    Symbol sym(name, SymbolKind::VARIABLE, decl_type);

    // Redeclaration under same scope.
    if (not sb->insert(sym)) {
      std::cout << "[ERROR] Symbol ´" << name << "´ is already declared.\n";
      this->type_ok = false;
    }
  }

  /*
   * Declaration and expression as value, must type-check.
  */
  VarDecl(SymbolTable* sb, std::string name, TypeInfo decl_type, TypeInfo actual_type) {
    this->type_ok = true;

    // Checking if type exists
    if (not decl_type.struct_name.empty() and not sb->lookup(decl_type.struct_name)) {
        std::cout << "[ERROR] ´" << decl_type.struct_name << "´ is not a type.\n";
        this->type_ok = false;
        decl_type.b_type = BaseType::NONE;
    }

    if (decl_type.b_type != BaseType::NONE and decl_type != actual_type) {
      if (not is_ValidCoercion(decl_type, actual_type)) {
        std::cout << "[ERROR] Expected ´"
                  << decl_type
                  << "´, received ´"
                  << actual_type
                  << "´.\n";

        this->type_ok = false;
      }
    }

    Symbol sym(name, SymbolKind::VARIABLE, decl_type);

    if (not sb->insert(sym)) {
      std::cout << "[ERROR] Symbol ´" << name << "´ is already declared.\n";
      this->type_ok = false;
    }
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

class ProcedureDecl : public SemanticAction {
public:
  std::string name;
  TypeInfo* return_type;

  ProcedureDecl(SymbolTable* symtab, std::string name, ParameterField* params, TypeInfo* return_type) {
    Symbol sym(name, SymbolKind::FUNCTION, *return_type);

    for (auto param : params->fields)
      sym.parameters.push_back({param->name, *param->type});

    if (not symtab->insert(sym)) {
      std::cerr << "[ERROR] Procedure `"
                << name
                << "` is already declared in this scope.\n";
    }

    this->name = name;
    this->return_type = new TypeInfo(*return_type);
  }

  void declare_params_in_scope(SymbolTable* symtab, ParameterField* params) {
    for (auto param : params->fields) {
      Symbol sym(param->name, SymbolKind::PARAMETER, *param->type);

      if (not symtab->insert(sym)) {
        std::cerr << "[ERROR] Redeclaration of parameter `"
                  << sym.name
                  << "` in procedure `"
                  << this->name
                  << "`.\n";
      }
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
    NEGATE,
    PLUS, MINUS, DIVIDES, TIMES, POW
  };

  Expression() {}

    Expression(BaseType t)
    { this->type = new TypeInfo(t);}

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
      std::cerr << "[ERROR] Invalid struct instantiation: `"
                << name
                << "` is not a declared struct in this scope.\n";
      this->type_ok = false;
      this->type = new TypeInfo(BaseType::NONE);
      return;
    }
    
    this->type_ok = true;
    this->type = new TypeInfo(sym->type);
  }

  /*
   * Unary operators: not and unary minus.
   */
  Expression(Operator op, TypeInfo* operand) {
    switch (op) {
        case Operator::NOT:
          this->type_ok = operand->b_type == BaseType::BOOL;

          if (this->type_ok)
            this->type = new TypeInfo(BaseType::BOOL);
          else
            this->type = new TypeInfo(BaseType::NONE);

          break;
        case Operator::NEGATE:
          this->type_ok = operand->b_type == BaseType::INT
                  or operand->b_type == BaseType::FLOAT;

          if (this->type_ok)
            this->type = new TypeInfo(operand->b_type);
          else
            this->type = new TypeInfo(BaseType::NONE);

          break;
        default:
            std::cerr << "[ERROR] Unsupported unary operator\n";
            this->type_ok = false;
            this->type = new TypeInfo(BaseType::NONE);
    }
  }

  // Binary operations
  Expression(TypeInfo* left, Operator op, TypeInfo* right) {
    switch (op) {
      case Operator::AND:
      case Operator::OR:
        this->typeCheck_Logical(left, op, right);
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
        this->typeCheck_Relational(left, op, right);
        break;
      case Operator::EQ:
      case Operator::NEQ:
        this->typeCheck_Equality(left, op, right);
        break;
      default:
        break;
    }
  }

private:
  std::string op_toString(Operator op) {
    switch (op) {
        case Operator::AND:     return "(&&)";
        case Operator::OR:      return "(||)";
        case Operator::NOT:     return "(not)";
        case Operator::LT:      return "(<)";
        case Operator::GT:      return "(>)";
        case Operator::LEQ:     return "(<=)";
        case Operator::GEQ:     return "(>=)";
        case Operator::EQ:      return "(=)";
        case Operator::NEQ:     return "(<>)";
        case Operator::NEGATE:  return "(- [unary])";
        case Operator::PLUS:    return "(+)";
        case Operator::MINUS:   return "(-)";
        case Operator::DIVIDES: return "(/)";
        case Operator::TIMES:   return "(*)";
        case Operator::POW:     return "(^)";
        default:                return "(unknown_operator)";
    }
  }

  void typeCheck_Logical(TypeInfo* left, Operator op, TypeInfo* right) {
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

      this->type = new TypeInfo(BaseType::NONE);
    }
  }

  
  void typeCheck_Arithmetic(TypeInfo* left, Operator op, TypeInfo* right) {
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

    bool left_valid  = (left->b_type == BaseType::INT || left->b_type == BaseType::FLOAT);
    bool right_valid = (right->b_type == BaseType::INT || right->b_type == BaseType::FLOAT);

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

  void typeCheck_Relational(TypeInfo* left, Operator op, TypeInfo* right) {
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

  void typeCheck_Equality(TypeInfo* left, Operator op, TypeInfo* right) {
    if ((left->b_type == BaseType::INT or left->b_type == BaseType::FLOAT) and
        (right->b_type == BaseType::INT or right->b_type == BaseType::FLOAT))
      this->type_ok = true;
    else
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
};

class ExpressionList : public SemanticAction {
public:
    std::vector<Expression*> exp_list;
    
    ExpressionList() = default;

    void add(Expression* exp) {
      this->exp_list.push_back(exp);
    }

    ~ExpressionList() {
      for (auto e : exp_list) delete e;
    }
};

class Variable : public SemanticAction {
public:
  TypeInfo* type;

  /*
   * Handling the reference of a simple variable, with no access tail.
   */
  Variable(SymbolTable* symtab, std::string name) {
    Symbol* sym = symtab->lookup(name);

    if (sym == nullptr) {
      std::cerr << "[ERROR] The name ´"
                << name
                << "´ isn't declared anywhere in this scope.\n";
      this->type = new TypeInfo(BaseType::NONE);
      this->type_ok = false;
      return;
    }

    if (sym->kind != SymbolKind::VARIABLE and sym->kind != SymbolKind::PARAMETER) {
      std::cerr << "[ERROR] The name `"
                << name
                << "` doesn't refer to a variable or a parameter.\n";
      this->type_ok = false;
      this->type = new TypeInfo(BaseType::NONE);
      return;
    }

    this->type_ok = true;
    this->type = &sym->type;
  }

  /*
  * Handling the case of accessing a field from a struct.
  * The struct may come by an expression, so we receive it.
  */
  Variable(SymbolTable* symtab, Expression* exp, std::string name) {
    if (exp->type->b_type != BaseType::STRUCT) {
      std::cerr << "[ERROR] Trying to use dot notation on a non-struct object.\n";

      this->type_ok = false;
      this->type = new TypeInfo(BaseType::NONE);

      return;
    }
    
    Symbol* sym = symtab->lookup(exp->type->struct_name);

    if (sym == nullptr) {
      std::cerr << "[ERROR] Invalid struct: ´"
                << exp->type->struct_name
                << "´ isn't declared as a struct anywhere in this scope.\n";

      this->type_ok = false;
      this->type = new TypeInfo(BaseType::NONE);

      return;
    }

    for (auto field : sym->parameters) {
      if (name == field.first) {
        this->type_ok = true;
        this->type = new TypeInfo(field.second);

        return;
      }
    }

    std::cerr << "[ERROR] Invalid access to struct field: the field `"
              << name 
              << "` doesn't exist in the struct `"
              << exp->type->struct_name << "`.\n";

    this->type_ok = false;
    this->type = new TypeInfo(BaseType::NONE);
  }
};

class Reference : public SemanticAction {
public:
  TypeInfo* type;

  Reference(TypeInfo* base_type) {
    if (base_type->b_type == BaseType::NONE) {
      std::cerr << "[ERROR] Cannot create reference to an invalid type.\n";
      this->type_ok = false;
      this->type = new TypeInfo(BaseType::NONE);
      return;
    }

    this->type_ok = true;
    this->type = new TypeInfo(BaseType::REFERENCE, *base_type);
  }
};

class Dereference : public SemanticAction {
public:
  TypeInfo* type;

  Dereference(TypeInfo* type) {
    if (type->b_type != BaseType::REFERENCE) {
      std::cerr << "[ERROR] Cannot dereference a type that isn't a reference.\n";
      this->type_ok = false;
      this->type = new TypeInfo(BaseType::NONE);
      return;
    }

    this->type_ok = true;
    this->type = type->ref_base.get();
  }
};

// ---- Statements ----

class Statement : public SemanticAction {
public:
  bool has_return = false;
  TypeInfo* return_type = new TypeInfo(BaseType::NONE);

  Statement() = default;
  virtual ~Statement() { delete return_type; }
};

class StatementList : public Statement {
public:
  std::vector<Statement*> statements;

  StatementList() = default;

  ~StatementList() {
    for (auto s : statements) delete s;
  }

  void add(SymbolTable* symtab, Statement* statement) {
    if (statement->has_return) {
      if (not this->has_return) {
        this->has_return = statement->has_return;
        this->return_type = new TypeInfo(*statement->return_type);
      }

      else if (*statement->return_type != *this->return_type) {
        if (not is_ValidCoercion(*statement->return_type, *this->return_type)) {
          std::cerr << "[ERROR] Inconsistent return types in function `"
                      << symtab->current()->name
                      << "`. Previously got `"
                      << *this->return_type
                      << "`, now got `"
                      << *statement->return_type
                      << "`.\n";

          this->type_ok = false;
        }
      }
    }

    statements.push_back(statement);
  }

  void verify_return(ProcedureDecl* declaration) {
    this->type_ok = true;

    if (declaration->return_type->b_type != BaseType::NONE and not this->has_return) {
      std::cerr << "[ERROR] Function `"
                << declaration->name
                << "` is expected to return a value of type `"
                << *declaration->return_type
                << "` but no return statement was found.\n";

      this->type_ok = false;
      return;
    }

    TypeInfo* r_type = nullptr;
    for (auto statement: statements)
      if (statement->has_return) { 
        if (*statement->return_type != *declaration->return_type) {
          if (not is_ValidCoercion(*declaration->return_type, *statement->return_type)) {
            std::cerr << "[ERROR] Function `"
                      << declaration->name
                      << "` is declared to return `"
                      << *declaration->return_type
                      << "`, but returns `"
                      << *statement->return_type
                      << "`.\n";

            this->type_ok = false;
            continue;
          }
        }

        r_type = new TypeInfo(*statement->return_type);
      }
    }
};

class Call : public Statement {
public:
  TypeInfo* type;

  Call(SymbolTable* symtab, std::string f_name, ExpressionList* exp_list) {
    Symbol* fun = symtab->lookup(f_name);

    if (fun == nullptr) {
      std::cerr << "[ERROR] Call to non-declared function `"
                << f_name
                << "`.\n";
      
      this->type_ok = false;
      this->type = new TypeInfo(BaseType::NONE);
      return;
    }

    if (fun->kind != SymbolKind::FUNCTION) {
      std::cerr << "[ERROR] `"
                << f_name
                << "` is not a function.\n";

      this->type_ok = false;
      this->type = new TypeInfo(BaseType::NONE);
    }

    const auto& param_types = fun->parameters;
    const auto& arg_exprs = exp_list->exp_list;

     if (param_types.size() != arg_exprs.size()) {
      std::cerr << "[ERROR] Function `" << f_name << "` expects " << param_types.size()
                << " arguments, but got " << arg_exprs.size() << ".\n";
      this->type_ok = false;
      this->type = new TypeInfo(BaseType::NONE);
      return;
    }

    for (size_t i = 0; i < param_types.size(); ++i) {
      if (!(*arg_exprs[i]->type == param_types[i].second)) {
        if (not is_ValidCoercion(*arg_exprs[i]->type, param_types[i].second)) {
          std::cerr << "[ERROR] Argument " << i + 1 << " of call to `" << f_name
                    << "` has type `" << *arg_exprs[i]->type << "`, expected `"
                    << param_types[i].second << "`.\n";
          this->type_ok = false;
          this->type = new TypeInfo(BaseType::NONE);
          return;
        }
      }
    }

    // If we reached here, the call is valid
    this->type_ok = true;
    this->type = new TypeInfo(fun->type);  // Copy return type
  }
};

class AssignStatement : public Statement {
public:
  AssignStatement(Variable* var, Expression* exp) {
    if (*var->type != *exp->type) {
      if (not is_ValidCoercion(*var->type, *exp->type)) {
        std::cerr << "[ERROR] Type error on assignment: expected `"
                  << *var->type
                  << "`, got `"
                  << *exp->type
                  << "`.\n";
        this->type_ok = false;
        return;
      }
    }

    this->type_ok = var->type_ok and exp->type_ok;
  }

  AssignStatement(Dereference* deref, Expression* exp) {
    if (*deref->type != *exp->type) {
      if (not is_ValidCoercion(*deref->type, *exp->type)) {
        std::cerr << "[ERROR] Type error on assignment: expected `"
                  << *deref->type
                  << "`, got `"
                  << *exp->type
                  << "`.\n";
        this->type_ok = false;
        return;
      }
    }

    this->type_ok = deref->type_ok and exp->type_ok;
  }
};

class ForStatement : public Statement {
public:
  ForStatement(SymbolTable* symtab, std::string name, Expression* eq, Expression* to, Expression* step, StatementList* body) {
    this->type_ok = true;
    this->has_return = body->has_return;
    this->return_type = body->return_type;

    Symbol* sym = symtab->lookup(name);

    if (sym == nullptr) {
      std::cerr << "[ERROR] Name `"
                << name
                << "` is not declared in this scope.\n";
    }

    if (sym->type.b_type != BaseType::INT
        or eq->type->b_type != BaseType::INT
        or to->type->b_type != BaseType::INT
        or step->type->b_type != BaseType::INT) {
      std::cerr << "[ERROR] Expressions that define the loop stepping logic for must be of type `int`.\n";
      this->type_ok = false;
    }
  }
};

class WhileStatement : public Statement {
public:
  WhileStatement(Expression* condition, StatementList* body) {
    this->type_ok = condition->type_ok;
    this->has_return = body->has_return;
    this->return_type = body->return_type;

    if (condition->type->b_type != BaseType::BOOL) {

      std::cerr << "[ERROR] The condition that defines the while loop stop must be of type `bool`."
                << " Received `" << *condition->type << "`.\n";

      this->type_ok = false;
    }
  }
};

class DoUntilStatement : public Statement {
public:
  DoUntilStatement(Expression* condition, StatementList* body) {
    this->type_ok = condition->type_ok;
    this->has_return = body->has_return;
    this->return_type = body->return_type;

    if (condition->type->b_type != BaseType::BOOL) {

      std::cerr << "[ERROR] The condition that defines the do-until loop stop must be of type `bool`."
                << " Received `" << *condition->type << "`.\n";

      this->type_ok = false;
    }
  }
};

class IfStatement : public Statement {
public:
  IfStatement(SymbolTable* symtab, Expression* condition, StatementList* body, StatementList* else_body) {
    this->type_ok = condition->type_ok;
    this->has_return = body->has_return;
    this->return_type = body->return_type;

    if (not else_body->statements.empty()) {
      for (auto statement : else_body->statements)
        body->add(symtab, statement);
    }

    if (condition->type->b_type != BaseType::BOOL) {
      std::cerr << "[ERROR] The condition that defines the if statement branching must be of type `bool`."
                << " Received `" << *condition->type << "`.\n";

      this->type_ok = false;
    }
  }
};

class ReturnStatement : public Statement {
public:
  ReturnStatement() {
    this->type_ok = true;
    this->has_return = true;
    this->return_type = new TypeInfo(BaseType::NONE);
  }

  ReturnStatement(Expression* exp) {
    this->type_ok = exp->type_ok;
    this->has_return = true;
    this->return_type = new TypeInfo(*exp->type);
  }
};
