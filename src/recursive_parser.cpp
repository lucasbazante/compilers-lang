#include "../include/recursive_src.hpp"

int main() {
    Token token = static_cast<Token>(yylex());
    procedure_S(token);
    std::cout << "Compiled successfully\n";
}