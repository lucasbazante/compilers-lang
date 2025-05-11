#include <cstdlib>
#include <iostream>
#include <sstream>
#include <variant>
#include <string>
#include "lexer.hpp"
#include "utils.hpp"




std::string token_to_string(Token token) {
   std::ostringstream oss;
   oss << token;
   return oss.str();
}


Token match(Token expected, Token actual) {

   if (actual == expected) {
       Token token = static_cast<Token>(yylex());
       return token;
   }
   else {
       // parar execução
       log_expected_another_token_error(actual);
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
			log_unexpected_token_error((Rule)0, token);			exit(1);
	}
	return token;
}
Token procedure_PROGRAM(Token token) {
	switch((int)token) {
		case 4:
			token = match((Token)4, token);
			token = match((Token)49, token);
			token = match((Token)5, token);
			token = procedure_DECLS_OPT(token);
			token = match((Token)7, token);
			break;
		default:
			log_unexpected_token_error((Rule)1, token);			exit(1);
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
			log_unexpected_token_error((Rule)2, token);			exit(1);
	}
	return token;
}
Token procedure_DECLS_LIST_OPT(Token token) {
	switch((int)token) {
		case 7:
			break;
		case 40:
			token = match((Token)40, token);
			token = procedure_DECL(token);
			token = procedure_DECLS_LIST_OPT(token);
			break;
		default:
			log_unexpected_token_error((Rule)3, token);			exit(1);
	}
	return token;
}
Token procedure_PROC_DECLS_OPT(Token token) {
	switch((int)token) {
		case 6:
			token = match((Token)6, token);
			break;
		case 7:
			break;
		case 8:
			token = procedure_DECL(token);
			token = procedure_PROC_DECLS_LIST_OPT(token);
			token = match((Token)6, token);
			break;
		case 9:
			token = procedure_DECL(token);
			token = procedure_PROC_DECLS_LIST_OPT(token);
			token = match((Token)6, token);
			break;
		case 10:
			token = procedure_DECL(token);
			token = procedure_PROC_DECLS_LIST_OPT(token);
			token = match((Token)6, token);
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
			log_unexpected_token_error((Rule)4, token);			exit(1);
	}
	return token;
}
Token procedure_PROC_DECLS_LIST_OPT(Token token) {
	switch((int)token) {
		case 6:
			break;
		case 40:
			token = match((Token)40, token);
			token = procedure_DECL(token);
			token = procedure_PROC_DECLS_LIST_OPT(token);
			break;
		default:
			log_unexpected_token_error((Rule)5, token);			exit(1);
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
			log_unexpected_token_error((Rule)6, token);			exit(1);
	}
	return token;
}
Token procedure_VAR_DECL(Token token) {
	switch((int)token) {
		case 8:
			token = match((Token)8, token);
			token = match((Token)49, token);
			token = procedure_VAR_DECL_TYPE_OPT(token);
			break;
		default:
			log_unexpected_token_error((Rule)7, token);			exit(1);
	}
	return token;
}
Token procedure_VAR_DECL_TYPE_OPT(Token token) {
	switch((int)token) {
		case 41:
			token = match((Token)41, token);
			token = procedure_TYPE(token);
			token = procedure_VAR_DECL_EXP_OPT(token);
			break;
		case 42:
			token = match((Token)42, token);
			token = procedure_EXP(token);
			break;
		default:
			log_unexpected_token_error((Rule)8, token);			exit(1);
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
			token = match((Token)42, token);
			token = procedure_EXP(token);
			break;
		default:
			log_unexpected_token_error((Rule)9, token);			exit(1);
	}
	return token;
}
Token procedure_PROC_DECL(Token token) {
	switch((int)token) {
		case 9:
			token = match((Token)9, token);
			token = match((Token)49, token);
			token = match((Token)43, token);
			token = procedure_PARAMFIELD_DECL_OPT(token);
			token = match((Token)44, token);
			token = procedure_PARAMFIELD_TYPE_OPT(token);
			token = match((Token)5, token);
			token = procedure_PROC_DECLS_OPT(token);
			token = procedure_STMT_LIST(token);
			token = match((Token)7, token);
			break;
		default:
			log_unexpected_token_error((Rule)10, token);			exit(1);
	}
	return token;
}
Token procedure_PARAMFIELD_TYPE_OPT(Token token) {
	switch((int)token) {
		case 5:
			break;
		case 41:
			token = match((Token)41, token);
			token = procedure_TYPE(token);
			break;
		default:
			log_unexpected_token_error((Rule)11, token);			exit(1);
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
			log_unexpected_token_error((Rule)12, token);			exit(1);
	}
	return token;
}
Token procedure_PARAMFIELD_DECL_LIST(Token token) {
	switch((int)token) {
		case 49:
			token = match((Token)49, token);
			token = match((Token)41, token);
			token = procedure_TYPE(token);
			token = procedure_PARAMFIELD_DECL_LIST_OPT(token);
			break;
		default:
			log_unexpected_token_error((Rule)13, token);			exit(1);
	}
	return token;
}
Token procedure_PARAMFIELD_DECL_LIST_OPT(Token token) {
	switch((int)token) {
		case 44:
			break;
		case 48:
			token = match((Token)48, token);
			token = procedure_PARAMFIELD_DECL_LIST(token);
			break;
		default:
			log_unexpected_token_error((Rule)14, token);			exit(1);
	}
	return token;
}
Token procedure_REC_DECL(Token token) {
	switch((int)token) {
		case 10:
			token = match((Token)10, token);
			token = match((Token)49, token);
			token = match((Token)45, token);
			token = procedure_PARAMFIELD_DECL_OPT_PRIME(token);
			token = match((Token)46, token);
			break;
		default:
			log_unexpected_token_error((Rule)15, token);			exit(1);
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
			log_unexpected_token_error((Rule)16, token);			exit(1);
	}
	return token;
}
Token procedure_PARAMFIELD_DECL_LIST_PRIME(Token token) {
	switch((int)token) {
		case 49:
			token = match((Token)49, token);
			token = match((Token)41, token);
			token = procedure_TYPE(token);
			token = procedure_PARAMFIELD_DECL_LIST_OPT_PRIME(token);
			break;
		default:
			log_unexpected_token_error((Rule)17, token);			exit(1);
	}
	return token;
}
Token procedure_PARAMFIELD_DECL_LIST_OPT_PRIME(Token token) {
	switch((int)token) {
		case 40:
			token = match((Token)40, token);
			token = procedure_PARAMFIELD_DECL_LIST_PRIME(token);
			break;
		case 46:
			break;
		default:
			log_unexpected_token_error((Rule)18, token);			exit(1);
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
			log_unexpected_token_error((Rule)19, token);			exit(1);
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
			token = match((Token)40, token);
			token = procedure_STMT_LIST(token);
			break;
		default:
			log_unexpected_token_error((Rule)20, token);			exit(1);
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
			log_unexpected_token_error((Rule)21, token);			exit(1);
	}
	return token;
}
Token procedure_IF_STMT(Token token) {
	switch((int)token) {
		case 28:
			token = match((Token)28, token);
			token = procedure_EXP(token);
			token = match((Token)29, token);
			token = procedure_STMT_LIST(token);
			token = procedure_IF_ELSE(token);
			token = match((Token)31, token);
			break;
		default:
			log_unexpected_token_error((Rule)22, token);			exit(1);
	}
	return token;
}
Token procedure_IF_ELSE(Token token) {
	switch((int)token) {
		case 30:
			token = match((Token)30, token);
			token = procedure_STMT_LIST(token);
			break;
		case 31:
			break;
		default:
			log_unexpected_token_error((Rule)23, token);			exit(1);
	}
	return token;
}
Token procedure_WHILE_STMT(Token token) {
	switch((int)token) {
		case 32:
			token = match((Token)32, token);
			token = procedure_EXP(token);
			token = match((Token)33, token);
			token = procedure_STMT_LIST(token);
			token = match((Token)35, token);
			break;
		default:
			log_unexpected_token_error((Rule)24, token);			exit(1);
	}
	return token;
}
Token procedure_FOR_STMT(Token token) {
	switch((int)token) {
		case 36:
			token = match((Token)36, token);
			token = procedure_PRIMARY(token);
			token = match((Token)22, token);
			token = procedure_EXP(token);
			token = match((Token)37, token);
			token = procedure_EXP(token);
			token = match((Token)38, token);
			token = procedure_EXP(token);
			token = match((Token)33, token);
			token = procedure_STMT_LIST(token);
			token = match((Token)35, token);
			break;
		default:
			log_unexpected_token_error((Rule)25, token);			exit(1);
	}
	return token;
}
Token procedure_DO_UNTIL_STMT(Token token) {
	switch((int)token) {
		case 33:
			token = match((Token)33, token);
			token = procedure_STMT_LIST(token);
			token = match((Token)34, token);
			token = procedure_EXP(token);
			token = match((Token)35, token);
			break;
		default:
			log_unexpected_token_error((Rule)26, token);			exit(1);
	}
	return token;
}
Token procedure_RETURN_STMT(Token token) {
	switch((int)token) {
		case 39:
			token = match((Token)39, token);
			token = match((Token)43, token);
			token = procedure_EXP(token);
			token = match((Token)44, token);
			break;
		default:
			log_unexpected_token_error((Rule)27, token);			exit(1);
	}
	return token;
}
Token procedure_CALL_OR_ASSIGNMENT(Token token) {
	switch((int)token) {
		case 11:
			token = match((Token)11, token);
			token = match((Token)49, token);
			token = procedure_ACCESS_TAIL(token);
			token = match((Token)42, token);
			token = procedure_EXP(token);
			break;
		case 12:
			token = procedure_REF_VAR(token);
			token = procedure_ACCESS_TAIL(token);
			token = match((Token)42, token);
			token = procedure_EXP(token);
			break;
		case 13:
			token = procedure_DEREF_VAR(token);
			token = procedure_ACCESS_TAIL(token);
			token = match((Token)42, token);
			token = procedure_EXP(token);
			break;
		case 23:
			token = procedure_LITERAL(token);
			token = procedure_ACCESS_TAIL(token);
			token = match((Token)42, token);
			token = procedure_EXP(token);
			break;
		case 24:
			token = procedure_LITERAL(token);
			token = procedure_ACCESS_TAIL(token);
			token = match((Token)42, token);
			token = procedure_EXP(token);
			break;
		case 25:
			token = procedure_LITERAL(token);
			token = procedure_ACCESS_TAIL(token);
			token = match((Token)42, token);
			token = procedure_EXP(token);
			break;
		case 26:
			token = procedure_LITERAL(token);
			token = procedure_ACCESS_TAIL(token);
			token = match((Token)42, token);
			token = procedure_EXP(token);
			break;
		case 27:
			token = procedure_LITERAL(token);
			token = procedure_ACCESS_TAIL(token);
			token = match((Token)42, token);
			token = procedure_EXP(token);
			break;
		case 43:
			token = match((Token)43, token);
			token = procedure_EXP(token);
			token = match((Token)44, token);
			token = procedure_ACCESS_TAIL(token);
			token = match((Token)42, token);
			token = procedure_EXP(token);
			break;
		case 49:
			token = match((Token)49, token);
			token = procedure_MAYBE_CALL(token);
			token = procedure_MAYBE_ACCESS_TAIL(token);
			break;
		default:
			log_unexpected_token_error((Rule)28, token);			exit(1);
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
			token = match((Token)47, token);
			token = match((Token)49, token);
			token = procedure_ACCESS_TAIL(token);
			break;
		case 48:
			break;
		default:
			log_unexpected_token_error((Rule)29, token);			exit(1);
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
			token = match((Token)42, token);
			token = procedure_EXP(token);
			break;
		case 47:
			token = procedure_ACCESS_TAIL(token);
			token = match((Token)42, token);
			token = procedure_EXP(token);
			break;
		default:
			log_unexpected_token_error((Rule)30, token);			exit(1);
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
			log_unexpected_token_error((Rule)31, token);			exit(1);
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
			log_unexpected_token_error((Rule)32, token);			exit(1);
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
			token = match((Token)21, token);
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
			log_unexpected_token_error((Rule)33, token);			exit(1);
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
			log_unexpected_token_error((Rule)34, token);			exit(1);
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
			token = match((Token)20, token);
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
			log_unexpected_token_error((Rule)35, token);			exit(1);
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
			log_unexpected_token_error((Rule)36, token);			exit(1);
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
			log_unexpected_token_error((Rule)37, token);			exit(1);
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
			log_unexpected_token_error((Rule)38, token);			exit(1);
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
			log_unexpected_token_error((Rule)39, token);			exit(1);
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
			log_unexpected_token_error((Rule)40, token);			exit(1);
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
			log_unexpected_token_error((Rule)41, token);			exit(1);
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
			log_unexpected_token_error((Rule)42, token);			exit(1);
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
			token = match((Token)18, token);
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
			log_unexpected_token_error((Rule)43, token);			exit(1);
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
			token = match((Token)22, token);
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
			log_unexpected_token_error((Rule)44, token);			exit(1);
	}
	return token;
}
Token procedure_PRIMARY(Token token) {
	switch((int)token) {
		case 11:
			token = match((Token)11, token);
			token = match((Token)49, token);
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
			token = match((Token)43, token);
			token = procedure_EXP(token);
			token = match((Token)44, token);
			token = procedure_ACCESS_TAIL(token);
			break;
		case 49:
			token = match((Token)49, token);
			token = procedure_MAYBE_CALL(token);
			token = procedure_ACCESS_TAIL(token);
			break;
		default:
			log_unexpected_token_error((Rule)45, token);			exit(1);
	}
	return token;
}
Token procedure_REF_VAR(Token token) {
	switch((int)token) {
		case 12:
			token = match((Token)12, token);
			token = match((Token)43, token);
			token = procedure_PRIMARY(token);
			token = match((Token)44, token);
			break;
		default:
			log_unexpected_token_error((Rule)46, token);			exit(1);
	}
	return token;
}
Token procedure_DEREF_VAR(Token token) {
	switch((int)token) {
		case 13:
			token = match((Token)13, token);
			token = match((Token)43, token);
			token = procedure_PRIMARY(token);
			token = match((Token)44, token);
			break;
		default:
			log_unexpected_token_error((Rule)47, token);			exit(1);
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
			token = match((Token)43, token);
			token = procedure_EXP_LIST(token);
			token = match((Token)44, token);
			break;
		case 44:
			break;
		case 47:
			break;
		case 48:
			break;
		default:
			log_unexpected_token_error((Rule)48, token);			exit(1);
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
			log_unexpected_token_error((Rule)49, token);			exit(1);
	}
	return token;
}
Token procedure_EXP_LIST_TAIL(Token token) {
	switch((int)token) {
		case 44:
			break;
		case 48:
			token = match((Token)48, token);
			token = procedure_EXP(token);
			token = procedure_EXP_LIST_TAIL(token);
			break;
		default:
			log_unexpected_token_error((Rule)50, token);			exit(1);
	}
	return token;
}
Token procedure_SUMINUS(Token token) {
	switch((int)token) {
		case 14:
			token = match((Token)14, token);
			break;
		case 15:
			token = match((Token)15, token);
			break;
		default:
			log_unexpected_token_error((Rule)51, token);			exit(1);
	}
	return token;
}
Token procedure_MULDIV(Token token) {
	switch((int)token) {
		case 16:
			token = match((Token)16, token);
			break;
		case 17:
			token = match((Token)17, token);
			break;
		default:
			log_unexpected_token_error((Rule)52, token);			exit(1);
	}
	return token;
}
Token procedure_REL_OP(Token token) {
	switch((int)token) {
		case 22:
			token = match((Token)22, token);
			break;
		default:
			log_unexpected_token_error((Rule)53, token);			exit(1);
	}
	return token;
}
Token procedure_LITERAL(Token token) {
	switch((int)token) {
		case 23:
			token = match((Token)23, token);
			break;
		case 24:
			token = match((Token)24, token);
			break;
		case 25:
			token = match((Token)25, token);
			break;
		case 26:
			token = match((Token)26, token);
			break;
		case 27:
			token = match((Token)27, token);
			break;
		default:
			log_unexpected_token_error((Rule)54, token);			exit(1);
	}
	return token;
}
Token procedure_TYPE(Token token) {
	switch((int)token) {
		case 0:
			token = match((Token)0, token);
			break;
		case 1:
			token = match((Token)1, token);
			break;
		case 2:
			token = match((Token)2, token);
			break;
		case 3:
			token = match((Token)3, token);
			break;
		case 12:
			token = match((Token)12, token);
			token = match((Token)43, token);
			token = procedure_TYPE(token);
			token = match((Token)44, token);
			break;
		case 49:
			token = match((Token)49, token);
			break;
		default:
			log_unexpected_token_error((Rule)55, token);			exit(1);
	}
	return token;
}
