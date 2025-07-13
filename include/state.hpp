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
  std::ostringstream header;
  std::ostringstream declarations;
  std::ostringstream program;

  void Generate_Std_Impl() {
    this->Generate_readint();
    this->Generate_readfloat();
    this->Generate_readchar();
    this->Generate_readstring();
    this->Generate_readline();

    this->Generate_printint();
    this->Generate_printfloat();
    this->Generate_printstr();
    this->Generate_printline();
  }

  void Generate_readint() {
    header << "int readint() {\n"
      << "    int x;\n"
      << "    cin >> x;\n"
      << "    return x;\n"
      << "}\n";
  }

  void Generate_readfloat() {
    header << "float readfloat() {\n"
      << "    float x;\n"
      << "    cin >> x;\n"
      << "    return x;\n"
      << "}\n\n";
  }

  void Generate_readchar() {
    header << "int readchar() {\n"
      << "    int c = cin.get();\n"
      << "    return c;\n"
      << "}\n\n";
  }

  void Generate_readstring() {
    header << "string readstring() {\n"
      << "    string s;\n"
      << "    cin >> s;\n"
      << "    return s;\n"
      << "}\n\n";
  }

  void Generate_readline() {
    header << "string readline() {\n"
      << "    string line;\n"
      << "    getline(cin, line);\n"
      << "    return line;\n"
      << "}\n\n";
  }

  void Generate_printint() {
    header << "void printint(int i) {\n"
      << "    cout << i;\n"
      << "}\n\n";
  }

  void Generate_printfloat() {
    header << "void printfloat(float f) {\n"
      << "    cout << f;\n"
      << "}\n\n";
  }

  void Generate_printstr() {
    header << "void printstr(const std::string& s) {\n"
      << "    cout << s;\n"
      << "}\n\n";
  }

  void Generate_printline() {
    header << "void printline(const std::string& s) {\n"
      << "    cout << s << endl;\n"
      << "}\n\n";
  }

  void Generate_Imports() {
    header << "#include <iostream>\n"
      << "#include <string>\n"
      << "using namespace std;\n\n";
  }

  void Generate_Main() {
    header << "int main() {\n\n";
  }

public:
  State()
  : sym_tab(SymbolTable()), error(false), temp_var_counter(0)
  {
    this->Generate_Imports();
    this->Generate_Std_Impl();
    this->Generate_Main();
  }

  ~State() {
    sym_tab.pop();
  }

  SymbolTable* Table() {
    return &sym_tab;
  }

  void FlagError() {
    error = true;
  }

  std::string Scoped_Name(const std::string& name) {
    auto scope = this->sym_tab.current()->name;
    return "_" + scope + "_" + name;
  }

  std::string Scoped_Type(TypeInfo* type) {
    if (type->b_type == BaseType::STRUCT) {
      auto scope = this->sym_tab.current()->name;
      return "_" + scope + "_" + type->Gen();
    }
    return type->Gen();
  }
  
  std::string Output() {
    header << declarations.str() << "\n";
    header << program.str();
    header << "\n"
      << "return 0;\n}\0";
    return header.str();
  }

  void Emit(const std::string& code) {
    if (not error)
      program << code << "\n";
  }

  void Emit_OnLine(const std::string& code) {
    if (not error)
      program << code;
  }

  void Emit_Var(const std::string& name) {
    if (not error)
      program << this->Scoped_Name(name);
  }

  void Emit_Access(const std::string& name, const std::string& struct_exp) {
    if (not error)
      program << struct_exp
        << "."
        << name;
  }

  std::string Next_TempVar(TypeInfo* type) {
    return this->Scoped_Type(type) + " _v" + std::to_string(temp_var_counter++);
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
      program << Current_TempVar() << " = " << code << ";\n";
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
    if (not error) {
      declarations << this->Scoped_Type(type)
        << " "
        << this->Scoped_Name(decl_name)
        << ";\n";
    }
  }

  /*
   * Emit a declaration with an expression assigned to it.
   * Example: `int x = 5;`.
  */
  void Emit_Decl(const std::string& decl_name, TypeInfo* type, const std::string& expr_repr) {
    if (not error) {
      this->Emit_Decl(decl_name, type);

      program << this->Scoped_Name(decl_name)
        << " = "
        << expr_repr
        << ";\n";
    }
  }

  void Emit_StructDecl(const std::string& struct_name, const std::string& params) {
    if (not error)
      declarations << "struct "
        << this->Scoped_Name(struct_name)
        << " {\n"
        << params
        << "};\n";
  }

  void Emit_Label(const std::string& label) {
    if (not error)
      program << label << ":\n";
  }

  void Emit_If_Header(const std::string& condition, const std::string& then_label, const std::string& end_label, const std::string& else_label = "") {
    if (not error) {
      program << "if ("
        << condition
        << ") goto "
        << then_label
        << ";\n";

      if (not else_label.empty())
        program << "goto "
          << else_label
          << ";\n";
      else
        program << "goto "
          << end_label
          << ";\n";
    }
  }

  void Emit_While_Header(const std::string& condition, const std::string& end_label) {
    if (not error)
      program << "if (!"
        << condition
        << ") goto "
        << end_label
        << ";\n"; 
  }

  void Emit_DoUntil_Header(const std::string& condition, const std::string& loop_label) {
    if (not error)
      program << "if ("
        << condition
        << ") goto "
        << loop_label
        << ";\n";
  }
};
