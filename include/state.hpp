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
  std::ostringstream output;

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

  std::string Next_TempVar(TypeInfo* type) {
    return type->Gen() + " _v" + std::to_string(temp_var_counter++);
  }

  std::string Current_TempVar() {
    return "_v" + std::to_string(temp_var_counter - 1);
  }

  std::string Next_Label() {
    return "L" + std::to_string(label_counter++);
  }

  std::string Current_Label() {
    return "L" + std::to_string(label_counter - 1);
  }

  void Emit(const std::string& code) {
    if (not error)
      output << code << "\n";
  }

  void Emit_OnLine(const std::string& code) {
    if (not error)
      output << code;
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

  /*
   * Emit an expression and assign it to a intermediary variable.
   *
   * Each expression contains at most one operator, so the result
   * of the operator is assign to the variable here.
   */
  void Emit_Expr(const std::string& code, TypeInfo* type) {
    if (not error)
      output << Next_TempVar(type) << " = " << code << ";\n";
  }

  /*
   * Emit a declaration without expression assigned to it.
   * Example: `int x;`.
  */
  void Emit_Decl(const std::string& decl_name, TypeInfo* type) {
    if (not error)
      output << type->Gen() << " "
        << decl_name
        << ";\n";
  }

  /*
   * Emit a declaration with an expression assigned to it.
   * Example: `int x = 5;`.
  */
  void Emit_Decl(const std::string& decl_name, TypeInfo* type, const std::string& expr_repr) {
    if (not error)
      output << type->Gen() << " "
        << decl_name
        << " = " << expr_repr
        << ";\n";
  }

  void Emit_StructDecl(const std::string& struct_name, const std::string& params) {
    if (not error)
      output << "struct " << struct_name << " {\n" << params << "};\n";
  }

  std::string Output() {
    return output.str();
  }
};
