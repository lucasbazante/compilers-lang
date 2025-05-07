#include <vector>
#include <stack>
#include <string>
#include <sstream>
#include <tuple>

enum class Symbol {
    // Non-terminals
    PROGRAM,
    DECLS_OPT,
    DECLS_LIST_OPT,
    PROC_DECLS_OPT,
    PROC_DECLS_LIST_OPT,
    DECL,
    VAR_DECL,
    VAR_DECL_TYPE_OPT,
    VAR_DECL_EXP_OPT,
    PROC_DECL,
    PARAMFIELD_TYPE_OPT,
    PARAMFIELD_DECL_OPT,
    PARAMFIELD_DECL_LIST,
    PARAMFIELD_DECL_LIST_OPT,
    REC_DECL,
    PARAMFIELD_DECL_OPT_PRIME,
    PARAMFIELD_DECL_LIST_PRIME,
    PARAMFIELD_DECL_LIST_OPT_PRIME,
    STMT_LIST,
    STMT_TAIL,
    STMT,
    EXP,
    OR_EXP,
    OR_EXP_TAIL,
    AND_EXP,
    AND_EXP_TAIL,
    REL_EXP,
    REL_EXP_TAIL,
    ADD_EXP,
    ADD_EXP_TAIL,
    MUL_EXP,
    MUL_EXP_TAIL,
    POW_EXP,
    POW_EXP_TAIL,
    UNARY_EXP,
    PRIMARY,
    ACCESS_TAIL,
    REF_VAR,
    DEREF_VAR,
    MAYBE_CALL,
    EXP_LIST,
    EXP_TAIL,
    MAYBE_ACESS_TAIL,
    CALL_OR_ASSIGNMET,
    IF_STMT,
    IF_ELSE,
    WHILE_STMT,
    RETURN_STMT,
    TYPE,
    FOR_STMT,
    DO_UNTIL_STMT,
    REL_OP,
    SUMINUS,
    MULDIV,
    LITERAL,


    // Terminals
    T_program,
    T_name,
    T_begin,
    T_end,
    T_var,
    T_procedure,
    T_lparen,     // (
    T_rparen,     // )
    T_colon,      // :
    T_semicolon,  // ;
    T_comma,      // ,
    T_dot,        // .
    T_struct,
    T_lbrace,     // {
    T_rbrace,     // }
    T_assign,     // :=
    T_or,         // ||
    T_and,        // &&
    T_not,
    T_plus,       // +
    T_minus,      // -
    T_multiply,   // *
    T_divide,        // /
    T_power,        // ^
    T_lt,
    T_le,
    T_gt,
    T_ge,
    T_eq,
    T_neq,        // <>
    T_float_literal,
    T_int_literal,
    T_string_literal,
    T_bool_literal,
    T_null,
    T_if,
    T_then,
    T_else,
    T_fi,
    T_while,
    T_do,
    T_od,
    T_return,
    T_float,
    T_int,
    T_string,
    T_bool,
    T_ref,
    T_deref,
    T_new,
    T_for,
    T_to,
    T_step,
    T_until,
    T_in,
    EPSILON  // Represents ''
};

struct Rule {
    Symbol lhs;
    std::vector<Symbol> rhs;
};

