#pragma once

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "state.hpp"

/*
 * Base class for the semantic actions.
 * 
 * The attribute `type_ok` is set to false whenever there is a type error
 * in the semantic action, or in the ones that depend on it.
 *
 * That way we can propagate type errors up the parse tree.
 */
class SemanticAction {
protected:
  bool type_ok;

  /*
   * Helper method for a very commons situation: checking if
   * we can have a valid type coercion from type `l` to `r`.
   * There is only one case, converting int to float.
   */
  bool is_ValidCoercion(TypeInfo l, TypeInfo r) {
    return (l.b_type == BaseType::FLOAT and r.b_type == BaseType::INT);
  }

private:
  std::string gen;

public:
  void Generate(const std::string& gen) {
    this->gen = gen;
  }

  const std::string& Gen() {
    return gen;
  }

  const bool Ok() {
    return type_ok;
  }
};

// ---- DECLARATIONS ----

/*
 * Class encapsulating all the semantic actions associated with the declaration of a variable.
 */
class VarDecl : public SemanticAction {
public:
  /*
   * Constructor for two cases of variable declaration:
   *    * Just a declaration, no expression.
   *    * Or it could be the case of a type inference.
   * So it handles both something like `var x : int` and `var x := 5`.
   *
   * To handle both these cases, we must get the environment, i.e. the symbol table,
   * the name of the variable being declared and the declaration type, written or inferred.
   *
   * This action perform every possible check for declaring variables, checking if the type
   * exists (it can be non-existent in cases of structs!), checking if the symbol is already declared,
   * inserting it if correctly defined, and so on.
  */
  VarDecl(State* St, std::string name, TypeInfo decl_type) {
    this->type_ok = true;

    // Checking if type exists, in case of an invalid struct.
    if (not decl_type.struct_name.empty() and not St->Table()->lookup(decl_type.struct_name)) {
        std::cout << "[ERROR] In variable ´"
                  << name << "` declaration: `"
                  << decl_type.struct_name << "´ is not a declared type in the current scope.\n";
        this->type_ok = false;
        decl_type.b_type = BaseType::NONE;
    }

    Symbol sym(name, SymbolKind::VARIABLE, decl_type);

    // If we could not insert, it means that there is already a symbol declared
    // in the same scope.
    if (not St->Table()->insert(sym)) {
      std::cout << "[ERROR] In variable ´"
                << name << "´ declaration: symbol `"
                << name << "` is already declared in the current scope.\n";
      this->type_ok = false;
    }

    this->Generate(name);
  }

  /*
   * This constructor handles the case of a declaration that contains both a written type declaration
   * and an expression assigned to it, namely something like `var x : int := 5`.
   *
   * It checks if the expected type is the same as the actual type (the type of the expression), or if
   * it's a valid coercion, and inserts the symbol if it's not already defined.
   */
  VarDecl(State* St, std::string name, TypeInfo decl_type, TypeInfo actual_type) {
    this->type_ok = true;

    // Checking if type is a valid type in case of it being a struct type.
    if (not decl_type.struct_name.empty() and not St->Table()->lookup(decl_type.struct_name)) {
        std::cout << "[ERROR] In variable ´"
                  << name << "` declaration: `"
                  << decl_type.struct_name << "´ is not a declared type in the current scope.\n";
        this->type_ok = false;
        decl_type.b_type = BaseType::NONE;
    }

    // Type checking the expected type agains the actual type, considering the possibilty
    // of a valid coercion.
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

    // If we could not insert the symbol, it is already declared under the current scope.
    if (not St->Table()->insert(sym)) {
      std::cout << "[ERROR] In variable ´"
                << name << "´ declaration: symbol `"
                << name << "` is already declared in the current scope.\n";
      this->type_ok = false;
    }

    this->Generate(name);
  }
};

