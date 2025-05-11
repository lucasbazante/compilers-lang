#include <cstdlib>
#include <iostream>
#include <sstream>
#include <variant>
#include <string>
#include "lexer.hpp"



std::string token_to_string(Token token) {
   std::ostringstream oss;
   oss << token;
   return oss.str();
}


Token match(std::string str_, Token current_token) {

   if (token_to_string(current_token) == str_) {
       Token token = static_cast<Token>(yylex());
       return token;
   }
   else {
       // parar execução
       std::cout << "expected " << str_ << " but found " << current_token << std::endl;
       exit(1);
   }
}
Token procedure_S(Token token);
Token procedure_PROGRAM(Token token);
Token procedure_DECLS_OPT(Token token);
Token procedure_DECLS_LIST_OPT(Token token);
Token procedure_PROC_DECLS_OPT(Token token);
Token procedure_PROC_DECLS_LIST_OPT(Token token);
Token procedure_DECL(Token token);
Token procedure_VAR_DECL(Token token);
Token procedure_VAR_DECL_TYPE_OPT(Token token);
Token procedure_VAR_DECL_EXP_OPT(Token token);
Token procedure_PROC_DECL(Token token);
Token procedure_PARAMFIELD_TYPE_OPT(Token token);
Token procedure_PARAMFIELD_DECL_OPT(Token token);
Token procedure_PARAMFIELD_DECL_LIST(Token token);
Token procedure_PARAMFIELD_DECL_LIST_OPT(Token token);
Token procedure_REC_DECL(Token token);
Token procedure_PARAMFIELD_DECL_OPT_PRIME(Token token);
Token procedure_PARAMFIELD_DECL_LIST_PRIME(Token token);
Token procedure_PARAMFIELD_DECL_LIST_OPT_PRIME(Token token);
Token procedure_STMT_LIST(Token token);
Token procedure_STMT_LIST_TAIL(Token token);
Token procedure_STMT(Token token);
Token procedure_IF_STMT(Token token);
Token procedure_IF_ELSE(Token token);
Token procedure_WHILE_STMT(Token token);
Token procedure_FOR_STMT(Token token);
Token procedure_DO_UNTIL_STMT(Token token);
Token procedure_RETURN_STMT(Token token);
Token procedure_CALL_OR_ASSIGNMENT(Token token);
Token procedure_ACCESS_TAIL(Token token);
Token procedure_MAYBE_ACCESS_TAIL(Token token);
Token procedure_EXP(Token token);
Token procedure_OR_EXP(Token token);
Token procedure_OR_EXP_TAIL(Token token);
Token procedure_AND_EXP(Token token);
Token procedure_AND_EXP_TAIL(Token token);
Token procedure_REL_EXP(Token token);
Token procedure_REL_EXP_TAIL(Token token);
Token procedure_ADD_EXP(Token token);
Token procedure_ADD_EXP_TAIL(Token token);
Token procedure_MUL_EXP(Token token);
Token procedure_MUL_EXP_TAIL(Token token);
Token procedure_POW_EXP(Token token);
Token procedure_POW_EXP_TAIL(Token token);
Token procedure_UNARY_EXP(Token token);
Token procedure_PRIMARY(Token token);
Token procedure_REF_VAR(Token token);
Token procedure_DEREF_VAR(Token token);
Token procedure_MAYBE_CALL(Token token);
Token procedure_EXP_LIST(Token token);
Token procedure_EXP_LIST_TAIL(Token token);
Token procedure_SUMINUS(Token token);
Token procedure_MULDIV(Token token);
Token procedure_REL_OP(Token token);
Token procedure_LITERAL(Token token);
Token procedure_TYPE(Token token);
Token procedure_S(Token token) {
	switch((int)token) {
		case 4:
			token = procedure_PROGRAM(token);
			if (token != Token::TEOF) {
				std::cerr << "[SYNTAX ERROR]\nInput not fully consumed!\n";
			}
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for S rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_PROGRAM(Token token) {
	switch((int)token) {
		case 4:
			token = match("program", token);
			token = match("name", token);
			token = match("begin", token);
			token = procedure_DECLS_OPT(token);
			token = match("end", token);
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for PROGRAM rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_DECLS_OPT(Token token) {
	switch((int)token) {
		case 7:
			break;
		case 8:
			token = procedure_DECL(token);
			token = procedure_DECLS_LIST_OPT(token);
			break;
		case 9:
			token = procedure_DECL(token);
			token = procedure_DECLS_LIST_OPT(token);
			break;
		case 10:
			token = procedure_DECL(token);
			token = procedure_DECLS_LIST_OPT(token);
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for DECLS_OPT rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_DECLS_LIST_OPT(Token token) {
	switch((int)token) {
		case 7:
			break;
		case 40:
			token = match(";", token);
			token = procedure_DECL(token);
			token = procedure_DECLS_LIST_OPT(token);
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for DECLS_LIST_OPT rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_PROC_DECLS_OPT(Token token) {
	switch((int)token) {
		case 6:
			token = match("in", token);
			break;
		case 7:
			break;
		case 8:
			token = procedure_DECL(token);
			token = procedure_PROC_DECLS_LIST_OPT(token);
			token = match("in", token);
			break;
		case 9:
			token = procedure_DECL(token);
			token = procedure_PROC_DECLS_LIST_OPT(token);
			token = match("in", token);
			break;
		case 10:
			token = procedure_DECL(token);
			token = procedure_PROC_DECLS_LIST_OPT(token);
			token = match("in", token);
			break;
		case 11:
			break;
		case 12:
			break;
		case 13:
			break;
		case 23:
			break;
		case 24:
			break;
		case 25:
			break;
		case 26:
			break;
		case 27:
			break;
		case 28:
			break;
		case 32:
			break;
		case 33:
			break;
		case 36:
			break;
		case 39:
			break;
		case 43:
			break;
		case 49:
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for PROC_DECLS_OPT rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_PROC_DECLS_LIST_OPT(Token token) {
	switch((int)token) {
		case 6:
			break;
		case 40:
			token = match(";", token);
			token = procedure_DECL(token);
			token = procedure_PROC_DECLS_LIST_OPT(token);
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for PROC_DECLS_LIST_OPT rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_DECL(Token token) {
	switch((int)token) {
		case 8:
			token = procedure_VAR_DECL(token);
			break;
		case 9:
			token = procedure_PROC_DECL(token);
			break;
		case 10:
			token = procedure_REC_DECL(token);
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for DECL rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_VAR_DECL(Token token) {
	switch((int)token) {
		case 8:
			token = match("var", token);
			token = match("name", token);
			token = procedure_VAR_DECL_TYPE_OPT(token);
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for VAR_DECL rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_VAR_DECL_TYPE_OPT(Token token) {
	switch((int)token) {
		case 41:
			token = match(":", token);
			token = procedure_TYPE(token);
			token = procedure_VAR_DECL_EXP_OPT(token);
			break;
		case 42:
			token = match(":=", token);
			token = procedure_EXP(token);
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for VAR_DECL_TYPE_OPT rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_VAR_DECL_EXP_OPT(Token token) {
	switch((int)token) {
		case 6:
			break;
		case 7:
			break;
		case 40:
			break;
		case 42:
			token = match(":=", token);
			token = procedure_EXP(token);
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for VAR_DECL_EXP_OPT rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_PROC_DECL(Token token) {
	switch((int)token) {
		case 9:
			token = match("procedure", token);
			token = match("name", token);
			token = match("(", token);
			token = procedure_PARAMFIELD_DECL_OPT(token);
			token = match(")", token);
			token = procedure_PARAMFIELD_TYPE_OPT(token);
			token = match("begin", token);
			token = procedure_PROC_DECLS_OPT(token);
			token = procedure_STMT_LIST(token);
			token = match("end", token);
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for PROC_DECL rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_PARAMFIELD_TYPE_OPT(Token token) {
	switch((int)token) {
		case 5:
			break;
		case 41:
			token = match(":", token);
			token = procedure_TYPE(token);
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for PARAMFIELD_TYPE_OPT rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_PARAMFIELD_DECL_OPT(Token token) {
	switch((int)token) {
		case 44:
			break;
		case 49:
			token = procedure_PARAMFIELD_DECL_LIST(token);
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for PARAMFIELD_DECL_OPT rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_PARAMFIELD_DECL_LIST(Token token) {
	switch((int)token) {
		case 49:
			token = match("name", token);
			token = match(":", token);
			token = procedure_TYPE(token);
			token = procedure_PARAMFIELD_DECL_LIST_OPT(token);
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for PARAMFIELD_DECL_LIST rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_PARAMFIELD_DECL_LIST_OPT(Token token) {
	switch((int)token) {
		case 44:
			break;
		case 48:
			token = match(",", token);
			token = procedure_PARAMFIELD_DECL_LIST(token);
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for PARAMFIELD_DECL_LIST_OPT rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_REC_DECL(Token token) {
	switch((int)token) {
		case 10:
			token = match("struct", token);
			token = match("name", token);
			token = match("{", token);
			token = procedure_PARAMFIELD_DECL_OPT_PRIME(token);
			token = match("}", token);
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for REC_DECL rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_PARAMFIELD_DECL_OPT_PRIME(Token token) {
	switch((int)token) {
		case 46:
			break;
		case 49:
			token = procedure_PARAMFIELD_DECL_LIST_PRIME(token);
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for PARAMFIELD_DECL_OPT_PRIME rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_PARAMFIELD_DECL_LIST_PRIME(Token token) {
	switch((int)token) {
		case 49:
			token = match("name", token);
			token = match(":", token);
			token = procedure_TYPE(token);
			token = procedure_PARAMFIELD_DECL_LIST_OPT_PRIME(token);
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for PARAMFIELD_DECL_LIST_PRIME rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_PARAMFIELD_DECL_LIST_OPT_PRIME(Token token) {
	switch((int)token) {
		case 40:
			token = match(";", token);
			token = procedure_PARAMFIELD_DECL_LIST_PRIME(token);
			break;
		case 46:
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for PARAMFIELD_DECL_LIST_OPT_PRIME rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_STMT_LIST(Token token) {
	switch((int)token) {
		case 7:
			break;
		case 11:
			token = procedure_STMT(token);
			token = procedure_STMT_LIST_TAIL(token);
			break;
		case 12:
			token = procedure_STMT(token);
			token = procedure_STMT_LIST_TAIL(token);
			break;
		case 13:
			token = procedure_STMT(token);
			token = procedure_STMT_LIST_TAIL(token);
			break;
		case 23:
			token = procedure_STMT(token);
			token = procedure_STMT_LIST_TAIL(token);
			break;
		case 24:
			token = procedure_STMT(token);
			token = procedure_STMT_LIST_TAIL(token);
			break;
		case 25:
			token = procedure_STMT(token);
			token = procedure_STMT_LIST_TAIL(token);
			break;
		case 26:
			token = procedure_STMT(token);
			token = procedure_STMT_LIST_TAIL(token);
			break;
		case 27:
			token = procedure_STMT(token);
			token = procedure_STMT_LIST_TAIL(token);
			break;
		case 28:
			token = procedure_STMT(token);
			token = procedure_STMT_LIST_TAIL(token);
			break;
		case 30:
			break;
		case 31:
			break;
		case 32:
			token = procedure_STMT(token);
			token = procedure_STMT_LIST_TAIL(token);
			break;
		case 33:
			token = procedure_STMT(token);
			token = procedure_STMT_LIST_TAIL(token);
			break;
		case 34:
			break;
		case 35:
			break;
		case 36:
			token = procedure_STMT(token);
			token = procedure_STMT_LIST_TAIL(token);
			break;
		case 39:
			token = procedure_STMT(token);
			token = procedure_STMT_LIST_TAIL(token);
			break;
		case 43:
			token = procedure_STMT(token);
			token = procedure_STMT_LIST_TAIL(token);
			break;
		case 49:
			token = procedure_STMT(token);
			token = procedure_STMT_LIST_TAIL(token);
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for STMT_LIST rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_STMT_LIST_TAIL(Token token) {
	switch((int)token) {
		case 7:
			break;
		case 30:
			break;
		case 31:
			break;
		case 34:
			break;
		case 35:
			break;
		case 40:
			token = match(";", token);
			token = procedure_STMT_LIST(token);
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for STMT_LIST_TAIL rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_STMT(Token token) {
	switch((int)token) {
		case 11:
			token = procedure_CALL_OR_ASSIGNMENT(token);
			break;
		case 12:
			token = procedure_CALL_OR_ASSIGNMENT(token);
			break;
		case 13:
			token = procedure_CALL_OR_ASSIGNMENT(token);
			break;
		case 23:
			token = procedure_CALL_OR_ASSIGNMENT(token);
			break;
		case 24:
			token = procedure_CALL_OR_ASSIGNMENT(token);
			break;
		case 25:
			token = procedure_CALL_OR_ASSIGNMENT(token);
			break;
		case 26:
			token = procedure_CALL_OR_ASSIGNMENT(token);
			break;
		case 27:
			token = procedure_CALL_OR_ASSIGNMENT(token);
			break;
		case 28:
			token = procedure_IF_STMT(token);
			break;
		case 32:
			token = procedure_WHILE_STMT(token);
			break;
		case 33:
			token = procedure_DO_UNTIL_STMT(token);
			break;
		case 36:
			token = procedure_FOR_STMT(token);
			break;
		case 39:
			token = procedure_RETURN_STMT(token);
			break;
		case 43:
			token = procedure_CALL_OR_ASSIGNMENT(token);
			break;
		case 49:
			token = procedure_CALL_OR_ASSIGNMENT(token);
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for STMT rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_IF_STMT(Token token) {
	switch((int)token) {
		case 28:
			token = match("if", token);
			token = procedure_EXP(token);
			token = match("then", token);
			token = procedure_STMT_LIST(token);
			token = procedure_IF_ELSE(token);
			token = match("fi", token);
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for IF_STMT rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_IF_ELSE(Token token) {
	switch((int)token) {
		case 30:
			token = match("else", token);
			token = procedure_STMT_LIST(token);
			break;
		case 31:
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for IF_ELSE rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_WHILE_STMT(Token token) {
	switch((int)token) {
		case 32:
			token = match("while", token);
			token = procedure_EXP(token);
			token = match("do", token);
			token = procedure_STMT_LIST(token);
			token = match("od", token);
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for WHILE_STMT rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_FOR_STMT(Token token) {
	switch((int)token) {
		case 36:
			token = match("for", token);
			token = procedure_PRIMARY(token);
			token = match("comparison", token);
			token = procedure_EXP(token);
			token = match("to", token);
			token = procedure_EXP(token);
			token = match("step", token);
			token = procedure_EXP(token);
			token = match("do", token);
			token = procedure_STMT_LIST(token);
			token = match("od", token);
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for FOR_STMT rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_DO_UNTIL_STMT(Token token) {
	switch((int)token) {
		case 33:
			token = match("do", token);
			token = procedure_STMT_LIST(token);
			token = match("until", token);
			token = procedure_EXP(token);
			token = match("od", token);
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for DO_UNTIL_STMT rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_RETURN_STMT(Token token) {
	switch((int)token) {
		case 39:
			token = match("return", token);
			token = match("(", token);
			token = procedure_EXP(token);
			token = match(")", token);
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for RETURN_STMT rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_CALL_OR_ASSIGNMENT(Token token) {
	switch((int)token) {
		case 11:
			token = match("new", token);
			token = match("name", token);
			token = procedure_ACCESS_TAIL(token);
			token = match(":=", token);
			token = procedure_EXP(token);
			break;
		case 12:
			token = procedure_REF_VAR(token);
			token = procedure_ACCESS_TAIL(token);
			token = match(":=", token);
			token = procedure_EXP(token);
			break;
		case 13:
			token = procedure_DEREF_VAR(token);
			token = procedure_ACCESS_TAIL(token);
			token = match(":=", token);
			token = procedure_EXP(token);
			break;
		case 23:
			token = procedure_LITERAL(token);
			token = procedure_ACCESS_TAIL(token);
			token = match(":=", token);
			token = procedure_EXP(token);
			break;
		case 24:
			token = procedure_LITERAL(token);
			token = procedure_ACCESS_TAIL(token);
			token = match(":=", token);
			token = procedure_EXP(token);
			break;
		case 25:
			token = procedure_LITERAL(token);
			token = procedure_ACCESS_TAIL(token);
			token = match(":=", token);
			token = procedure_EXP(token);
			break;
		case 26:
			token = procedure_LITERAL(token);
			token = procedure_ACCESS_TAIL(token);
			token = match(":=", token);
			token = procedure_EXP(token);
			break;
		case 27:
			token = procedure_LITERAL(token);
			token = procedure_ACCESS_TAIL(token);
			token = match(":=", token);
			token = procedure_EXP(token);
			break;
		case 43:
			token = match("(", token);
			token = procedure_EXP(token);
			token = match(")", token);
			token = procedure_ACCESS_TAIL(token);
			token = match(":=", token);
			token = procedure_EXP(token);
			break;
		case 49:
			token = match("name", token);
			token = procedure_MAYBE_CALL(token);
			token = procedure_MAYBE_ACCESS_TAIL(token);
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for CALL_OR_ASSIGNMENT rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_ACCESS_TAIL(Token token) {
	switch((int)token) {
		case 6:
			break;
		case 7:
			break;
		case 14:
			break;
		case 15:
			break;
		case 16:
			break;
		case 17:
			break;
		case 18:
			break;
		case 20:
			break;
		case 21:
			break;
		case 22:
			break;
		case 29:
			break;
		case 30:
			break;
		case 31:
			break;
		case 33:
			break;
		case 34:
			break;
		case 35:
			break;
		case 37:
			break;
		case 38:
			break;
		case 40:
			break;
		case 42:
			break;
		case 44:
			break;
		case 47:
			token = match(".", token);
			token = match("name", token);
			token = procedure_ACCESS_TAIL(token);
			break;
		case 48:
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for ACCESS_TAIL rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_MAYBE_ACCESS_TAIL(Token token) {
	switch((int)token) {
		case 7:
			break;
		case 30:
			break;
		case 31:
			break;
		case 34:
			break;
		case 35:
			break;
		case 40:
			break;
		case 42:
			token = procedure_ACCESS_TAIL(token);
			token = match(":=", token);
			token = procedure_EXP(token);
			break;
		case 47:
			token = procedure_ACCESS_TAIL(token);
			token = match(":=", token);
			token = procedure_EXP(token);
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for MAYBE_ACCESS_TAIL rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_EXP(Token token) {
	switch((int)token) {
		case 11:
			token = procedure_OR_EXP(token);
			break;
		case 12:
			token = procedure_OR_EXP(token);
			break;
		case 13:
			token = procedure_OR_EXP(token);
			break;
		case 22:
			token = procedure_OR_EXP(token);
			break;
		case 23:
			token = procedure_OR_EXP(token);
			break;
		case 24:
			token = procedure_OR_EXP(token);
			break;
		case 25:
			token = procedure_OR_EXP(token);
			break;
		case 26:
			token = procedure_OR_EXP(token);
			break;
		case 27:
			token = procedure_OR_EXP(token);
			break;
		case 43:
			token = procedure_OR_EXP(token);
			break;
		case 49:
			token = procedure_OR_EXP(token);
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for EXP rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_OR_EXP(Token token) {
	switch((int)token) {
		case 11:
			token = procedure_AND_EXP(token);
			token = procedure_OR_EXP_TAIL(token);
			break;
		case 12:
			token = procedure_AND_EXP(token);
			token = procedure_OR_EXP_TAIL(token);
			break;
		case 13:
			token = procedure_AND_EXP(token);
			token = procedure_OR_EXP_TAIL(token);
			break;
		case 22:
			token = procedure_AND_EXP(token);
			token = procedure_OR_EXP_TAIL(token);
			break;
		case 23:
			token = procedure_AND_EXP(token);
			token = procedure_OR_EXP_TAIL(token);
			break;
		case 24:
			token = procedure_AND_EXP(token);
			token = procedure_OR_EXP_TAIL(token);
			break;
		case 25:
			token = procedure_AND_EXP(token);
			token = procedure_OR_EXP_TAIL(token);
			break;
		case 26:
			token = procedure_AND_EXP(token);
			token = procedure_OR_EXP_TAIL(token);
			break;
		case 27:
			token = procedure_AND_EXP(token);
			token = procedure_OR_EXP_TAIL(token);
			break;
		case 43:
			token = procedure_AND_EXP(token);
			token = procedure_OR_EXP_TAIL(token);
			break;
		case 49:
			token = procedure_AND_EXP(token);
			token = procedure_OR_EXP_TAIL(token);
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for OR_EXP rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_OR_EXP_TAIL(Token token) {
	switch((int)token) {
		case 6:
			break;
		case 7:
			break;
		case 21:
			token = match("||", token);
			token = procedure_AND_EXP(token);
			token = procedure_OR_EXP_TAIL(token);
			break;
		case 29:
			break;
		case 30:
			break;
		case 31:
			break;
		case 33:
			break;
		case 34:
			break;
		case 35:
			break;
		case 37:
			break;
		case 38:
			break;
		case 40:
			break;
		case 44:
			break;
		case 48:
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for OR_EXP_TAIL rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_AND_EXP(Token token) {
	switch((int)token) {
		case 11:
			token = procedure_REL_EXP(token);
			token = procedure_AND_EXP_TAIL(token);
			break;
		case 12:
			token = procedure_REL_EXP(token);
			token = procedure_AND_EXP_TAIL(token);
			break;
		case 13:
			token = procedure_REL_EXP(token);
			token = procedure_AND_EXP_TAIL(token);
			break;
		case 22:
			token = procedure_REL_EXP(token);
			token = procedure_AND_EXP_TAIL(token);
			break;
		case 23:
			token = procedure_REL_EXP(token);
			token = procedure_AND_EXP_TAIL(token);
			break;
		case 24:
			token = procedure_REL_EXP(token);
			token = procedure_AND_EXP_TAIL(token);
			break;
		case 25:
			token = procedure_REL_EXP(token);
			token = procedure_AND_EXP_TAIL(token);
			break;
		case 26:
			token = procedure_REL_EXP(token);
			token = procedure_AND_EXP_TAIL(token);
			break;
		case 27:
			token = procedure_REL_EXP(token);
			token = procedure_AND_EXP_TAIL(token);
			break;
		case 43:
			token = procedure_REL_EXP(token);
			token = procedure_AND_EXP_TAIL(token);
			break;
		case 49:
			token = procedure_REL_EXP(token);
			token = procedure_AND_EXP_TAIL(token);
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for AND_EXP rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_AND_EXP_TAIL(Token token) {
	switch((int)token) {
		case 6:
			break;
		case 7:
			break;
		case 20:
			token = match("&&", token);
			token = procedure_REL_EXP(token);
			token = procedure_AND_EXP_TAIL(token);
			break;
		case 21:
			break;
		case 29:
			break;
		case 30:
			break;
		case 31:
			break;
		case 33:
			break;
		case 34:
			break;
		case 35:
			break;
		case 37:
			break;
		case 38:
			break;
		case 40:
			break;
		case 44:
			break;
		case 48:
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for AND_EXP_TAIL rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_REL_EXP(Token token) {
	switch((int)token) {
		case 11:
			token = procedure_ADD_EXP(token);
			token = procedure_REL_EXP_TAIL(token);
			break;
		case 12:
			token = procedure_ADD_EXP(token);
			token = procedure_REL_EXP_TAIL(token);
			break;
		case 13:
			token = procedure_ADD_EXP(token);
			token = procedure_REL_EXP_TAIL(token);
			break;
		case 22:
			token = procedure_ADD_EXP(token);
			token = procedure_REL_EXP_TAIL(token);
			break;
		case 23:
			token = procedure_ADD_EXP(token);
			token = procedure_REL_EXP_TAIL(token);
			break;
		case 24:
			token = procedure_ADD_EXP(token);
			token = procedure_REL_EXP_TAIL(token);
			break;
		case 25:
			token = procedure_ADD_EXP(token);
			token = procedure_REL_EXP_TAIL(token);
			break;
		case 26:
			token = procedure_ADD_EXP(token);
			token = procedure_REL_EXP_TAIL(token);
			break;
		case 27:
			token = procedure_ADD_EXP(token);
			token = procedure_REL_EXP_TAIL(token);
			break;
		case 43:
			token = procedure_ADD_EXP(token);
			token = procedure_REL_EXP_TAIL(token);
			break;
		case 49:
			token = procedure_ADD_EXP(token);
			token = procedure_REL_EXP_TAIL(token);
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for REL_EXP rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_REL_EXP_TAIL(Token token) {
	switch((int)token) {
		case 6:
			break;
		case 7:
			break;
		case 20:
			break;
		case 21:
			break;
		case 22:
			token = procedure_REL_OP(token);
			token = procedure_ADD_EXP(token);
			token = procedure_REL_EXP_TAIL(token);
			break;
		case 29:
			break;
		case 30:
			break;
		case 31:
			break;
		case 33:
			break;
		case 34:
			break;
		case 35:
			break;
		case 37:
			break;
		case 38:
			break;
		case 40:
			break;
		case 44:
			break;
		case 48:
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for REL_EXP_TAIL rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_ADD_EXP(Token token) {
	switch((int)token) {
		case 11:
			token = procedure_MUL_EXP(token);
			token = procedure_ADD_EXP_TAIL(token);
			break;
		case 12:
			token = procedure_MUL_EXP(token);
			token = procedure_ADD_EXP_TAIL(token);
			break;
		case 13:
			token = procedure_MUL_EXP(token);
			token = procedure_ADD_EXP_TAIL(token);
			break;
		case 22:
			token = procedure_MUL_EXP(token);
			token = procedure_ADD_EXP_TAIL(token);
			break;
		case 23:
			token = procedure_MUL_EXP(token);
			token = procedure_ADD_EXP_TAIL(token);
			break;
		case 24:
			token = procedure_MUL_EXP(token);
			token = procedure_ADD_EXP_TAIL(token);
			break;
		case 25:
			token = procedure_MUL_EXP(token);
			token = procedure_ADD_EXP_TAIL(token);
			break;
		case 26:
			token = procedure_MUL_EXP(token);
			token = procedure_ADD_EXP_TAIL(token);
			break;
		case 27:
			token = procedure_MUL_EXP(token);
			token = procedure_ADD_EXP_TAIL(token);
			break;
		case 43:
			token = procedure_MUL_EXP(token);
			token = procedure_ADD_EXP_TAIL(token);
			break;
		case 49:
			token = procedure_MUL_EXP(token);
			token = procedure_ADD_EXP_TAIL(token);
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for ADD_EXP rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_ADD_EXP_TAIL(Token token) {
	switch((int)token) {
		case 6:
			break;
		case 7:
			break;
		case 14:
			token = procedure_SUMINUS(token);
			token = procedure_MUL_EXP(token);
			token = procedure_ADD_EXP_TAIL(token);
			break;
		case 15:
			token = procedure_SUMINUS(token);
			token = procedure_MUL_EXP(token);
			token = procedure_ADD_EXP_TAIL(token);
			break;
		case 20:
			break;
		case 21:
			break;
		case 22:
			break;
		case 29:
			break;
		case 30:
			break;
		case 31:
			break;
		case 33:
			break;
		case 34:
			break;
		case 35:
			break;
		case 37:
			break;
		case 38:
			break;
		case 40:
			break;
		case 44:
			break;
		case 48:
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for ADD_EXP_TAIL rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_MUL_EXP(Token token) {
	switch((int)token) {
		case 11:
			token = procedure_POW_EXP(token);
			token = procedure_MUL_EXP_TAIL(token);
			break;
		case 12:
			token = procedure_POW_EXP(token);
			token = procedure_MUL_EXP_TAIL(token);
			break;
		case 13:
			token = procedure_POW_EXP(token);
			token = procedure_MUL_EXP_TAIL(token);
			break;
		case 22:
			token = procedure_POW_EXP(token);
			token = procedure_MUL_EXP_TAIL(token);
			break;
		case 23:
			token = procedure_POW_EXP(token);
			token = procedure_MUL_EXP_TAIL(token);
			break;
		case 24:
			token = procedure_POW_EXP(token);
			token = procedure_MUL_EXP_TAIL(token);
			break;
		case 25:
			token = procedure_POW_EXP(token);
			token = procedure_MUL_EXP_TAIL(token);
			break;
		case 26:
			token = procedure_POW_EXP(token);
			token = procedure_MUL_EXP_TAIL(token);
			break;
		case 27:
			token = procedure_POW_EXP(token);
			token = procedure_MUL_EXP_TAIL(token);
			break;
		case 43:
			token = procedure_POW_EXP(token);
			token = procedure_MUL_EXP_TAIL(token);
			break;
		case 49:
			token = procedure_POW_EXP(token);
			token = procedure_MUL_EXP_TAIL(token);
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for MUL_EXP rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_MUL_EXP_TAIL(Token token) {
	switch((int)token) {
		case 6:
			break;
		case 7:
			break;
		case 14:
			break;
		case 15:
			break;
		case 16:
			token = procedure_MULDIV(token);
			token = procedure_POW_EXP(token);
			token = procedure_MUL_EXP_TAIL(token);
			break;
		case 17:
			token = procedure_MULDIV(token);
			token = procedure_POW_EXP(token);
			token = procedure_MUL_EXP_TAIL(token);
			break;
		case 20:
			break;
		case 21:
			break;
		case 22:
			break;
		case 29:
			break;
		case 30:
			break;
		case 31:
			break;
		case 33:
			break;
		case 34:
			break;
		case 35:
			break;
		case 37:
			break;
		case 38:
			break;
		case 40:
			break;
		case 44:
			break;
		case 48:
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for MUL_EXP_TAIL rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_POW_EXP(Token token) {
	switch((int)token) {
		case 11:
			token = procedure_UNARY_EXP(token);
			token = procedure_POW_EXP_TAIL(token);
			break;
		case 12:
			token = procedure_UNARY_EXP(token);
			token = procedure_POW_EXP_TAIL(token);
			break;
		case 13:
			token = procedure_UNARY_EXP(token);
			token = procedure_POW_EXP_TAIL(token);
			break;
		case 22:
			token = procedure_UNARY_EXP(token);
			token = procedure_POW_EXP_TAIL(token);
			break;
		case 23:
			token = procedure_UNARY_EXP(token);
			token = procedure_POW_EXP_TAIL(token);
			break;
		case 24:
			token = procedure_UNARY_EXP(token);
			token = procedure_POW_EXP_TAIL(token);
			break;
		case 25:
			token = procedure_UNARY_EXP(token);
			token = procedure_POW_EXP_TAIL(token);
			break;
		case 26:
			token = procedure_UNARY_EXP(token);
			token = procedure_POW_EXP_TAIL(token);
			break;
		case 27:
			token = procedure_UNARY_EXP(token);
			token = procedure_POW_EXP_TAIL(token);
			break;
		case 43:
			token = procedure_UNARY_EXP(token);
			token = procedure_POW_EXP_TAIL(token);
			break;
		case 49:
			token = procedure_UNARY_EXP(token);
			token = procedure_POW_EXP_TAIL(token);
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for POW_EXP rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_POW_EXP_TAIL(Token token) {
	switch((int)token) {
		case 6:
			break;
		case 7:
			break;
		case 14:
			break;
		case 15:
			break;
		case 16:
			break;
		case 17:
			break;
		case 18:
			token = match("^", token);
			token = procedure_UNARY_EXP(token);
			token = procedure_POW_EXP_TAIL(token);
			break;
		case 20:
			break;
		case 21:
			break;
		case 22:
			break;
		case 29:
			break;
		case 30:
			break;
		case 31:
			break;
		case 33:
			break;
		case 34:
			break;
		case 35:
			break;
		case 37:
			break;
		case 38:
			break;
		case 40:
			break;
		case 44:
			break;
		case 48:
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for POW_EXP_TAIL rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_UNARY_EXP(Token token) {
	switch((int)token) {
		case 11:
			token = procedure_PRIMARY(token);
			break;
		case 12:
			token = procedure_PRIMARY(token);
			break;
		case 13:
			token = procedure_PRIMARY(token);
			break;
		case 22:
			token = match("comparison", token);
			token = procedure_UNARY_EXP(token);
			break;
		case 23:
			token = procedure_PRIMARY(token);
			break;
		case 24:
			token = procedure_PRIMARY(token);
			break;
		case 25:
			token = procedure_PRIMARY(token);
			break;
		case 26:
			token = procedure_PRIMARY(token);
			break;
		case 27:
			token = procedure_PRIMARY(token);
			break;
		case 43:
			token = procedure_PRIMARY(token);
			break;
		case 49:
			token = procedure_PRIMARY(token);
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for UNARY_EXP rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_PRIMARY(Token token) {
	switch((int)token) {
		case 11:
			token = match("new", token);
			token = match("name", token);
			token = procedure_ACCESS_TAIL(token);
			break;
		case 12:
			token = procedure_REF_VAR(token);
			token = procedure_ACCESS_TAIL(token);
			break;
		case 13:
			token = procedure_DEREF_VAR(token);
			token = procedure_ACCESS_TAIL(token);
			break;
		case 23:
			token = procedure_LITERAL(token);
			token = procedure_ACCESS_TAIL(token);
			break;
		case 24:
			token = procedure_LITERAL(token);
			token = procedure_ACCESS_TAIL(token);
			break;
		case 25:
			token = procedure_LITERAL(token);
			token = procedure_ACCESS_TAIL(token);
			break;
		case 26:
			token = procedure_LITERAL(token);
			token = procedure_ACCESS_TAIL(token);
			break;
		case 27:
			token = procedure_LITERAL(token);
			token = procedure_ACCESS_TAIL(token);
			break;
		case 43:
			token = match("(", token);
			token = procedure_EXP(token);
			token = match(")", token);
			token = procedure_ACCESS_TAIL(token);
			break;
		case 49:
			token = match("name", token);
			token = procedure_MAYBE_CALL(token);
			token = procedure_ACCESS_TAIL(token);
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for PRIMARY rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_REF_VAR(Token token) {
	switch((int)token) {
		case 12:
			token = match("ref", token);
			token = match("(", token);
			token = procedure_PRIMARY(token);
			token = match(")", token);
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for REF_VAR rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_DEREF_VAR(Token token) {
	switch((int)token) {
		case 13:
			token = match("deref", token);
			token = match("(", token);
			token = procedure_PRIMARY(token);
			token = match(")", token);
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for DEREF_VAR rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_MAYBE_CALL(Token token) {
	switch((int)token) {
		case 6:
			break;
		case 7:
			break;
		case 14:
			break;
		case 15:
			break;
		case 16:
			break;
		case 17:
			break;
		case 18:
			break;
		case 20:
			break;
		case 21:
			break;
		case 22:
			break;
		case 29:
			break;
		case 30:
			break;
		case 31:
			break;
		case 33:
			break;
		case 34:
			break;
		case 35:
			break;
		case 37:
			break;
		case 38:
			break;
		case 40:
			break;
		case 42:
			break;
		case 43:
			token = match("(", token);
			token = procedure_EXP_LIST(token);
			token = match(")", token);
			break;
		case 44:
			break;
		case 47:
			break;
		case 48:
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for MAYBE_CALL rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_EXP_LIST(Token token) {
	switch((int)token) {
		case 11:
			token = procedure_EXP(token);
			token = procedure_EXP_LIST_TAIL(token);
			break;
		case 12:
			token = procedure_EXP(token);
			token = procedure_EXP_LIST_TAIL(token);
			break;
		case 13:
			token = procedure_EXP(token);
			token = procedure_EXP_LIST_TAIL(token);
			break;
		case 22:
			token = procedure_EXP(token);
			token = procedure_EXP_LIST_TAIL(token);
			break;
		case 23:
			token = procedure_EXP(token);
			token = procedure_EXP_LIST_TAIL(token);
			break;
		case 24:
			token = procedure_EXP(token);
			token = procedure_EXP_LIST_TAIL(token);
			break;
		case 25:
			token = procedure_EXP(token);
			token = procedure_EXP_LIST_TAIL(token);
			break;
		case 26:
			token = procedure_EXP(token);
			token = procedure_EXP_LIST_TAIL(token);
			break;
		case 27:
			token = procedure_EXP(token);
			token = procedure_EXP_LIST_TAIL(token);
			break;
		case 43:
			token = procedure_EXP(token);
			token = procedure_EXP_LIST_TAIL(token);
			break;
		case 44:
			break;
		case 49:
			token = procedure_EXP(token);
			token = procedure_EXP_LIST_TAIL(token);
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for EXP_LIST rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_EXP_LIST_TAIL(Token token) {
	switch((int)token) {
		case 44:
			break;
		case 48:
			token = match(",", token);
			token = procedure_EXP(token);
			token = procedure_EXP_LIST_TAIL(token);
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for EXP_LIST_TAIL rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_SUMINUS(Token token) {
	switch((int)token) {
		case 14:
			token = match("+", token);
			break;
		case 15:
			token = match("-", token);
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for SUMINUS rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_MULDIV(Token token) {
	switch((int)token) {
		case 16:
			token = match("*", token);
			break;
		case 17:
			token = match("divides", token);
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for MULDIV rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_REL_OP(Token token) {
	switch((int)token) {
		case 22:
			token = match("comparison", token);
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for REL_OP rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_LITERAL(Token token) {
	switch((int)token) {
		case 23:
			token = match("int_literal", token);
			break;
		case 24:
			token = match("float literal", token);
			break;
		case 25:
			token = match("bool_literal", token);
			break;
		case 26:
			token = match("string_literal", token);
			break;
		case 27:
			token = match("null", token);
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for LITERAL rule with token " << token;
			exit(1);
	}
	return token;
}
Token procedure_TYPE(Token token) {
	switch((int)token) {
		case 0:
			token = match("int", token);
			break;
		case 1:
			token = match("float", token);
			break;
		case 2:
			token = match("bool", token);
			break;
		case 3:
			token = match("string", token);
			break;
		case 12:
			token = match("ref", token);
			token = match("(", token);
			token = procedure_TYPE(token);
			token = match(")", token);
			break;
		case 49:
			token = match("name", token);
			break;
		default:
			std::cout << "[SYNTAX ERROR] No rule found for TYPE rule with token " << token;
			exit(1);
	}
	return token;
}
