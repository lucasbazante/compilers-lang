#pragma once

#include <sstream>
#include <string>

#include "symbol_table.hpp"

class State {
private:
  SymbolTable sym_tab;
  bool error;
  int temp_var_counter;
  int label_counter;
  std::ostringstream code;
  std::ostringstream output;
  std::ostringstream declarations;

public:
  State()
    : sym_tab(SymbolTable()), error(false), temp_var_counter(0)
  { }

  ~State() {
    sym_tab.pop();
  }

  SymbolTable* Table() {
    return &sym_tab;
  }

  void FlagError() {
    error = true;
  }

  void Emit_Code() {
    declarations << "\n" << output.str();
    code << declarations.str() << "\n";

    output.str("");
    declarations.str("");
  }

  std::string Output() {
    return code.str();
  }
  
  void Emit(const std::string& code) {
    if (not error)
      output << code << "\n";
  }

  void Emit_OnLine(const std::string& code) {
    if (not error)
      output << code;
  }

  std::string Next_TempVar(TypeInfo* type) {
    return type->Gen() + " _v" + std::to_string(temp_var_counter++);
  }

  std::string Current_TempVar() {
    return "_v" + std::to_string(temp_var_counter - 1);
  }
  
  /*
   * Emit an expression and assign it to a intermediary variable.
   *
   * Each expression contains at most one operator, so the result
   * of the operator is assign to the variable here.
   */
  void Emit_Expr(const std::string& code, TypeInfo* type) {
    if (not error) {
      declarations << Next_TempVar(type) << ";\n";
      output << Current_TempVar() << " = " << code << ";\n";
    }
  }

  std::string Next_Label() {
    return "L" + std::to_string(label_counter++);
  }

  std::string Current_Label() {
    return "L" + std::to_string(label_counter - 1);
  }

  /*
   * Emit a declaration without expression assigned to it.
   * Example: `int x;`.
  */
  void Emit_Decl(const std::string& decl_name, TypeInfo* type) {
    if (not error)
      declarations << type->Gen() << " "
        << decl_name
        << ";\n";
  }

  /*
   * Emit a declaration with an expression assigned to it.
   * Example: `int x = 5;`.
  */
  void Emit_Decl(const std::string& decl_name, TypeInfo* type, const std::string& expr_repr) {
    if (not error)
      declarations << type->Gen() << " "
        << decl_name
        << " = " << expr_repr
        << ";\n";
  }

  void Emit_StructDecl(const std::string& struct_name, const std::string& params) {
    if (not error)
      declarations << "struct " << struct_name << " {\n" << params << "};\n";
  }

  void Emit_Label(const std::string& label) {
    if (not error)
      output << label << ":\n";
  }

  void Emit_If_Header(const std::string& condition, const std::string& then_label, const std::string& end_label, const std::string& else_label = "") {
    if (not error) {
      output << "if ("
        << condition
        << ") goto "
        << then_label
        << ";\n";

      if (not else_label.empty())
        output << "goto "
          << else_label
          << ";\n";
      else
        output << "goto "
          << end_label
          << ";\n";
    }
  }

  void Emit_While_Header(const std::string& condition, const std::string& end_label) {
    if (not error)
      output << "if (!"
        << condition
        << ") goto "
        << end_label
        << ";\n"; 
  }
  
  void Emit_DoUntil_Header(const std::string& condition, const std::string& loop_label) {
    if (not error)
      output << "if ("
        << condition
        << ") goto "
        << loop_label
        << ";\n";
  }
};