/*
 * Class to store parameters declarations of functions and structs.
 *
 * We just store the name and type of a given parameter, for posterior
 * allocation in the symbol table, so it has only a very simple constructor.
 */
class ParameterDecl : public SemanticAction {
public:
  std::string name;
  TypeInfo* type;

  ParameterDecl(std::string name, TypeInfo* type)
    : name(name), type(type)
  {}
};

/*
 * Class to store a list of parameter declaration for functions and structs.
 *
 * It stores pointers to `ParameterDecl` in a vector.
 * This action correspond to sequences of parameters that appear on a struct and
 * function declaration, like in `f(x: int, y: float)`.
 */
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

  std::string Gen() {
    std::ostringstream gen;

    for (auto f : fields) {
      gen << "\t" << f->type->Gen() << " " << f->name << ";\n";
    }

    return gen.str();
  }
};

/*
 * Class to handle the semantic actions associated with struct declarations.
 *
 * It simply builds the symbol correctly for the struct, adding the parameters,
 * which are stored via a pointer to a `ParameterField` object, and declares it in case it's not already declared.
 */
class StructDecl : public SemanticAction {
public:
  std::string name;
  ParameterField* fields;

  /*
   * The only constructor we need, receiving a pointer to the symbol table,
   * a name for the new struct and a pointer to a `ParameterField` object,
   * which contains the names and types of each of its parameters.
   *
   * It then adds the parameters to the symbol struct so we can refer to it later.
   *
   * Then it tries to add the type to the symbol table, registering an error
   * in case we already have a symbol with the same name declared.
   */
  StructDecl(State* St, std::string name, ParameterField* fields) {
    this->type_ok = true;

    TypeInfo struct_type(BaseType::STRUCT, name);

    Symbol symbol(name, SymbolKind::STRUCT, struct_type);

    // Adding the parameters to the type information.
    for (const auto& field : fields->fields) {
      symbol.parameters.push_back({field->name, *field->type});
    }

    // Try to add, flag error if it already exists.
    if (not St->Table()->insert(symbol)) {
      std::cerr << "[ERROR] In struct `"
                << name
                << "` declaration: the symbol `" << name << "` is already declared in the current scope.\n";
      this->type_ok = false;
    }
  }
};

/*
 * This class handles the semantic actions associated with the declaration of
 * procedures, i.e. functions.
 *
 * It takes all the information contained on the signature of a function, namely its name,
 * its parameters names and types, its return type (if any) and validates it.
 *
 * It then sets its return type (setting to NONE in case of "void" procedures), and tries to declare
 * it in the symbol table.
 *
 * This class also has a method for declaring all of its parameters in its scope after we push
 * the procedure symbol in the symbol table.
 */
class ProcedureDecl : public SemanticAction {
public:
  std::string name;
  TypeInfo* return_type;

  /*
   * The only constructor, receiving a pointer to the symbol table, the procedure name, its parameters,
   * coming from a `ParameterField` object and its return type, which may be `NONE` if it's a "void" procedure.
   *
   * It builds the symbol for the function, register all of it's parameters and tries to push it into the symbol table.
   */
  ProcedureDecl(State* St, std::string name, ParameterField* params, TypeInfo* return_type) {
    this->type_ok = true;

    Symbol sym(name, SymbolKind::FUNCTION, *return_type);

    // Saving its parameters.
    for (auto param : params->fields)
      sym.parameters.push_back({param->name, *param->type});

    if (not St->Table()->insert(sym)) {
      std::cerr << "[ERROR] In declaration of procedure `"
                << name
                << "`: symbol `"
                << name << "` is already declared in this scope.\n";
      this->type_ok = false;
    }

    this->name = name;
    this->return_type = new TypeInfo(*return_type);
  }