std::vector<Rule> grammar = {
    { Symbol::PROGRAM, { Symbol::T_program, Symbol::T_name, Symbol::T_begin, Symbol::DECLS_OPT, Symbol::T_end } },
    
    { Symbol::DECLS_OPT, { Symbol::DECL, Symbol::DECLS_LIST_OPT } },
    { Symbol::DECLS_OPT, { Symbol::EPSILON } },
    
    { Symbol::DECLS_LIST_OPT, { Symbol::T_semicolon, Symbol::DECL, Symbol::DECLS_LIST_OPT } },
    { Symbol::DECLS_LIST_OPT, { Symbol::EPSILON } },
    
    { Symbol::PROC_DECLS_OPT, { Symbol::DECL, Symbol::PROC_DECLS_LIST_OPT, Symbol::T_in } },
    { Symbol::PROC_DECLS_OPT, { Symbol::EPSILON } },
    
    { Symbol::PROC_DECLS_LIST_OPT, { Symbol::T_semicolon, Symbol::DECL, Symbol::PROC_DECLS_LIST_OPT } },
    { Symbol::PROC_DECLS_LIST_OPT, { Symbol::EPSILON } },
    
    { Symbol::DECL, { Symbol::VAR_DECL } },
    { Symbol::DECL, { Symbol::PROC_DECL } },
    { Symbol::DECL, { Symbol::REC_DECL } },
    
    { Symbol::VAR_DECL, { Symbol::T_var, Symbol::T_name, Symbol::VAR_DECL_TYPE_OPT } },
    
    { Symbol::VAR_DECL_TYPE_OPT, { Symbol::T_colon, Symbol::TYPE, Symbol::VAR_DECL_EXP_OPT } },
    { Symbol::VAR_DECL_TYPE_OPT, { Symbol::T_assign, Symbol::EXP } },
    
    { Symbol::VAR_DECL_EXP_OPT, { Symbol::T_assign, Symbol::EXP } },
    { Symbol::VAR_DECL_EXP_OPT, { Symbol::EPSILON } },
    
    { Symbol::PROC_DECL, { Symbol::T_procedure, Symbol::T_name, Symbol::T_lparen, Symbol::PARAMFIELD_DECL_OPT, Symbol::T_rparen, Symbol::PARAMFIELD_TYPE_OPT, Symbol::T_begin, Symbol::PROC_DECLS_OPT, Symbol::STMT_LIST, Symbol::T_end } },
    
    { Symbol::PARAMFIELD_TYPE_OPT, { Symbol::T_colon, Symbol::TYPE } },
    { Symbol::PARAMFIELD_TYPE_OPT, { Symbol::EPSILON } },
    
    { Symbol::PARAMFIELD_DECL_OPT, { Symbol::PARAMFIELD_DECL_LIST } },
    { Symbol::PARAMFIELD_DECL_OPT, { Symbol::EPSILON } },
    
    { Symbol::PARAMFIELD_DECL_LIST, { Symbol::T_name, Symbol::T_colon, Symbol::TYPE, Symbol::PARAMFIELD_DECL_LIST_OPT } },
    
    { Symbol::PARAMFIELD_DECL_LIST_OPT, { Symbol::T_comma, Symbol::PARAMFIELD_DECL_LIST } },
    { Symbol::PARAMFIELD_DECL_LIST_OPT, { Symbol::EPSILON } },
    
    { Symbol::REC_DECL, { Symbol::T_struct, Symbol::T_name, Symbol::T_lbrace, Symbol::PARAMFIELD_DECL_OPT_PRIME, Symbol::T_rbrace } },
    
    { Symbol::PARAMFIELD_DECL_OPT_PRIME, { Symbol::PARAMFIELD_DECL_LIST_PRIME } },
    { Symbol::PARAMFIELD_DECL_OPT_PRIME, { Symbol::EPSILON } },
    
    { Symbol::PARAMFIELD_DECL_LIST_PRIME, { Symbol::T_name, Symbol::T_colon, Symbol::TYPE, Symbol::PARAMFIELD_DECL_LIST_OPT_PRIME } },
    
    { Symbol::PARAMFIELD_DECL_LIST_OPT_PRIME, { Symbol::T_semicolon, Symbol::PARAMFIELD_DECL_LIST_PRIME } },
    { Symbol::PARAMFIELD_DECL_LIST_OPT_PRIME, { Symbol::EPSILON } },
    
    { Symbol::STMT_LIST, { Symbol::STMT, Symbol::STMT_TAIL } },
    { Symbol::STMT_LIST, { Symbol::EPSILON } },
    
    { Symbol::STMT_TAIL, { Symbol::T_semicolon, Symbol::STMT_LIST } },
    { Symbol::STMT_TAIL, { Symbol::EPSILON } },
    
    { Symbol::EXP, { Symbol::OR_EXP } },
    
    { Symbol::OR_EXP, { Symbol::AND_EXP, Symbol::OR_EXP_TAIL } },
    
    { Symbol::OR_EXP_TAIL, { Symbol::T_or, Symbol::AND_EXP, Symbol::OR_EXP_TAIL } },
    { Symbol::OR_EXP_TAIL, { Symbol::EPSILON } },
    
    { Symbol::AND_EXP, { Symbol::REL_EXP, Symbol::AND_EXP_TAIL } },
    
    { Symbol::AND_EXP_TAIL, { Symbol::T_and, Symbol::REL_EXP, Symbol::AND_EXP_TAIL } },
    { Symbol::AND_EXP_TAIL, { Symbol::EPSILON } },
    
    { Symbol::REL_EXP, { Symbol::ADD_EXP, Symbol::REL_EXP_TAIL } },
    
    { Symbol::REL_EXP_TAIL, { Symbol::REL_OP, Symbol::ADD_EXP, Symbol::REL_EXP_TAIL } },
    { Symbol::REL_EXP_TAIL, { Symbol::EPSILON } },
    
    { Symbol::ADD_EXP, { Symbol::MUL_EXP, Symbol::ADD_EXP_TAIL } },
    
    { Symbol::ADD_EXP_TAIL, { Symbol::SUMINUS, Symbol::MUL_EXP, Symbol::ADD_EXP_TAIL } },
    { Symbol::ADD_EXP_TAIL, { Symbol::EPSILON } },
    
    { Symbol::MUL_EXP, { Symbol::POW_EXP, Symbol::MUL_EXP_TAIL } },
    
    { Symbol::MUL_EXP_TAIL, { Symbol::MULDIV, Symbol::POW_EXP, Symbol::MUL_EXP_TAIL } },
    { Symbol::MUL_EXP_TAIL, { Symbol::EPSILON } },
    
    { Symbol::POW_EXP, { Symbol::UNARY_EXP, Symbol::POW_EXP_TAIL } },
    
    { Symbol::POW_EXP_TAIL, { Symbol::T_power, Symbol::UNARY_EXP, Symbol::POW_EXP_TAIL } },
    { Symbol::POW_EXP_TAIL, { Symbol::EPSILON } },
    
    { Symbol::UNARY_EXP, { Symbol::T_not, Symbol::UNARY_EXP } },
    { Symbol::UNARY_EXP, { Symbol::PRIMARY } },
    
    { Symbol::MAYBE_ACESS_TAIL, { Symbol::ACCESS_TAIL, Symbol::T_assign, Symbol::EXP } },
    { Symbol::MAYBE_ACESS_TAIL, { Symbol::EPSILON } },
    
    { Symbol::CALL_OR_ASSIGNMET, { Symbol::T_name, Symbol::MAYBE_CALL, Symbol::MAYBE_ACESS_TAIL } },
    { Symbol::CALL_OR_ASSIGNMET, { Symbol::LITERAL, Symbol::ACCESS_TAIL, Symbol::T_assign, Symbol::EXP } },
    { Symbol::CALL_OR_ASSIGNMET, { Symbol::T_new, Symbol::T_name, Symbol::ACCESS_TAIL, Symbol::T_assign, Symbol::EXP } },
    { Symbol::CALL_OR_ASSIGNMET, { Symbol::REF_VAR, Symbol::ACCESS_TAIL, Symbol::T_assign, Symbol::EXP } },
    { Symbol::CALL_OR_ASSIGNMET, { Symbol::DEREF_VAR, Symbol::ACCESS_TAIL, Symbol::T_assign, Symbol::EXP } },
    { Symbol::CALL_OR_ASSIGNMET, { Symbol::T_lparen, Symbol::EXP, Symbol::T_rparen, Symbol::ACCESS_TAIL, Symbol::T_assign, Symbol::EXP } },
    
    { Symbol::PRIMARY, { Symbol::LITERAL, Symbol::ACCESS_TAIL } },
    { Symbol::PRIMARY, { Symbol::T_name, Symbol::MAYBE_CALL, Symbol::ACCESS_TAIL } },
    { Symbol::PRIMARY, { Symbol::T_new, Symbol::T_name, Symbol::ACCESS_TAIL } },
    { Symbol::PRIMARY, { Symbol::REF_VAR, Symbol::ACCESS_TAIL } },
    { Symbol::PRIMARY, { Symbol::DEREF_VAR, Symbol::ACCESS_TAIL } },
    { Symbol::PRIMARY, { Symbol::T_lparen, Symbol::EXP, Symbol::T_rparen, Symbol::ACCESS_TAIL } },
    
    { Symbol::ACCESS_TAIL, { Symbol::T_dot, Symbol::T_name, Symbol::ACCESS_TAIL } },
    { Symbol::ACCESS_TAIL, { Symbol::EPSILON } },
    
    { Symbol::REF_VAR, { Symbol::T_ref, Symbol::T_lparen, Symbol::PRIMARY, Symbol::T_rparen } },
    
    { Symbol::DEREF_VAR, { Symbol::T_deref, Symbol::T_lparen, Symbol::PRIMARY, Symbol::T_rparen } },
    
    { Symbol::MAYBE_CALL, { Symbol::T_lparen, Symbol::EXP_LIST, Symbol::T_rparen } },
    { Symbol::MAYBE_CALL, { Symbol::EPSILON } },
    
    { Symbol::EXP_LIST, { Symbol::EXP, Symbol::EXP_TAIL } },
    { Symbol::EXP_LIST, { Symbol::EPSILON } },
    
    { Symbol::EXP_TAIL, { Symbol::T_comma, Symbol::EXP, Symbol::EXP_TAIL } },
    { Symbol::EXP_TAIL, { Symbol::EPSILON } },
    
    { Symbol::SUMINUS, { Symbol::T_plus } },
    { Symbol::SUMINUS, { Symbol::T_minus } },
    
    { Symbol::MULDIV, { Symbol::T_multiply } },
    { Symbol::MULDIV, { Symbol::T_divide } },
    
    { Symbol::REL_OP, { Symbol::T_lt } },
    { Symbol::REL_OP, { Symbol::T_le } },
    { Symbol::REL_OP, { Symbol::T_gt } },
    { Symbol::REL_OP, { Symbol::T_ge } },
    { Symbol::REL_OP, { Symbol::T_eq } },
    { Symbol::REL_OP, { Symbol::T_neq } },
    
    { Symbol::LITERAL, { Symbol::T_float_literal } },
    { Symbol::LITERAL, { Symbol::T_int_literal } },
    { Symbol::LITERAL, { Symbol::T_string_literal } },
    { Symbol::LITERAL, { Symbol::T_bool_literal } },
    { Symbol::LITERAL, { Symbol::T_null } },
    
    { Symbol::STMT, { Symbol::IF_STMT } },
    { Symbol::STMT, { Symbol::WHILE_STMT } },
    { Symbol::STMT, { Symbol::RETURN_STMT } },
    { Symbol::STMT, { Symbol::CALL_OR_ASSIGNMET } },
    { Symbol::STMT, { Symbol::FOR_STMT } },
    { Symbol::STMT, { Symbol::DO_UNTIL_STMT } },
    
    { Symbol::IF_STMT, { Symbol::T_if, Symbol::EXP, Symbol::T_then, Symbol::STMT_LIST, Symbol::IF_ELSE, Symbol::T_fi } },
    
    { Symbol::IF_ELSE, { Symbol::T_else, Symbol::STMT_LIST } },
    { Symbol::IF_ELSE, { Symbol::EPSILON } },
    
    { Symbol::WHILE_STMT, { Symbol::T_while, Symbol::EXP, Symbol::T_do, Symbol::STMT_LIST, Symbol::T_od } },
    
    { Symbol::RETURN_STMT, { Symbol::T_return, Symbol::T_lparen, Symbol::EXP, Symbol::T_rparen } },
    
    { Symbol::TYPE, { Symbol::T_float } },
    { Symbol::TYPE, { Symbol::T_int } },
    { Symbol::TYPE, { Symbol::T_string } },
    { Symbol::TYPE, { Symbol::T_bool } },
    { Symbol::TYPE, { Symbol::T_name } },
    { Symbol::TYPE, { Symbol::T_ref, Symbol::T_lparen, Symbol::TYPE, Symbol::T_rparen } },
    
    { Symbol::FOR_STMT, { Symbol::T_for, Symbol::PRIMARY, Symbol::T_eq, Symbol::EXP, Symbol::T_to, Symbol::EXP, Symbol::T_step, Symbol::EXP, Symbol::T_do, Symbol::STMT_LIST, Symbol::T_od } },
    
    { Symbol::DO_UNTIL_STMT, { Symbol::T_do, Symbol::STMT_LIST, Symbol::T_until, Symbol::EXP, Symbol::T_od } }
};

