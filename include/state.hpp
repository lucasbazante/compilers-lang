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

  void Emit_Expr(const std::string& code, TypeInfo* type) {
    if (not error)
      output << Next_TempVar(type) << " = " << code << ";\n";
  }

  void Emit_Decl(const std::string& decl_name, const std::string& expr = "", TypeInfo* type = nullptr) {
    if (not error) {
      if (type != nullptr)
        output << type->Gen() << " ";

      output << decl_name;

      if (not expr.empty())
        output << " = " << expr;

      output << ";\n";
    }
  }

  void Emit_StructDecl(const std::string& struct_name, const std::string& params) {
    if (not error)
      output << "struct " << struct_name << " {\n" << params << "};\n";
  }

  std::string Output() {
    return output.str();
  }
};