  /*
   * This method pushes all the parameters of the procedure being declared into the
   * symbol table, so we can properly refer to them in posterior type analysis.
   *
   * This is a separate operation from the declaration since it must happen after
   * a new scope is pushed into the symbol table, namely the procedure scope.
   *
   * The action of pushing a new scope into the table is done in the yacc parser,
   * and this method is called in sequence.
   */
  void declare_params_in_scope(State* St, ParameterField* params) {
    for (auto param : params->fields) {
      Symbol sym(param->name, SymbolKind::PARAMETER, *param->type);

      // If we can't insert the symbol, it means there are two
      // or more parameters with the same name, since its a fresh scope
      // with no other symbols other than the parameters at this point.
      if (not St->Table()->insert(sym)) {
        std::cerr << "[ERROR] Redeclaration of parameter `"
                  << sym.name
                  << "` in procedure `"
                  << this->name
                  << "` declaration.\n";
        this->type_ok = false;
      }
    }
  }
};

// ---- Expressions ----

/*
 * This is the most important class for type checking: the semantic action
 * for expressions.
 *
 * In this class we'll validate the type of any possible expression in our language,
 * be it arithmetic, logical, relational, reference and struct-like.
 *
 * This is the backbone of the type checker of the language. Each of its
 * constructors handle a case of expression and sets its type, flagging type errors
 * when they happen.
 *
 * In case of type errors, the attribute `type_ok` is set to false, and any
 * expression that depends on the one that got an error will also invalidate
 * this attribute.
 *
 * Also, the type is set to `NONE`, so it pops up errors climbing up the parse tree.
 */
class Expression : public SemanticAction {
public:
  TypeInfo* type; // The type of the expression
  std::string t_var; // For the 3AC pattern: the temporary variable in which the expression is stored.

  /*
   * A simple enum to indicate which operator is being used
   * in the cases where the rule calling the constructor comes from
   * an operator.
   */
  enum class Operator {
    AND, OR, NOT,
    LT, GT, LEQ, GEQ, EQ, NEQ,
    NEGATE,
    PLUS, MINUS, DIVIDES, TIMES, POW
  };

  // Default constructor, testing purposes.
  Expression() {}

  // Setting expression from base types, testing purposes.
  Expression(BaseType t)
  { this->type = new TypeInfo(t);}

  /*
   * This constructor handles the case of literals, references, dereferences and variables,
   * in which the type information (and checking) comes from other rules, so we dont't have
   * to do anything else here.
   */
  Expression(TypeInfo* type, bool ok, const std::string& gen)
    : type(type)
  {
    this->type_ok = ok;
    this->Generate(gen);
  }

  /*
   * This constructor handles the case of instantiating a struct with the `new name` pattern.
   *
   * It simply checks if the name refers to a valid struct type, performing the only type
   * validation possible in this case.
   *
   * It receives the symbol table and the name to do so.
   */
  Expression(State* St, std::string name) {
    Symbol* sym = St->Table()->lookup(name);
    
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
    this->Generate(name + "{}");
  }