std::unordered_map<Symbol, std::string> symbol_to_string = {
    {Symbol::T_program, "program"},
    {Symbol::T_name, "name"},
    {Symbol::T_begin, "begin"},
    {Symbol::T_end, "end"},
    {Symbol::T_semicolon, ";"},
    {Symbol::T_in, "in"},
    {Symbol::T_var, "var"},
    {Symbol::T_colon, ":"},
    {Symbol::T_assign, ":="},
    {Symbol::T_procedure, "procedure"},
    {Symbol::T_lparen, "("},
    {Symbol::T_rparen, ")"},
    {Symbol::T_comma, ","},
    {Symbol::T_struct, "struct"},
    {Symbol::T_lbrace, "{"},
    {Symbol::T_rbrace, "}"},
    {Symbol::T_or, "||"},
    {Symbol::T_and, "&&"},
    {Symbol::T_power, "^"},
    {Symbol::T_not, "not"},
    {Symbol::T_new, "new"},
    {Symbol::T_dot, "."},
    {Symbol::T_ref, "ref"},
    {Symbol::T_deref, "deref"},
    {Symbol::T_plus, "+"},
    {Symbol::T_minus, "-"},
    {Symbol::T_multiply, "*"},
    {Symbol::T_divide, "/"},
    {Symbol::T_lt, "<"},
    {Symbol::T_le, "<="},
    {Symbol::T_gt, ">"},
    {Symbol::T_ge, ">="},
    {Symbol::T_eq, "="},
    {Symbol::T_neq, "<>"},
    {Symbol::T_float_literal, "float_literal"},
    {Symbol::T_int_literal, "int_literal"},
    {Symbol::T_string_literal, "string_literal"},
    {Symbol::T_bool_literal, "bool_literal"},
    {Symbol::T_null, "null"},
    {Symbol::T_if, "if"},
    {Symbol::T_then, "then"},
    {Symbol::T_fi, "fi"},
    {Symbol::T_else, "else"},
    {Symbol::T_while, "while"},
    {Symbol::T_do, "do"},
    {Symbol::T_od, "od"},
    {Symbol::T_return, "return"},
    {Symbol::T_float, "float"},
    {Symbol::T_int, "int"},
    {Symbol::T_string, "string"},
    {Symbol::T_bool, "bool"},
    {Symbol::T_for, "for"},
    {Symbol::T_to, "to"},
    {Symbol::T_step, "step"},
    {Symbol::T_until, "until"},
    {Symbol::EPSILON, "e"}
};

