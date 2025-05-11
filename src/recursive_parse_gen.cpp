#include <fstream>
#include "../include/parse_table.hpp"

using namespace std;


std::string code = "#include <cstdlib>\n"
                   "#include <iostream>\n"
                   "#include <sstream>\n"
                   "#include <variant>\n"
                   "#include <string>\n"
                   "#include \"lexer.hpp\""
                   "\n"
                   "\n"
                   "\n"
                   "\n"
                   "std::string token_to_string(Token token) {\n"
                   "   std::ostringstream oss;\n"
                   "   oss << token;\n"
                   "   return oss.str();\n"
                   "}\n"
                   "\n"
                   "\n"
                   "Token match(std::string str_, Token current_token) {\n"
                   "\n"
                   "   if (token_to_string(current_token) == str_) {\n"
                   "       Token token = static_cast<Token>(yylex());\n"
                   "       return token;\n"
                   "   }\n"
                   "   else {\n"
                   "       // parar execução\n"
                   "       std::cout << \"info\\n\";\n"
                   "       exit(1);\n"
                   "   }\n"
                   "}";


int main() {
    ofstream file("../include/recursive_src.hpp");

    file << code << '\n';

    int count = (int)Rule::COUNT;
    ParseTable parseTable = build_parse_table();
    for(int i=0; i<count; i++) {
        file << "Token procedure_" << (Rule)i << "(Token token);\n";
    }

    for(int i=0; i<count; i++) {
        file << "Token procedure_" << (Rule)i << "(Token token) {\n";
        file << "\tswitch((int)token) {\n";
        for(int j=0; j<parseTable[i].size(); j++) {
            auto cell = parseTable[i][j];
            
            if(cell.has_value()) {
                file << "\t\tcase " << j << ":\n";
                for(auto el : cell.value()) {
                    if(el.index() == 0) {
                        if(i == 0) {
                            file << "\t\t\tif (token != Token::TEOF) {\n"
                                    "\t\t\t\tstd::cerr << \"[SYNTAX ERROR]\\nInput not fully consumed!\\n\";\n"
                                    "\t\t\t}\n";
                        } else {
                            file << "\t\t\ttoken = match(\"" << (Token) std::get<Token>(el) << "\", token);\n";
                        }
                    } else {
                        file << "\t\t\ttoken = procedure_" << (Rule) std::get<Rule>(el) << "(token);\n";
                    }
                }
                file << "\t\t\tbreak;\n";
            }
        }

        file << "\t\tdefault:\n";
        file << "\t\t\tstd::cout << \"[SYNTAX ERROR] No rule found for " << (Rule)i << " rule with token \" << token;\n";
        file << "\t\t\texit(1);\n";
        file << "\t}\n";
        file << "\treturn token;\n}\n";
    }
}