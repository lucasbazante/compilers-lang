#pragma once

#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "state.hpp"
#include "symbol_table.hpp"

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
    /*
     * For the 3AC pattern: the internal representation in which the action is stored.
     * The representation can be a temporary variable or a raw value, like a literal.
     * Or it can be a name, access pattern, reference or dereferencing of something.
     *
     * In summary, it can store the internal representation of things, so we can access
     * them after code generation.
    */
    std::string repr;

public:
    virtual void Generate(State* St) {};
    ~SemanticAction() = default;

    const std::string& Repr() {
        return repr;
    }

    void Set_Repr(std::string repr) {
        this->repr = repr;
    }

    const bool Ok() {
        return type_ok;
    }
};

// Forward Declaration
class Expression;
class Variable;
class Reference;
class Dereference;

// ---- DECLARATIONS ----

/*
 * Class encapsulating all the semantic actions associated with the declaration of a variable.
 */
class VarDecl : public SemanticAction {
public:
    TypeInfo* type;
    Expression* expression;
    std::string name;

    /*
   * Both constructors are for two cases of variable declaration, respectively:
   *    * Just a declaration, no expression.
   *    * Or it could be the case of a type inference.
   * So they handle both something like `var x : int` and `var x := 5`.
   *
   * To handle both these cases, we must get the environment, i.e. the symbol table,
   * the name of the variable being declared and the declaration type, written or inferred.
   *
   * This action perform every possible check for declaring variables, checking if the type
   * exists (it can be non-existent in cases of structs!), checking if the symbol is already declared,
   * inserting it if correctly defined, and so on.
  */
    VarDecl(State* St, std::string name, TypeInfo* decl_type);

    VarDecl(State* St, std::string name, Expression* expression);

    /*
   * This constructor handles the case of a declaration that contains both a written type declaration
   * and an expression assigned to it, namely something like `var x : int := 5`.
   *
   * It checks if the expected type is the same as the actual type (the type of the expression), or if
   * it's a valid coercion, and inserts the symbol if it's not already defined.
   */
    VarDecl(State* St, std::string name, TypeInfo* decl_type, Expression* decl_exp);

    void Generate(State* St);
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
    ParameterField* paramfield;

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
    StructDecl(State* St, std::string name, ParameterField* paramfield);

    void Generate(State* St);
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
    ProcedureDecl(State* St, std::string name, ParameterField* params, TypeInfo* return_type);

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
    void declare_params_in_scope(State* St, ParameterField* params);
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

    TypeInfo* type; // The type of the expression

    /*
     * For further reference in code generation, we'll need to store this data.
    */
    Expression* lhs;
    Expression* rhs;
    Variable* var;
    Reference* ref;
    Dereference* deref;
    Operator op;

    /*
     * This constructor handles the case of literals.
     *
     * We'll just store the type, and the code generation will simply
     * output the literal received as text.
     *
     * There is no type checking in this case. A literal is always type-correct.
    */
    Expression(TypeInfo* type, const std::string& gen);

    /*
     * These constructors handle the case of the expression being one of
     * a variable, a reference or a dereference.
     *
     * In this case, the type checking is already done within the class of
     * each option.
     * Further type checking will just get the type of those.
    */
    Expression(Variable* var);

    Expression(Reference* ref);

    Expression(Dereference* deref);

    /*
   * This constructor handles the case of instantiating a struct with the `new name` pattern.
   *
   * It simply checks if the name refers to a valid struct type, performing the only type
   * validation possible in this case.
   *
   * It receives the symbol table and the name to do so.
   */
    Expression(State* St, std::string struct_name);

    /*
   * This constructor handles the case of unary operators: not and unary minus.
   *
   * It simply receives the enum operarator to know which called the constructor,
   * and the type of the operand.
   *
   * It then type checks the expression according to the rules below.
   */
    Expression(Operator op, Expression* operand);

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
    Expression(Expression* left, Operator op, Expression* right);

    void Generate(State* St);

private:

    /*
   * Helper method to handle different cases of code generation.
   */
    void GenerateUnary(State* St);
    void GenerateBinary(State* St);
    void GenerateOperand(State* St);

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
    void typeCheck_Logical(TypeInfo* left, Operator op, TypeInfo* right);

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
    void typeCheck_Arithmetic(TypeInfo* left, Operator op, TypeInfo* right);

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
    void typeCheck_Relational(TypeInfo* left, Operator op, TypeInfo* right);

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
    void typeCheck_Equality(TypeInfo* left, Operator op, TypeInfo* right);
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
    std::string name;
    Expression* struct_exp;

    /*
   * This constructor handles the case of a simple variable access, with no dot notation
   * for accessing fields of structs.
   *
   * In this case, we simply look for the name in the symbol table, and check that it
   * refers to a variable or parameter, flagging error otherwise.
   *
   * As always, in case of errors the type will be set to `NONE`.
   */
    Variable(State* St, std::string name);

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
    Variable(State* St, Expression* exp, std::string name);

    void Generate(State* St);
    
    void Internal_Generation(State* St);
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
    Variable* var;

    Reference(State* St, Variable* var);

    void Generate(State* St);

    void Internal_Generation(State* St);
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
    Variable* var;
    Dereference* deref;

    Dereference(State* St, Variable* var);

    Dereference(State* St, Dereference* deref);

    void Generate(State* St);

    void Internal_Generation(State* St);
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
    void add(State* St, Statement* statement);

    /*
     * In the case of an `if` with an `else`, to match the return types,
     * we add all the statements present in the `else` body, so we can capture
     * the return errors there.
     *
     * This method pops them out so the `if` body keeps its original statements.
     */
    void pop_added(int n);

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
    void verify_return(State* St, ProcedureDecl* declaration);

    void Generate(State* St);
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
    Call(State* St, std::string f_name, ExpressionList* exp_list);
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
    Variable* var;
    Dereference* deref;
    Expression* exp;

    /*
   * This constructor handle the cases where the left-hand side corresponds
   * to an instance of the `var` rule.
   */
    AssignStatement(State* St, Variable* var, Expression* exp);

    /*
   * This constructor handle the cases where the left-hand side corresponds
   * to an instance of the `deref` rule.
   */
    AssignStatement(State* St, Dereference* deref, Expression* exp);

    void Generate(State* St);
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
    Variable* var;
    Expression* eq;
    Expression* to;
    Expression* step;
    StatementList* body;

    ForStatement(State* St, Variable* var, Expression* eq, Expression* to, Expression* step, StatementList* body);

    void Generate(State *St);
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
    Expression* condition;
    StatementList* body;

    WhileStatement(State* St, Expression* condition, StatementList* body);

    void Generate(State* St);
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
    Expression* condition;
    StatementList* body;

    DoUntilStatement(State* St, Expression* condition, StatementList* body);

    void Generate(State* St);
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
    Expression* condition;
    StatementList* then_body;
    StatementList* else_body;

    IfStatement(State* St, Expression* condition, StatementList* body, StatementList* else_body);

    void Generate(State* St);
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
    ReturnStatement();

    /*
   * This constructor handles the case of returns with expressions.
   *
   * In this case we check if the expression is well-typed.
   */
    ReturnStatement(State* St, Expression* exp);
};