bool is_terminal(Symbol symbol) {
	return (int) symbol > (int) Symbol::LITERAL;
}

std::vector<Rule> symbol_to_rules(Symbol symbol) {

	// CAN BE OPTMIZED

	std::vector<Rule> rules;

	for (Rule rule : grammar) {
		if (rule.lhs == symbol) {
			rules.push_back(rule);
		}
	}
	
	return rules;
}

std::string token_to_string(Token token) {
	std::ostringstream oss;
	oss << token;
	return oss.str();
}

std::string rule_to_string(Rule rule) {
	std::string s;

	for (Symbol symbol : rule.rhs) {
		s += is_terminal(symbol) ? symbol_to_string.at(symbol) : "~" + std::to_string((int)symbol) + "~";
		s += " ";
	}

	return s;
}

Token get_next_token() {
	return static_cast<Token>(yylex());
}

class RecursiveParsing {
 private:
	static std::stack<std::vector<Rule>> state_of_execution;
 public:
 	static Token token;

	static void save_state(Symbol symbol, Rule current_state, std::vector<Rule> rules) {
        Rule new_rule;
        new_rule.lhs = current_state.lhs;
        bool can_push_back = false;
        for (int i = 0;i < current_state.rhs.size();i ++) {
            if (can_push_back) new_rule.rhs.push_back(current_state.rhs[i]);
            if (current_state.rhs[i] == symbol) can_push_back = true;
        }
        save_state(new_rule);
		state_of_execution.push(rules);
	}

    static void save_state(Rule rule) {
        state_of_execution.push({rule});
    }

	static std::vector<Rule> get_state() {
        std::vector<Rule> elements = state_of_execution.top();
        state_of_execution.pop();
        return elements;
	}

    static bool eof() {
        return token == Token::TEOF;
    }

	static void start() {
		state_of_execution.push({grammar[0]});
		token = get_next_token();
	}

};

Token RecursiveParsing::token;
std::stack<std::vector<Rule>> RecursiveParsing::state_of_execution;