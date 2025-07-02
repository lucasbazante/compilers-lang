#pragma once

#include <sstream>

#include "symbol_table.hpp"

class State {
private:
  SymbolTable sym_tab;
  bool error;
  std::ostringstream output;

public:
  State()
    : sym_tab(SymbolTable()), error(false)
  { }

  ~State() {
    sym_tab.pop();
  }

  SymbolTable* Table() {
    return &sym_tab;
  }

  void FlagError() {
    error = false;
  }
};