  /*
   * This constructor handles the case of unary operators: not and unary minus.
   *
   * It simply receives the enum operarator to know which called the constructor,
   * and the type of the operand.
   *
   * It then type checks the expression according to the rules below.
   */
  Expression(Operator op, Expression* operand) {
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

  /*
   * This constructor handles the case of all the binary operators based expressions.
   * 
   * It receives both operands types and the enum indicating which operator was used.
   *
   * It then calls the appropriate method for handling the expression, the possible cases
   * being a logical expression, an arithmetic, a relational and one involving equalities.
   *
   * Since each of these cases of expressions involves different type checking mechanisms,
   * it's good to handle them separately, keeping the code organized and decoupled.
   */
  Expression(Expression* left, Operator op, Expression* right) {
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

private:
  /*
   * Simple method to convert the operator to string so we can
   * have a better error log.
   */
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
  
  /*
   * Simple method to convert the operator to string so we can
   * add it to the code generation.
   * Differs from the above on the absence of parenthesis.
   */
  std::string op_toString_Gen(Operator op) {
    switch (op) {
        case Operator::AND:     return " && ";
        case Operator::OR:      return " || ";
        case Operator::NOT:     return "!";
        case Operator::LT:      return " < ";
        case Operator::GT:      return " > ";
        case Operator::LEQ:     return " <= ";
        case Operator::GEQ:     return " >= ";
        case Operator::EQ:      return " == ";
        case Operator::NEQ:     return " != ";
        case Operator::NEGATE:  return "-";
        case Operator::PLUS:    return " + ";
        case Operator::MINUS:   return " - ";
        case Operator::DIVIDES: return " / ";
        case Operator::TIMES:   return " * ";
        default:                return "";
    }
  }

  /*
   * This method performs the type checking of logical operator based expressions,
   * namely the ones in which the operator is either `(&&)` or `(||)`.
   *
   * Both operands must be of type bool.
   * If this is not the case, a type error is reported back to the parser,
   * and the type of the expression becomes ill-formed.
   */
  void typeCheck_Logical(TypeInfo* left, Operator op, TypeInfo* right) {
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

  /*
   * This method performs the type checking of arithmetic expressions.
   *
   * It performs the type checking with respect to the possible conversions,
   * and sets the correct type of the resulting expression for further use.
   *
   * If the operator is `(^)`, the resulting type is always float.
   * If it's one of the remaining operators, the type is, of course, numeric,
   * but depends on the type of the operands.
   * 
   * Basically, if a float is present in any of the operands, the resulting
   * type is a float, respecting the type coercion rule.
   *
   * The validity of these expressions depends if both operands are of a numeric type.
   */
  void typeCheck_Arithmetic(TypeInfo* left, Operator op, TypeInfo* right) {
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

  /*
   * This method performs the type checking of relational expressions.
   *
   * It receives the operator only for log purposes, as they all have the same type rules.
   *
   * For a type valid relational expression, both operands must be of a numeric type,
   * else we don't know how to compare them. This is precisely what this method checks.
   * We don't require the operands to be of the same exact type, as we can safely compare
   * between int and bool.
   *
   * A valid relational expression will be set to have type bool.
   */
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

  /*
   * This method performs the type checking of (in)equality based expressions.
   *
   * This is separate from the relational expressions even though they are both
   * classified as the same type of expressions because it has slightly different
   * type checking rules.
   *
   * We can compare freely between any two types that are equal, which is not the case
   * for relational operators (references can be compared as equals, but we can't have a notion of which is lesser).
   *
   * Also, if the operands are of numeric type, we can compare them even if they are of different types.
   * 
   * The type of a well-formed equality expression is set to have type bool.
   */
  void typeCheck_Equality(TypeInfo* left, Operator op, TypeInfo* right) {
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
};

/*
 * This class is a semantic action that simply stores a list of expressions.
 *
 * This is invoked whenever we have a call statement, as the list of expressions
 * is used as the actual parameters of the call, as of in `f(x + 5, true, ref(i))`.
 */
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

/*
 * This is the class that implements the semantic actions associated with referring to a variable
 * or struct name in any appropriate place, namely on assign statements and expressions.
 *
 * For example, this is invoked for `x` whenever we have `x := ...` or `... := ... x ...`.
 * Also, `struct.field` is a legal case of this rule, and any case of `x` above is also valid
 * for this one.
 *
 * For the semantic actions, we must verify that the name refers to an existing variable,
 * parameter, or struct and field.
 *
 * Also, if access notation is used, we must verify that the expression before the dot denotes an
 * existing struct.
 *
 * The type of this action will be set as the type of the variable being accessed, if correctly accessed.
 */
class Variable : public SemanticAction {
public:
  TypeInfo* type;

  /*
   * This constructor handles the case of a simple variable access, with no dot notation
   * for accessing fields of structs.
   *
   * In this case, we simply look for the name in the symbol table, and check that it
   * refers to a variable or parameter, flagging error otherwise.
   *
   * As always, in case of errors the type will be set to `NONE`.
   */
  Variable(State* St, std::string name) {
    Symbol* sym = St->Table()->lookup(name);

    // Is it declared in any way?
    if (sym == nullptr) {
      std::cerr << "[ERROR] The name ´"
                << name
                << "´ isn't declared anywhere in this scope.\n";
      this->type = new TypeInfo(BaseType::NONE);
      this->type_ok = false;
      return;
    }

    // If it's declared, is it a variable or parameter?
    // We could have recovered a function symbol, for example.
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
    this->Generate(name);
  }

  /*
   * This constructor handles the case of accessing a field from a struct.
   *
   * The struct may come as an expression, so we receive a pointer to `Expression` as argument.
   *
   * We then check if the expression has type struct, and check if the struct is declared in the
   * current scope.
   *
   * Lastly, we check if the field we're acessing really is a field from our struct.
   */
  Variable(State* St, Expression* exp, std::string name) {
    if (exp->type->b_type != BaseType::STRUCT) {
      std::cerr << "[ERROR] Trying to use dot notation on a non-struct object.\n";

      this->type_ok = false;
      this->type = new TypeInfo(BaseType::NONE);

      return;
    }
    
    Symbol* sym = St->Table()->lookup(exp->type->struct_name);

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
        this->Generate(exp->Gen() + "." + name);

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

/*
 * This class implements the semantic action of creating a reference to a variable in
 * any appropriate place, namely as an expression.
 *
 * It is invoked whenever `ref(...)` is parsed, where what goes inside of the brackets
 * must be a variable or struct field access.
 *
 * For the semantic actions, we receive the type of the argument to the `ref`. We then check if the
 * type is valid, namely if something went wrong with the variable access semantic action.
 *
 * This is a good example of dependency in our rules.
 *
 * The type of the reference will then be a reference type that carries as the base type
 * the type of its argument. For example `ref(x)` will be of type "reference of type of x".
 */
class Reference : public SemanticAction {
public:
  TypeInfo* type;

  Reference(Variable* var) {
    // If anything went wrong already with the argument.
    if (var->type->b_type == BaseType::NONE) {
      std::cerr << "[ERROR] Cannot create reference to an invalid type.\n";
      this->type_ok = false;
      this->type = new TypeInfo(BaseType::NONE);
      return;
    }

    this->type_ok = true;
    this->type = new TypeInfo(BaseType::REFERENCE, *var->type);
    this->Generate("&" + var->Gen());
  }
};

/*
 * This class implements the semantic action of deferencing something in any appropriate places,
 * namely as expressions or as right-hand sides of assignment statements.
 *
 * It is invoked whenever `deref(...) := exp` happens, or as a regular expression.
 * Note that `deref(deref(...))` is valid, as long as the type of the argument is appropriate.
 *
 * For the semantic actions, we simply receive the argument and check if
 * its type is a reference, since that's the only valid case for a dereferencing.
 * The argument can be either a variable or another dereference.
 * 
 * We then set the type of this action to be the type that the reference was pointing to.
 */
class Dereference : public SemanticAction {
public:
  TypeInfo* type;

  Dereference(Variable* var) {
    if (var->type->b_type != BaseType::REFERENCE) {
      std::cerr << "[ERROR] Cannot dereference a type that isn't a reference.\n";
      this->type_ok = false;
      this->type = new TypeInfo(BaseType::NONE);
      return;
    }

    this->type_ok = true;
    this->type = type->ref_base.get();
    this->Generate("*" + var->Gen());
  }

  Dereference(Dereference* deref) {
    if (deref->type->b_type != BaseType::REFERENCE) {
      std::cerr << "[ERROR] Cannot dereference a type that isn't a reference.\n";
      this->type_ok = false;
      this->type = new TypeInfo(BaseType::NONE);
      return;
    }

    this->type_ok = true;
    this->type = type->ref_base.get();
    this->Generate("*" + deref->Gen());
  }
};

// ---- Statements ----


/*
 * Here we have the base class for all the semantic actions involving statements.
 *
 * It has the `type_ok` attribute since it's a subclass of 'SemanticAction', and
 * two more new attributes.
 *
 * The `has_return` is a bool that indicates if the current statement has a return, namely if
 * inside it's body there is a return statement.
 *
 * Then, the `return_type` tells us precisely what's the type of the expression in the
 * return statement, if any.
 *
 * These informations will both be used to type-check the declaration of functions, checking
 * if the correct types are being returned, and if anything is being returned at all.
 */
class Statement : public SemanticAction {
public:
  bool has_return = false;
  TypeInfo* return_type = new TypeInfo(BaseType::NONE);

  Statement() = default;
  virtual ~Statement() { delete return_type; }
};

/*
 * This class implements semantic actions associated with list of statements.
 * These may be called bodies, or blocks, and contains statements in sequence.
 *
 * We have a vector of pointers to `Statement` that'll be populated whenever we parse
 * the body of any statement (or procedure).
 *
 * There are some extra semantic actions associated with this. Since statements can have return types,
 * and this one is also considered a statement, we'll check the return types of its children and based
 * on that, set this one's return type.
 *
 * With this approach, we can check if there are return statements and types in the body, and
 * check for inconsistent return types, as we'll see below.
 */
class StatementList : public Statement {
public:
  std::vector<Statement*> statements;

  StatementList() = default;

  ~StatementList() {
    for (auto s : statements) delete s;
  }

  /*
   * This method adds a statement to the list and check for inconsistent return types.
   *
   * If the statement being added to the list has a return type, this method checks if
   * the current statement list already has a return type. 
   * 
   * If not, it'll set its return type to the return type of the statement being added.
   *
   * But if a return type is already set, it compares with the return type of the statement,
   * flagging error if they are different (with respect to type coercions).
   *
   * For example, if the statement list is `if ... then {...} else {...}`, both bodies
   * must have the same return type, or its flagged as inconsistent.
   *
   * This is the approach chosen for this compiler, so to avoid incosistent returns in
   * nested blocks and branching.
   */
  void add(State* St, Statement* statement) {
    if (statement->has_return) {
      // If this body has no return, set.
      if (not this->has_return) {
        this->has_return = statement->has_return;
        this->return_type = new TypeInfo(*statement->return_type);
      }

      // If it has, compare.
      else if (*statement->return_type != *this->return_type) {
        if (not is_ValidCoercion(*statement->return_type, *this->return_type)) {
          std::cerr << "[ERROR] Inconsistent return types in function `"
                      << St->Table()->current()->name
                      << "`. Previously got `"
                      << *this->return_type
                      << "`, now got `"
                      << *statement->return_type
                      << "`.\n";

          // Keep the return type as it was.
          this->type_ok = false;
        }
      }
    }

    statements.push_back(statement);
  }

  /*
   * This method performs the verification of the return type of functions.
   *
   * It receives a pointer to the declaration, i.e. its signature.
   * Since functions are composed of its signatures, a block of declaration and a body of statements,
   * this semantic action will be called whenever we parse the body, so it can properly verify the
   * return type of the function.
   *
   * We first check if, according to the signature, the function is supposed to have a return type.
   * If yes and we got no return in the body, we flag an error of missing return.
   *
   * Then, if the body has a return, we check every return type of every statement, and type checks
   * against the return type of the signature, flagging errors if the types are incompatible.
   */
  void verify_return(ProcedureDecl* declaration) {
    this->type_ok = true;

    // If the function is non-void and the body has no returns.
    if (declaration->return_type->b_type != BaseType::NONE and not this->has_return) {
      std::cerr << "[ERROR] Function `"
                << declaration->name
                << "` is expected to return a value of type `"
                << *declaration->return_type
                << "` but no return statement was found.\n";

      this->type_ok = false;
      return;
    }

    // Checks every type of every return.
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
      }
    }
};

/*
 * This class implements the semantic actions associated with the calling of a function.
 *
 * It must check if the callee exists as a function, set a type if it has a return type.
 *
 * It also checks the number and types of the actual parameters passed, so to check if the call
 * is well-formed.
 *
 * If the call is in any way ill-formed, its type is set to `NONE`. This will mainly be useful
 * in the type checkin of expressions, since the call of a function is considered to be a valid
 * expression case.
 */
class Call : public Statement {
public:
  TypeInfo* type;

  /*
   * This constructor is the only one and implements the semantic actions described above.
   */
  Call(State* St, std::string f_name, ExpressionList* exp_list) {
    Symbol* fun = St->Table()->lookup(f_name);

    // Check if the callee exists.
    if (fun == nullptr) {
      std::cerr << "[ERROR] Call to non-declared function `"
                << f_name
                << "`.\n";
      
      this->type_ok = false;
      this->type = new TypeInfo(BaseType::NONE);
      return;
    }

    // Check if the callee is a function.
    if (fun->kind != SymbolKind::FUNCTION) {
      std::cerr << "[ERROR] `"
                << f_name
                << "` is not a function.\n";

      this->type_ok = false;
      this->type = new TypeInfo(BaseType::NONE);
    }

    const auto& param_types = fun->parameters;
    const auto& arg_exprs = exp_list->exp_list;

    // Check if the actual parameters were all passed.
    if (param_types.size() != arg_exprs.size()) {
      std::cerr << "[ERROR] Function `" << f_name << "` expects " << param_types.size()
                << " arguments, but got " << arg_exprs.size() << ".\n";
      this->type_ok = false;
      this->type = new TypeInfo(BaseType::NONE);
      return;
    }

    // Checks the types of the actual parameters against the formal ones.
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

/*
 * This class implements the semantic actions associated with an assignment statement.
 *
 * Assignment statements come in the following forms:
 *    * `x := exp`
 *    * `struct.field := exp`
 *    * `deref(...) := exp`
 *
 * We'll handle each case accordingly.
 *
 * Mainly we need to check if the expression type is compatible with the symbol type we're
 * trying to assign to.
 *
 * We don't need to check for existence of the symbols since each left-hand side
 * of the assignment correspond to a different rule, a different semantic action
 * which will properly check the existence.
 * If it doesn't exist, the type will be `NONE` and we'll propagate errors.
 */
class AssignStatement : public Statement {
public:

  /*
   * This constructor handle the cases where the left-hand side corresponds
   * to an instance of the `var` rule.
   */
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

    this->type_ok = var->Ok() and exp->Ok();
    this->Generate(var->Gen());
  }

  /*
   * This constructor handle the cases where the left-hand side corresponds
   * to an instance of the `deref` rule.
   */
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

    this->type_ok = deref->Ok() and exp->Ok();
    this->Generate(deref->Gen());
  }
};


/*
 * This class implements the semantic actions associated with the for-loop statement.
 *
 * Mainly what we want to do is to check if the variable and the expressions defining the loop counter,
 * stop condition and step are all of the correct type, namely `int`.
 *
 * We also parse the body of the loop, its list of statements, to check for return statements and consequently return types,
 * so we can type check in the future.
 *
 * The return type of this rule will be set as the return type of its body.
 */
class ForStatement : public Statement {
public:
  ForStatement(State* St, std::string name, Expression* eq, Expression* to, Expression* step, StatementList* body) {
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
    }
  }
};

/*
 * This class implements the semantic actions associated with the while-loop statement.
 *
 * Mainly what we want to do is to check if expression defining the loop condition is of type `bool`.
 *
 * We also parse the body of the loop, its list of statements, to check for return statements and consequently return types,
 * so we can type check in the future.
 *
 * The return type of this rule will be set as the return type of its body.
 */
class WhileStatement : public Statement {
public:
  WhileStatement(Expression* condition, StatementList* body) {
    this->type_ok = condition->Ok();
    this->has_return = body->has_return;
    this->return_type = body->return_type;

    if (condition->type->b_type != BaseType::BOOL) {

      std::cerr << "[ERROR] The condition that defines the while loop stop must be of type `bool`."
                << " Received `" << *condition->type << "`.\n";

      this->type_ok = false;
    }
  }
};

/*
 * This class implements the semantic actions associated with the do-until-loop statement.
 *
 * Mainly what we want to do is to check if expression defining the loop condition is of type `bool`.
 *
 * We also parse the body of the loop, its list of statements, to check for return statements and consequently return types,
 * so we can type check in the future.
 *
 * The return type of this rule will be set as the return type of its body.
 */
class DoUntilStatement : public Statement {
public:
  DoUntilStatement(Expression* condition, StatementList* body) {
    this->type_ok = condition->Ok();
    this->has_return = body->has_return;
    this->return_type = body->return_type;

    if (condition->type->b_type != BaseType::BOOL) {

      std::cerr << "[ERROR] The condition that defines the do-until loop stop must be of type `bool`."
                << " Received `" << *condition->type << "`.\n";

      this->type_ok = false;
    }
  }
};

/*
 * This class implements the semantic actions associated with the if statement.
 *
 * Mainly what we want to do is to check if expression defining the if condition is of type `bool`.
 *
 * We also parse the body of the loop, its list of statements, to check for return statements and consequently return types,
 * so we can type check in the future. We take into consideration the body of the else clause, if it has one.
 * If there is no else clause, the list of statements will come as empty, and we don't need to check against it.
 *
 * In case of an else clause, to check for inconsistent return types, we just add the statements of its body
 * to the body of the if. The method will check for us if there are inconsistencies.
 *
 * The return type of this rule will be set as the return type of its body.
 */
class IfStatement : public Statement {
public:
  IfStatement(State* St, Expression* condition, StatementList* body, StatementList* else_body) {
    this->type_ok = condition->Ok();
    this->has_return = body->has_return;
    this->return_type = body->return_type;

    if (not else_body->statements.empty()) {
      for (auto statement : else_body->statements)
        body->add(St, statement);
    }

    if (condition->type->b_type != BaseType::BOOL) {
      std::cerr << "[ERROR] The condition that defines the if statement branching must be of type `bool`."
                << " Received `" << *condition->type << "`.\n";

      this->type_ok = false;
    }
  }
};

/*
 * This class implements the semantic actions associated with the return statement.
 *
 * This is the backbone of the return types of the statements. Reaching this statement means that we'll be
 * able to fill in the return type with something and pass it up the tree to the enclosing statements.
 *
 * If it's just an early return, the type is set to `NONE`. This is the case of `return` without an expression.
 *
 * But if there is an expression, the return type will be set to be the type of the expression, and the `type_ok`
 * verification flag to be the same of its expression (meaning if the expression is not well-typed, the return won't be either).
 */
class ReturnStatement : public Statement {
public:
  /*
   * This constructor handles the case of early returns, without expressions.
   *
   * The `type_ok` is always true in this case since there are no chance of ill-formed type
   * if there are no expressions.
   */
  ReturnStatement() {
    this->type_ok = true;
    this->has_return = true;
    this->return_type = new TypeInfo(BaseType::NONE);
  }
  
  /*
   * This constructor handles the case of returns with expressions.
   *
   * In this case we check if the expression is well-typed.
   */
  ReturnStatement(Expression* exp) {
    this->type_ok = exp->Ok();
    this->has_return = true;
    this->return_type = new TypeInfo(*exp->type);
  }
};
