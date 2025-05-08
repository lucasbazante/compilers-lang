#include <variant>
#include <vector>
#include <optional>
#include <iostream>
#include <string_view>

#include "tokens.hpp"

// An enum for each rule we have.
// Its integer conversion will index our table.
enum class Rule {
    // Start
    S = 0,

    // Declarations
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

    // Statements
    STMT_LIST,
    STMT_TAIL,
    STMT,
    IF_STMT,
    IF_ELSE,
    WHILE_STMT,
    FOR_STMT,
    DO_UNTIL_STMT,
    RETURN_STMT,
    CALL_OR_ASSIGNMENT,

    // Access Tail
    ACCESS_TAIL,
    MAYBE_ACCESS_TAIL,

    // Expressions
    EXP,
    OR_EXP,
    OR_EXP_LIST_TAIL,
    AND_EXP,
    AND_EXP_LIST_TAIL,
    REL_EXP,
    REL_EXP_LIST_TAIL,
    ADD_EXP,
    ADD_EXP_LIST_TAIL,
    MUL_EXP,
    MUL_EXP_LIST_TAIL,
    POW_EXP,
    POW_EXP_LIST_TAIL,
    UNARY_EXP,
    PRIMARY,
    REF_VAR,
    DEREF_VAR,
    MAYBE_CALL,
    EXP_LIST,
    EXP_LIST_TAIL,

    // Operators
    SUMINUS,
    MULDIV,
    REL_OP,
    LITERAL,

    // Type names
    TYPE,
    
    // If our enums start from 0, this one
    // will have the total count of cases.
    COUNT
};

inline std::ostream& operator<<(std::ostream& os, Rule rule) {
    constexpr std::string_view names[] = {
        "S",
        "PROGRAM",
        "DECLS_OPT",
        "DECLS_LIST_OPT",
        "PROC_DECLS_OPT",
        "PROC_DECLS_LIST_OPT",
        "DECL",
        "VAR_DECL",
        "VAR_DECL_TYPE_OPT",
        "VAR_DECL_EXP_OPT",
        "PROC_DECL",
        "PARAMFIELD_TYPE_OPT",
        "PARAMFIELD_DECL_OPT",
        "PARAMFIELD_DECL_LIST",
        "PARAMFIELD_DECL_LIST_OPT",
        "REC_DECL",
        "PARAMFIELD_DECL_OPT_PRIME",
        "PARAMFIELD_DECL_LIST_PRIME",
        "PARAMFIELD_DECL_LIST_OPT_PRIME",
        "STMT_LIST",
        "STMT_TAIL",
        "STMT",
        "IF_STMT",
        "IF_ELSE",
        "WHILE_STMT",
        "FOR_STMT",
        "DO_UNTIL_STMT",
        "RETURN_STMT",
        "CALL_OR_ASSIGNMENT",
        "ACCESS_TAIL",
        "MAYBE_ACCESS_TAIL",
        "EXP",
        "OR_EXP",
        "OR_EXP_LIST_TAIL",
        "AND_EXP",
        "AND_EXP_LIST_TAIL",
        "REL_EXP",
        "REL_EXP_LIST_TAIL",
        "ADD_EXP",
        "ADD_EXP_LIST_TAIL",
        "MUL_EXP",
        "MUL_EXP_LIST_TAIL",
        "POW_EXP",
        "POW_EXP_LIST_TAIL",
        "UNARY_EXP",
        "PRIMARY",
        "REF_VAR",
        "DEREF_VAR",
        "MAYBE_CALL",
        "EXP_LIST",
        "EXP_LIST_TAIL",
        "SUMINUS",
        "MULDIV",
        "REL_OP",
        "LITERAL",
        "TYPE",
        "COUNT"
    };

    auto index = static_cast<std::size_t>(rule);
    if (index < static_cast<std::size_t>(Rule::COUNT)) {
        os << names[index];
    } else {
        os << "UNKNOWN_RULE";
    }
    return os;
}

// Symbols are either a Token or a grammar Rule.
using Symbol = std::variant<Token, Rule>;

// The entries of the tables are symbol sequences
// from which we populate the stack.
using SymbolSequence = std::vector<Symbol>;

/* Finally, our table type.
 
 * A vector indexed by grammar rules and terminal symbols,
   representing the table, in which the lines are indexed
   by grammar rules, and the columns the terminal symbols,
   and for each entry rule × symbol, there is a sequence
   of terminals and non-terminals that we need to stack.

 * Each sequence is wrapped in a std::optional, because for
   some rule × symbol entries, there may not be a suitable
   sequence, indicating parsing error.
*/
using ParseTable = std::vector<std::vector< std::optional<SymbolSequence> >>;

inline const std::optional<SymbolSequence>& get_derivation(Rule rule, Token token, ParseTable& table) {
   return table[static_cast<size_t>(rule)][static_cast<size_t>(token)];
}

inline ParseTable build_parse_table() {
    const size_t TOKENS_COUNT = static_cast<size_t>(Token::COUNT);
    const size_t RULES_COUNT = static_cast<size_t>(Rule::COUNT);

    ParseTable table(
	RULES_COUNT,
	std::vector<std::optional<SymbolSequence>>(TOKENS_COUNT)
    );


    {
	table[static_cast<size_t>(Rule::S)][static_cast<size_t>(Token::Program)] = SymbolSequence{Rule::PROGRAM, Token::TEOF};
	table[static_cast<size_t>(Rule::PROGRAM)][static_cast<size_t>(Token::Program)] = SymbolSequence{Token::Program, Token::Identifier, Token::Begin, Rule::DECLS_OPT, Token::End};
	table[static_cast<size_t>(Rule::DECLS_OPT)][static_cast<size_t>(Token::End)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::DECLS_OPT)][static_cast<size_t>(Token::Var)] = SymbolSequence{Rule::DECL, Rule::DECLS_LIST_OPT};
	table[static_cast<size_t>(Rule::DECLS_OPT)][static_cast<size_t>(Token::Procedure)] = SymbolSequence{Rule::DECL, Rule::DECLS_LIST_OPT};
	table[static_cast<size_t>(Rule::DECLS_OPT)][static_cast<size_t>(Token::Struct)] = SymbolSequence{Rule::DECL, Rule::DECLS_LIST_OPT};
	table[static_cast<size_t>(Rule::DECLS_LIST_OPT)][static_cast<size_t>(Token::End)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::DECLS_LIST_OPT)][static_cast<size_t>(Token::Semicolon)] = SymbolSequence{Token::Semicolon, Rule::DECL, Rule::DECLS_LIST_OPT};
	table[static_cast<size_t>(Rule::PROC_DECLS_OPT)][static_cast<size_t>(Token::Identifier)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::PROC_DECLS_OPT)][static_cast<size_t>(Token::End)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::PROC_DECLS_OPT)][static_cast<size_t>(Token::Var)] = SymbolSequence{Rule::DECL, Rule::PROC_DECLS_LIST_OPT, Token::In};
	table[static_cast<size_t>(Rule::PROC_DECLS_OPT)][static_cast<size_t>(Token::Procedure)] = SymbolSequence{Rule::DECL, Rule::PROC_DECLS_LIST_OPT, Token::In};
	table[static_cast<size_t>(Rule::PROC_DECLS_OPT)][static_cast<size_t>(Token::L_Paren)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::PROC_DECLS_OPT)][static_cast<size_t>(Token::Struct)] = SymbolSequence{Rule::DECL, Rule::PROC_DECLS_LIST_OPT, Token::In};
	table[static_cast<size_t>(Rule::PROC_DECLS_OPT)][static_cast<size_t>(Token::New)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::PROC_DECLS_OPT)][static_cast<size_t>(Token::Ref)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::PROC_DECLS_OPT)][static_cast<size_t>(Token::Deref)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::PROC_DECLS_OPT)][static_cast<size_t>(Token::Float_L)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::PROC_DECLS_OPT)][static_cast<size_t>(Token::Int_L)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::PROC_DECLS_OPT)][static_cast<size_t>(Token::String_L)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::PROC_DECLS_OPT)][static_cast<size_t>(Token::Bool_L)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::PROC_DECLS_OPT)][static_cast<size_t>(Token::Null)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::PROC_DECLS_OPT)][static_cast<size_t>(Token::If)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::PROC_DECLS_OPT)][static_cast<size_t>(Token::While)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::PROC_DECLS_OPT)][static_cast<size_t>(Token::Do)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::PROC_DECLS_OPT)][static_cast<size_t>(Token::Return)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::PROC_DECLS_OPT)][static_cast<size_t>(Token::For)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::PROC_DECLS_LIST_OPT)][static_cast<size_t>(Token::Semicolon)] = SymbolSequence{Token::Semicolon, Rule::DECL, Rule::PROC_DECLS_LIST_OPT};
	table[static_cast<size_t>(Rule::PROC_DECLS_LIST_OPT)][static_cast<size_t>(Token::In)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::DECL)][static_cast<size_t>(Token::Var)] = SymbolSequence{Rule::VAR_DECL};
	table[static_cast<size_t>(Rule::DECL)][static_cast<size_t>(Token::Procedure)] = SymbolSequence{Rule::PROC_DECL};
	table[static_cast<size_t>(Rule::DECL)][static_cast<size_t>(Token::Struct)] = SymbolSequence{Rule::REC_DECL};
	table[static_cast<size_t>(Rule::VAR_DECL)][static_cast<size_t>(Token::Var)] = SymbolSequence{Token::Var, Token::Identifier, Rule::VAR_DECL_TYPE_OPT};
	table[static_cast<size_t>(Rule::VAR_DECL_TYPE_OPT)][static_cast<size_t>(Token::Colon)] = SymbolSequence{Token::Colon, Rule::TYPE, Rule::VAR_DECL_EXP_OPT};
	table[static_cast<size_t>(Rule::VAR_DECL_TYPE_OPT)][static_cast<size_t>(Token::Assign)] = SymbolSequence{Token::Assign, Rule::EXP};
	table[static_cast<size_t>(Rule::VAR_DECL_EXP_OPT)][static_cast<size_t>(Token::End)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::VAR_DECL_EXP_OPT)][static_cast<size_t>(Token::Semicolon)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::VAR_DECL_EXP_OPT)][static_cast<size_t>(Token::In)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::VAR_DECL_EXP_OPT)][static_cast<size_t>(Token::Assign)] = SymbolSequence{Token::Assign, Rule::EXP};
	table[static_cast<size_t>(Rule::PROC_DECL)][static_cast<size_t>(Token::Procedure)] = SymbolSequence{Token::Procedure, Token::Identifier, Token::L_Paren, Rule::PARAMFIELD_DECL_OPT, Token::R_Paren, Rule::PARAMFIELD_TYPE_OPT, Token::Begin, Rule::PROC_DECLS_OPT, Rule::STMT_LIST, Token::End};
	table[static_cast<size_t>(Rule::PARAMFIELD_TYPE_OPT)][static_cast<size_t>(Token::Begin)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::PARAMFIELD_TYPE_OPT)][static_cast<size_t>(Token::Colon)] = SymbolSequence{Token::Colon, Rule::TYPE};
	table[static_cast<size_t>(Rule::PARAMFIELD_DECL_OPT)][static_cast<size_t>(Token::Identifier)] = SymbolSequence{Rule::PARAMFIELD_DECL_LIST};
	table[static_cast<size_t>(Rule::PARAMFIELD_DECL_OPT)][static_cast<size_t>(Token::R_Paren)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::PARAMFIELD_DECL_LIST)][static_cast<size_t>(Token::Identifier)] = SymbolSequence{Token::Identifier, Token::Colon, Rule::TYPE, Rule::PARAMFIELD_DECL_LIST_OPT};
	table[static_cast<size_t>(Rule::PARAMFIELD_DECL_LIST_OPT)][static_cast<size_t>(Token::R_Paren)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::PARAMFIELD_DECL_LIST_OPT)][static_cast<size_t>(Token::Comma)] = SymbolSequence{Token::Comma, Rule::PARAMFIELD_DECL_LIST};
	table[static_cast<size_t>(Rule::REC_DECL)][static_cast<size_t>(Token::Struct)] = SymbolSequence{Token::Struct, Token::Identifier, Token::L_Bracket, Rule::PARAMFIELD_DECL_OPT_PRIME, Token::R_Bracket};
	table[static_cast<size_t>(Rule::PARAMFIELD_DECL_OPT_PRIME)][static_cast<size_t>(Token::Identifier)] = SymbolSequence{Rule::PARAMFIELD_DECL_LIST_PRIME};
	table[static_cast<size_t>(Rule::PARAMFIELD_DECL_OPT_PRIME)][static_cast<size_t>(Token::R_Bracket)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::PARAMFIELD_DECL_LIST_PRIME)][static_cast<size_t>(Token::Identifier)] = SymbolSequence{Token::Identifier, Token::Colon, Rule::TYPE, Rule::PARAMFIELD_DECL_LIST_OPT_PRIME};
	table[static_cast<size_t>(Rule::PARAMFIELD_DECL_LIST_OPT_PRIME)][static_cast<size_t>(Token::Semicolon)] = SymbolSequence{Token::Semicolon, Rule::PARAMFIELD_DECL_LIST_PRIME};
	table[static_cast<size_t>(Rule::PARAMFIELD_DECL_LIST_OPT_PRIME)][static_cast<size_t>(Token::R_Bracket)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::STMT_LIST)][static_cast<size_t>(Token::Identifier)] = SymbolSequence{Rule::STMT, Rule::STMT_TAIL};
	table[static_cast<size_t>(Rule::STMT_LIST)][static_cast<size_t>(Token::End)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::STMT_LIST)][static_cast<size_t>(Token::L_Paren)] = SymbolSequence{Rule::STMT, Rule::STMT_TAIL};
	table[static_cast<size_t>(Rule::STMT_LIST)][static_cast<size_t>(Token::New)] = SymbolSequence{Rule::STMT, Rule::STMT_TAIL};
	table[static_cast<size_t>(Rule::STMT_LIST)][static_cast<size_t>(Token::Ref)] = SymbolSequence{Rule::STMT, Rule::STMT_TAIL};
	table[static_cast<size_t>(Rule::STMT_LIST)][static_cast<size_t>(Token::Deref)] = SymbolSequence{Rule::STMT, Rule::STMT_TAIL};
	table[static_cast<size_t>(Rule::STMT_LIST)][static_cast<size_t>(Token::Float_L)] = SymbolSequence{Rule::STMT, Rule::STMT_TAIL};
	table[static_cast<size_t>(Rule::STMT_LIST)][static_cast<size_t>(Token::Int_L)] = SymbolSequence{Rule::STMT, Rule::STMT_TAIL};
	table[static_cast<size_t>(Rule::STMT_LIST)][static_cast<size_t>(Token::String_L)] = SymbolSequence{Rule::STMT, Rule::STMT_TAIL};
	table[static_cast<size_t>(Rule::STMT_LIST)][static_cast<size_t>(Token::Bool_L)] = SymbolSequence{Rule::STMT, Rule::STMT_TAIL};
	table[static_cast<size_t>(Rule::STMT_LIST)][static_cast<size_t>(Token::Null)] = SymbolSequence{Rule::STMT, Rule::STMT_TAIL};
	table[static_cast<size_t>(Rule::STMT_LIST)][static_cast<size_t>(Token::If)] = SymbolSequence{Rule::STMT, Rule::STMT_TAIL};
	table[static_cast<size_t>(Rule::STMT_LIST)][static_cast<size_t>(Token::Fi)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::STMT_LIST)][static_cast<size_t>(Token::Else)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::STMT_LIST)][static_cast<size_t>(Token::While)] = SymbolSequence{Rule::STMT, Rule::STMT_TAIL};
	table[static_cast<size_t>(Rule::STMT_LIST)][static_cast<size_t>(Token::Do)] = SymbolSequence{Rule::STMT, Rule::STMT_TAIL};
	table[static_cast<size_t>(Rule::STMT_LIST)][static_cast<size_t>(Token::Od)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::STMT_LIST)][static_cast<size_t>(Token::Return)] = SymbolSequence{Rule::STMT, Rule::STMT_TAIL};
	table[static_cast<size_t>(Rule::STMT_LIST)][static_cast<size_t>(Token::For)] = SymbolSequence{Rule::STMT, Rule::STMT_TAIL};
	table[static_cast<size_t>(Rule::STMT_LIST)][static_cast<size_t>(Token::Until)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::STMT_TAIL)][static_cast<size_t>(Token::End)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::STMT_TAIL)][static_cast<size_t>(Token::Semicolon)] = SymbolSequence{Token::Semicolon, Rule::STMT_LIST};
	table[static_cast<size_t>(Rule::STMT_TAIL)][static_cast<size_t>(Token::Fi)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::STMT_TAIL)][static_cast<size_t>(Token::Else)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::STMT_TAIL)][static_cast<size_t>(Token::Od)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::STMT_TAIL)][static_cast<size_t>(Token::Until)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::EXP)][static_cast<size_t>(Token::Identifier)] = SymbolSequence{Rule::OR_EXP};
	table[static_cast<size_t>(Rule::EXP)][static_cast<size_t>(Token::L_Paren)] = SymbolSequence{Rule::OR_EXP};
	table[static_cast<size_t>(Rule::EXP)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{Rule::OR_EXP};
	table[static_cast<size_t>(Rule::EXP)][static_cast<size_t>(Token::New)] = SymbolSequence{Rule::OR_EXP};
	table[static_cast<size_t>(Rule::EXP)][static_cast<size_t>(Token::Ref)] = SymbolSequence{Rule::OR_EXP};
	table[static_cast<size_t>(Rule::EXP)][static_cast<size_t>(Token::Deref)] = SymbolSequence{Rule::OR_EXP};
	table[static_cast<size_t>(Rule::EXP)][static_cast<size_t>(Token::Float_L)] = SymbolSequence{Rule::OR_EXP};
	table[static_cast<size_t>(Rule::EXP)][static_cast<size_t>(Token::Int_L)] = SymbolSequence{Rule::OR_EXP};
	table[static_cast<size_t>(Rule::EXP)][static_cast<size_t>(Token::String_L)] = SymbolSequence{Rule::OR_EXP};
	table[static_cast<size_t>(Rule::EXP)][static_cast<size_t>(Token::Bool_L)] = SymbolSequence{Rule::OR_EXP};
	table[static_cast<size_t>(Rule::EXP)][static_cast<size_t>(Token::Null)] = SymbolSequence{Rule::OR_EXP};
	table[static_cast<size_t>(Rule::OR_EXP)][static_cast<size_t>(Token::Identifier)] = SymbolSequence{Rule::AND_EXP, Rule::OR_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::OR_EXP)][static_cast<size_t>(Token::L_Paren)] = SymbolSequence{Rule::AND_EXP, Rule::OR_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::OR_EXP)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{Rule::AND_EXP, Rule::OR_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::OR_EXP)][static_cast<size_t>(Token::New)] = SymbolSequence{Rule::AND_EXP, Rule::OR_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::OR_EXP)][static_cast<size_t>(Token::Ref)] = SymbolSequence{Rule::AND_EXP, Rule::OR_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::OR_EXP)][static_cast<size_t>(Token::Deref)] = SymbolSequence{Rule::AND_EXP, Rule::OR_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::OR_EXP)][static_cast<size_t>(Token::Float_L)] = SymbolSequence{Rule::AND_EXP, Rule::OR_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::OR_EXP)][static_cast<size_t>(Token::Int_L)] = SymbolSequence{Rule::AND_EXP, Rule::OR_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::OR_EXP)][static_cast<size_t>(Token::String_L)] = SymbolSequence{Rule::AND_EXP, Rule::OR_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::OR_EXP)][static_cast<size_t>(Token::Bool_L)] = SymbolSequence{Rule::AND_EXP, Rule::OR_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::OR_EXP)][static_cast<size_t>(Token::Null)] = SymbolSequence{Rule::AND_EXP, Rule::OR_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::OR_EXP_LIST_TAIL)][static_cast<size_t>(Token::End)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::OR_EXP_LIST_TAIL)][static_cast<size_t>(Token::Semicolon)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::OR_EXP_LIST_TAIL)][static_cast<size_t>(Token::In)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::OR_EXP_LIST_TAIL)][static_cast<size_t>(Token::R_Paren)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::OR_EXP_LIST_TAIL)][static_cast<size_t>(Token::Comma)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::OR_EXP_LIST_TAIL)][static_cast<size_t>(Token::Or)] = SymbolSequence{Token::Or, Rule::AND_EXP, Rule::OR_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::OR_EXP_LIST_TAIL)][static_cast<size_t>(Token::Then)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::OR_EXP_LIST_TAIL)][static_cast<size_t>(Token::Fi)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::OR_EXP_LIST_TAIL)][static_cast<size_t>(Token::Else)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::OR_EXP_LIST_TAIL)][static_cast<size_t>(Token::Do)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::OR_EXP_LIST_TAIL)][static_cast<size_t>(Token::Od)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::OR_EXP_LIST_TAIL)][static_cast<size_t>(Token::To)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::OR_EXP_LIST_TAIL)][static_cast<size_t>(Token::Step)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::OR_EXP_LIST_TAIL)][static_cast<size_t>(Token::Until)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::AND_EXP)][static_cast<size_t>(Token::Identifier)] = SymbolSequence{Rule::REL_EXP, Rule::AND_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::AND_EXP)][static_cast<size_t>(Token::L_Paren)] = SymbolSequence{Rule::REL_EXP, Rule::AND_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::AND_EXP)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{Rule::REL_EXP, Rule::AND_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::AND_EXP)][static_cast<size_t>(Token::New)] = SymbolSequence{Rule::REL_EXP, Rule::AND_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::AND_EXP)][static_cast<size_t>(Token::Ref)] = SymbolSequence{Rule::REL_EXP, Rule::AND_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::AND_EXP)][static_cast<size_t>(Token::Deref)] = SymbolSequence{Rule::REL_EXP, Rule::AND_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::AND_EXP)][static_cast<size_t>(Token::Float_L)] = SymbolSequence{Rule::REL_EXP, Rule::AND_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::AND_EXP)][static_cast<size_t>(Token::Int_L)] = SymbolSequence{Rule::REL_EXP, Rule::AND_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::AND_EXP)][static_cast<size_t>(Token::String_L)] = SymbolSequence{Rule::REL_EXP, Rule::AND_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::AND_EXP)][static_cast<size_t>(Token::Bool_L)] = SymbolSequence{Rule::REL_EXP, Rule::AND_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::AND_EXP)][static_cast<size_t>(Token::Null)] = SymbolSequence{Rule::REL_EXP, Rule::AND_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::AND_EXP_LIST_TAIL)][static_cast<size_t>(Token::End)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::AND_EXP_LIST_TAIL)][static_cast<size_t>(Token::Semicolon)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::AND_EXP_LIST_TAIL)][static_cast<size_t>(Token::In)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::AND_EXP_LIST_TAIL)][static_cast<size_t>(Token::R_Paren)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::AND_EXP_LIST_TAIL)][static_cast<size_t>(Token::Comma)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::AND_EXP_LIST_TAIL)][static_cast<size_t>(Token::Or)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::AND_EXP_LIST_TAIL)][static_cast<size_t>(Token::And)] = SymbolSequence{Token::And, Rule::REL_EXP, Rule::AND_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::AND_EXP_LIST_TAIL)][static_cast<size_t>(Token::Then)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::AND_EXP_LIST_TAIL)][static_cast<size_t>(Token::Fi)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::AND_EXP_LIST_TAIL)][static_cast<size_t>(Token::Else)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::AND_EXP_LIST_TAIL)][static_cast<size_t>(Token::Do)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::AND_EXP_LIST_TAIL)][static_cast<size_t>(Token::Od)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::AND_EXP_LIST_TAIL)][static_cast<size_t>(Token::To)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::AND_EXP_LIST_TAIL)][static_cast<size_t>(Token::Step)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::AND_EXP_LIST_TAIL)][static_cast<size_t>(Token::Until)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::REL_EXP)][static_cast<size_t>(Token::Identifier)] = SymbolSequence{Rule::ADD_EXP, Rule::REL_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::REL_EXP)][static_cast<size_t>(Token::L_Paren)] = SymbolSequence{Rule::ADD_EXP, Rule::REL_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::REL_EXP)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{Rule::ADD_EXP, Rule::REL_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::REL_EXP)][static_cast<size_t>(Token::New)] = SymbolSequence{Rule::ADD_EXP, Rule::REL_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::REL_EXP)][static_cast<size_t>(Token::Ref)] = SymbolSequence{Rule::ADD_EXP, Rule::REL_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::REL_EXP)][static_cast<size_t>(Token::Deref)] = SymbolSequence{Rule::ADD_EXP, Rule::REL_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::REL_EXP)][static_cast<size_t>(Token::Float_L)] = SymbolSequence{Rule::ADD_EXP, Rule::REL_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::REL_EXP)][static_cast<size_t>(Token::Int_L)] = SymbolSequence{Rule::ADD_EXP, Rule::REL_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::REL_EXP)][static_cast<size_t>(Token::String_L)] = SymbolSequence{Rule::ADD_EXP, Rule::REL_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::REL_EXP)][static_cast<size_t>(Token::Bool_L)] = SymbolSequence{Rule::ADD_EXP, Rule::REL_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::REL_EXP)][static_cast<size_t>(Token::Null)] = SymbolSequence{Rule::ADD_EXP, Rule::REL_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::REL_EXP_LIST_TAIL)][static_cast<size_t>(Token::End)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::REL_EXP_LIST_TAIL)][static_cast<size_t>(Token::Semicolon)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::REL_EXP_LIST_TAIL)][static_cast<size_t>(Token::In)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::REL_EXP_LIST_TAIL)][static_cast<size_t>(Token::R_Paren)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::REL_EXP_LIST_TAIL)][static_cast<size_t>(Token::Comma)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::REL_EXP_LIST_TAIL)][static_cast<size_t>(Token::Or)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::REL_EXP_LIST_TAIL)][static_cast<size_t>(Token::And)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::REL_EXP_LIST_TAIL)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{Rule::REL_OP, Rule::ADD_EXP, Rule::REL_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::REL_EXP_LIST_TAIL)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{Rule::REL_OP, Rule::ADD_EXP, Rule::REL_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::REL_EXP_LIST_TAIL)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{Rule::REL_OP, Rule::ADD_EXP, Rule::REL_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::REL_EXP_LIST_TAIL)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{Rule::REL_OP, Rule::ADD_EXP, Rule::REL_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::REL_EXP_LIST_TAIL)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{Rule::REL_OP, Rule::ADD_EXP, Rule::REL_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::REL_EXP_LIST_TAIL)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{Rule::REL_OP, Rule::ADD_EXP, Rule::REL_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::REL_EXP_LIST_TAIL)][static_cast<size_t>(Token::Then)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::REL_EXP_LIST_TAIL)][static_cast<size_t>(Token::Fi)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::REL_EXP_LIST_TAIL)][static_cast<size_t>(Token::Else)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::REL_EXP_LIST_TAIL)][static_cast<size_t>(Token::Do)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::REL_EXP_LIST_TAIL)][static_cast<size_t>(Token::Od)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::REL_EXP_LIST_TAIL)][static_cast<size_t>(Token::To)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::REL_EXP_LIST_TAIL)][static_cast<size_t>(Token::Step)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::REL_EXP_LIST_TAIL)][static_cast<size_t>(Token::Until)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ADD_EXP)][static_cast<size_t>(Token::Identifier)] = SymbolSequence{Rule::MUL_EXP, Rule::ADD_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::ADD_EXP)][static_cast<size_t>(Token::L_Paren)] = SymbolSequence{Rule::MUL_EXP, Rule::ADD_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::ADD_EXP)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{Rule::MUL_EXP, Rule::ADD_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::ADD_EXP)][static_cast<size_t>(Token::New)] = SymbolSequence{Rule::MUL_EXP, Rule::ADD_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::ADD_EXP)][static_cast<size_t>(Token::Ref)] = SymbolSequence{Rule::MUL_EXP, Rule::ADD_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::ADD_EXP)][static_cast<size_t>(Token::Deref)] = SymbolSequence{Rule::MUL_EXP, Rule::ADD_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::ADD_EXP)][static_cast<size_t>(Token::Float_L)] = SymbolSequence{Rule::MUL_EXP, Rule::ADD_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::ADD_EXP)][static_cast<size_t>(Token::Int_L)] = SymbolSequence{Rule::MUL_EXP, Rule::ADD_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::ADD_EXP)][static_cast<size_t>(Token::String_L)] = SymbolSequence{Rule::MUL_EXP, Rule::ADD_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::ADD_EXP)][static_cast<size_t>(Token::Bool_L)] = SymbolSequence{Rule::MUL_EXP, Rule::ADD_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::ADD_EXP)][static_cast<size_t>(Token::Null)] = SymbolSequence{Rule::MUL_EXP, Rule::ADD_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::ADD_EXP_LIST_TAIL)][static_cast<size_t>(Token::End)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ADD_EXP_LIST_TAIL)][static_cast<size_t>(Token::Semicolon)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ADD_EXP_LIST_TAIL)][static_cast<size_t>(Token::In)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ADD_EXP_LIST_TAIL)][static_cast<size_t>(Token::R_Paren)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ADD_EXP_LIST_TAIL)][static_cast<size_t>(Token::Comma)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ADD_EXP_LIST_TAIL)][static_cast<size_t>(Token::Or)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ADD_EXP_LIST_TAIL)][static_cast<size_t>(Token::And)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ADD_EXP_LIST_TAIL)][static_cast<size_t>(Token::Plus)] = SymbolSequence{Rule::SUMINUS, Rule::MUL_EXP, Rule::ADD_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::ADD_EXP_LIST_TAIL)][static_cast<size_t>(Token::Minus)] = SymbolSequence{Rule::SUMINUS, Rule::MUL_EXP, Rule::ADD_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::ADD_EXP_LIST_TAIL)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ADD_EXP_LIST_TAIL)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ADD_EXP_LIST_TAIL)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ADD_EXP_LIST_TAIL)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ADD_EXP_LIST_TAIL)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ADD_EXP_LIST_TAIL)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ADD_EXP_LIST_TAIL)][static_cast<size_t>(Token::Then)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ADD_EXP_LIST_TAIL)][static_cast<size_t>(Token::Fi)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ADD_EXP_LIST_TAIL)][static_cast<size_t>(Token::Else)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ADD_EXP_LIST_TAIL)][static_cast<size_t>(Token::Do)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ADD_EXP_LIST_TAIL)][static_cast<size_t>(Token::Od)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ADD_EXP_LIST_TAIL)][static_cast<size_t>(Token::To)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ADD_EXP_LIST_TAIL)][static_cast<size_t>(Token::Step)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ADD_EXP_LIST_TAIL)][static_cast<size_t>(Token::Until)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MUL_EXP)][static_cast<size_t>(Token::Identifier)] = SymbolSequence{Rule::POW_EXP, Rule::MUL_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::MUL_EXP)][static_cast<size_t>(Token::L_Paren)] = SymbolSequence{Rule::POW_EXP, Rule::MUL_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::MUL_EXP)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{Rule::POW_EXP, Rule::MUL_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::MUL_EXP)][static_cast<size_t>(Token::New)] = SymbolSequence{Rule::POW_EXP, Rule::MUL_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::MUL_EXP)][static_cast<size_t>(Token::Ref)] = SymbolSequence{Rule::POW_EXP, Rule::MUL_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::MUL_EXP)][static_cast<size_t>(Token::Deref)] = SymbolSequence{Rule::POW_EXP, Rule::MUL_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::MUL_EXP)][static_cast<size_t>(Token::Float_L)] = SymbolSequence{Rule::POW_EXP, Rule::MUL_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::MUL_EXP)][static_cast<size_t>(Token::Int_L)] = SymbolSequence{Rule::POW_EXP, Rule::MUL_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::MUL_EXP)][static_cast<size_t>(Token::String_L)] = SymbolSequence{Rule::POW_EXP, Rule::MUL_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::MUL_EXP)][static_cast<size_t>(Token::Bool_L)] = SymbolSequence{Rule::POW_EXP, Rule::MUL_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::MUL_EXP)][static_cast<size_t>(Token::Null)] = SymbolSequence{Rule::POW_EXP, Rule::MUL_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::MUL_EXP_LIST_TAIL)][static_cast<size_t>(Token::End)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MUL_EXP_LIST_TAIL)][static_cast<size_t>(Token::Semicolon)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MUL_EXP_LIST_TAIL)][static_cast<size_t>(Token::In)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MUL_EXP_LIST_TAIL)][static_cast<size_t>(Token::R_Paren)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MUL_EXP_LIST_TAIL)][static_cast<size_t>(Token::Comma)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MUL_EXP_LIST_TAIL)][static_cast<size_t>(Token::Or)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MUL_EXP_LIST_TAIL)][static_cast<size_t>(Token::And)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MUL_EXP_LIST_TAIL)][static_cast<size_t>(Token::Plus)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MUL_EXP_LIST_TAIL)][static_cast<size_t>(Token::Minus)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MUL_EXP_LIST_TAIL)][static_cast<size_t>(Token::Times)] = SymbolSequence{Rule::MULDIV, Rule::POW_EXP, Rule::MUL_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::MUL_EXP_LIST_TAIL)][static_cast<size_t>(Token::Divides)] = SymbolSequence{Rule::MULDIV, Rule::POW_EXP, Rule::MUL_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::MUL_EXP_LIST_TAIL)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MUL_EXP_LIST_TAIL)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MUL_EXP_LIST_TAIL)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MUL_EXP_LIST_TAIL)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MUL_EXP_LIST_TAIL)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MUL_EXP_LIST_TAIL)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MUL_EXP_LIST_TAIL)][static_cast<size_t>(Token::Then)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MUL_EXP_LIST_TAIL)][static_cast<size_t>(Token::Fi)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MUL_EXP_LIST_TAIL)][static_cast<size_t>(Token::Else)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MUL_EXP_LIST_TAIL)][static_cast<size_t>(Token::Do)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MUL_EXP_LIST_TAIL)][static_cast<size_t>(Token::Od)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MUL_EXP_LIST_TAIL)][static_cast<size_t>(Token::To)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MUL_EXP_LIST_TAIL)][static_cast<size_t>(Token::Step)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MUL_EXP_LIST_TAIL)][static_cast<size_t>(Token::Until)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::POW_EXP)][static_cast<size_t>(Token::Identifier)] = SymbolSequence{Rule::UNARY_EXP, Rule::POW_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::POW_EXP)][static_cast<size_t>(Token::L_Paren)] = SymbolSequence{Rule::UNARY_EXP, Rule::POW_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::POW_EXP)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{Rule::UNARY_EXP, Rule::POW_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::POW_EXP)][static_cast<size_t>(Token::New)] = SymbolSequence{Rule::UNARY_EXP, Rule::POW_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::POW_EXP)][static_cast<size_t>(Token::Ref)] = SymbolSequence{Rule::UNARY_EXP, Rule::POW_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::POW_EXP)][static_cast<size_t>(Token::Deref)] = SymbolSequence{Rule::UNARY_EXP, Rule::POW_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::POW_EXP)][static_cast<size_t>(Token::Float_L)] = SymbolSequence{Rule::UNARY_EXP, Rule::POW_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::POW_EXP)][static_cast<size_t>(Token::Int_L)] = SymbolSequence{Rule::UNARY_EXP, Rule::POW_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::POW_EXP)][static_cast<size_t>(Token::String_L)] = SymbolSequence{Rule::UNARY_EXP, Rule::POW_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::POW_EXP)][static_cast<size_t>(Token::Bool_L)] = SymbolSequence{Rule::UNARY_EXP, Rule::POW_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::POW_EXP)][static_cast<size_t>(Token::Null)] = SymbolSequence{Rule::UNARY_EXP, Rule::POW_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::POW_EXP_LIST_TAIL)][static_cast<size_t>(Token::End)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::POW_EXP_LIST_TAIL)][static_cast<size_t>(Token::Semicolon)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::POW_EXP_LIST_TAIL)][static_cast<size_t>(Token::In)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::POW_EXP_LIST_TAIL)][static_cast<size_t>(Token::R_Paren)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::POW_EXP_LIST_TAIL)][static_cast<size_t>(Token::Comma)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::POW_EXP_LIST_TAIL)][static_cast<size_t>(Token::Or)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::POW_EXP_LIST_TAIL)][static_cast<size_t>(Token::And)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::POW_EXP_LIST_TAIL)][static_cast<size_t>(Token::Pow)] = SymbolSequence{Token::Pow, Rule::UNARY_EXP, Rule::POW_EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::POW_EXP_LIST_TAIL)][static_cast<size_t>(Token::Plus)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::POW_EXP_LIST_TAIL)][static_cast<size_t>(Token::Minus)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::POW_EXP_LIST_TAIL)][static_cast<size_t>(Token::Times)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::POW_EXP_LIST_TAIL)][static_cast<size_t>(Token::Divides)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::POW_EXP_LIST_TAIL)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::POW_EXP_LIST_TAIL)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::POW_EXP_LIST_TAIL)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::POW_EXP_LIST_TAIL)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::POW_EXP_LIST_TAIL)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::POW_EXP_LIST_TAIL)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::POW_EXP_LIST_TAIL)][static_cast<size_t>(Token::Then)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::POW_EXP_LIST_TAIL)][static_cast<size_t>(Token::Fi)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::POW_EXP_LIST_TAIL)][static_cast<size_t>(Token::Else)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::POW_EXP_LIST_TAIL)][static_cast<size_t>(Token::Do)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::POW_EXP_LIST_TAIL)][static_cast<size_t>(Token::Od)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::POW_EXP_LIST_TAIL)][static_cast<size_t>(Token::To)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::POW_EXP_LIST_TAIL)][static_cast<size_t>(Token::Step)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::POW_EXP_LIST_TAIL)][static_cast<size_t>(Token::Until)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::UNARY_EXP)][static_cast<size_t>(Token::Identifier)] = SymbolSequence{Rule::PRIMARY};
	table[static_cast<size_t>(Rule::UNARY_EXP)][static_cast<size_t>(Token::L_Paren)] = SymbolSequence{Rule::PRIMARY};
	table[static_cast<size_t>(Rule::UNARY_EXP)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{Token::Comparison, Rule::UNARY_EXP};
	table[static_cast<size_t>(Rule::UNARY_EXP)][static_cast<size_t>(Token::New)] = SymbolSequence{Rule::PRIMARY};
	table[static_cast<size_t>(Rule::UNARY_EXP)][static_cast<size_t>(Token::Ref)] = SymbolSequence{Rule::PRIMARY};
	table[static_cast<size_t>(Rule::UNARY_EXP)][static_cast<size_t>(Token::Deref)] = SymbolSequence{Rule::PRIMARY};
	table[static_cast<size_t>(Rule::UNARY_EXP)][static_cast<size_t>(Token::Float_L)] = SymbolSequence{Rule::PRIMARY};
	table[static_cast<size_t>(Rule::UNARY_EXP)][static_cast<size_t>(Token::Int_L)] = SymbolSequence{Rule::PRIMARY};
	table[static_cast<size_t>(Rule::UNARY_EXP)][static_cast<size_t>(Token::String_L)] = SymbolSequence{Rule::PRIMARY};
	table[static_cast<size_t>(Rule::UNARY_EXP)][static_cast<size_t>(Token::Bool_L)] = SymbolSequence{Rule::PRIMARY};
	table[static_cast<size_t>(Rule::UNARY_EXP)][static_cast<size_t>(Token::Null)] = SymbolSequence{Rule::PRIMARY};
	table[static_cast<size_t>(Rule::MAYBE_ACCESS_TAIL)][static_cast<size_t>(Token::End)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MAYBE_ACCESS_TAIL)][static_cast<size_t>(Token::Semicolon)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MAYBE_ACCESS_TAIL)][static_cast<size_t>(Token::Assign)] = SymbolSequence{Rule::ACCESS_TAIL, Token::Assign, Rule::EXP};
	table[static_cast<size_t>(Rule::MAYBE_ACCESS_TAIL)][static_cast<size_t>(Token::Dot)] = SymbolSequence{Rule::ACCESS_TAIL, Token::Assign, Rule::EXP};
	table[static_cast<size_t>(Rule::MAYBE_ACCESS_TAIL)][static_cast<size_t>(Token::Fi)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MAYBE_ACCESS_TAIL)][static_cast<size_t>(Token::Else)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MAYBE_ACCESS_TAIL)][static_cast<size_t>(Token::Od)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MAYBE_ACCESS_TAIL)][static_cast<size_t>(Token::Until)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::CALL_OR_ASSIGNMENT)][static_cast<size_t>(Token::Identifier)] = SymbolSequence{Token::Identifier, Rule::MAYBE_CALL, Rule::MAYBE_ACCESS_TAIL};
	table[static_cast<size_t>(Rule::CALL_OR_ASSIGNMENT)][static_cast<size_t>(Token::L_Paren)] = SymbolSequence{Token::L_Paren, Rule::EXP, Token::R_Paren, Rule::ACCESS_TAIL, Token::Assign, Rule::EXP};
	table[static_cast<size_t>(Rule::CALL_OR_ASSIGNMENT)][static_cast<size_t>(Token::New)] = SymbolSequence{Token::New, Token::Identifier, Rule::ACCESS_TAIL, Token::Assign, Rule::EXP};
	table[static_cast<size_t>(Rule::CALL_OR_ASSIGNMENT)][static_cast<size_t>(Token::Ref)] = SymbolSequence{Rule::REF_VAR, Rule::ACCESS_TAIL, Token::Assign, Rule::EXP};
	table[static_cast<size_t>(Rule::CALL_OR_ASSIGNMENT)][static_cast<size_t>(Token::Deref)] = SymbolSequence{Rule::DEREF_VAR, Rule::ACCESS_TAIL, Token::Assign, Rule::EXP};
	table[static_cast<size_t>(Rule::CALL_OR_ASSIGNMENT)][static_cast<size_t>(Token::Float_L)] = SymbolSequence{Rule::LITERAL, Rule::ACCESS_TAIL, Token::Assign, Rule::EXP};
	table[static_cast<size_t>(Rule::CALL_OR_ASSIGNMENT)][static_cast<size_t>(Token::Int_L)] = SymbolSequence{Rule::LITERAL, Rule::ACCESS_TAIL, Token::Assign, Rule::EXP};
	table[static_cast<size_t>(Rule::CALL_OR_ASSIGNMENT)][static_cast<size_t>(Token::String_L)] = SymbolSequence{Rule::LITERAL, Rule::ACCESS_TAIL, Token::Assign, Rule::EXP};
	table[static_cast<size_t>(Rule::CALL_OR_ASSIGNMENT)][static_cast<size_t>(Token::Bool_L)] = SymbolSequence{Rule::LITERAL, Rule::ACCESS_TAIL, Token::Assign, Rule::EXP};
	table[static_cast<size_t>(Rule::CALL_OR_ASSIGNMENT)][static_cast<size_t>(Token::Null)] = SymbolSequence{Rule::LITERAL, Rule::ACCESS_TAIL, Token::Assign, Rule::EXP};
	table[static_cast<size_t>(Rule::PRIMARY)][static_cast<size_t>(Token::Identifier)] = SymbolSequence{Token::Identifier, Rule::MAYBE_CALL, Rule::ACCESS_TAIL};
	table[static_cast<size_t>(Rule::PRIMARY)][static_cast<size_t>(Token::L_Paren)] = SymbolSequence{Token::L_Paren, Rule::EXP, Token::R_Paren, Rule::ACCESS_TAIL};
	table[static_cast<size_t>(Rule::PRIMARY)][static_cast<size_t>(Token::New)] = SymbolSequence{Token::New, Token::Identifier, Rule::ACCESS_TAIL};
	table[static_cast<size_t>(Rule::PRIMARY)][static_cast<size_t>(Token::Ref)] = SymbolSequence{Rule::REF_VAR, Rule::ACCESS_TAIL};
	table[static_cast<size_t>(Rule::PRIMARY)][static_cast<size_t>(Token::Deref)] = SymbolSequence{Rule::DEREF_VAR, Rule::ACCESS_TAIL};
	table[static_cast<size_t>(Rule::PRIMARY)][static_cast<size_t>(Token::Float_L)] = SymbolSequence{Rule::LITERAL, Rule::ACCESS_TAIL};
	table[static_cast<size_t>(Rule::PRIMARY)][static_cast<size_t>(Token::Int_L)] = SymbolSequence{Rule::LITERAL, Rule::ACCESS_TAIL};
	table[static_cast<size_t>(Rule::PRIMARY)][static_cast<size_t>(Token::String_L)] = SymbolSequence{Rule::LITERAL, Rule::ACCESS_TAIL};
	table[static_cast<size_t>(Rule::PRIMARY)][static_cast<size_t>(Token::Bool_L)] = SymbolSequence{Rule::LITERAL, Rule::ACCESS_TAIL};
	table[static_cast<size_t>(Rule::PRIMARY)][static_cast<size_t>(Token::Null)] = SymbolSequence{Rule::LITERAL, Rule::ACCESS_TAIL};
	table[static_cast<size_t>(Rule::ACCESS_TAIL)][static_cast<size_t>(Token::End)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ACCESS_TAIL)][static_cast<size_t>(Token::Semicolon)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ACCESS_TAIL)][static_cast<size_t>(Token::In)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ACCESS_TAIL)][static_cast<size_t>(Token::Assign)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ACCESS_TAIL)][static_cast<size_t>(Token::R_Paren)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ACCESS_TAIL)][static_cast<size_t>(Token::Comma)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ACCESS_TAIL)][static_cast<size_t>(Token::Or)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ACCESS_TAIL)][static_cast<size_t>(Token::And)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ACCESS_TAIL)][static_cast<size_t>(Token::Pow)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ACCESS_TAIL)][static_cast<size_t>(Token::Dot)] = SymbolSequence{Token::Dot, Token::Identifier, Rule::ACCESS_TAIL};
	table[static_cast<size_t>(Rule::ACCESS_TAIL)][static_cast<size_t>(Token::Plus)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ACCESS_TAIL)][static_cast<size_t>(Token::Minus)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ACCESS_TAIL)][static_cast<size_t>(Token::Times)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ACCESS_TAIL)][static_cast<size_t>(Token::Divides)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ACCESS_TAIL)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ACCESS_TAIL)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ACCESS_TAIL)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ACCESS_TAIL)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ACCESS_TAIL)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ACCESS_TAIL)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ACCESS_TAIL)][static_cast<size_t>(Token::Then)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ACCESS_TAIL)][static_cast<size_t>(Token::Fi)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ACCESS_TAIL)][static_cast<size_t>(Token::Else)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ACCESS_TAIL)][static_cast<size_t>(Token::Do)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ACCESS_TAIL)][static_cast<size_t>(Token::Od)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ACCESS_TAIL)][static_cast<size_t>(Token::To)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ACCESS_TAIL)][static_cast<size_t>(Token::Step)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::ACCESS_TAIL)][static_cast<size_t>(Token::Until)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::REF_VAR)][static_cast<size_t>(Token::Ref)] = SymbolSequence{Token::Ref, Token::L_Paren, Rule::PRIMARY, Token::R_Paren};
	table[static_cast<size_t>(Rule::DEREF_VAR)][static_cast<size_t>(Token::Deref)] = SymbolSequence{Token::Deref, Token::L_Paren, Rule::PRIMARY, Token::R_Paren};
	table[static_cast<size_t>(Rule::MAYBE_CALL)][static_cast<size_t>(Token::End)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MAYBE_CALL)][static_cast<size_t>(Token::Semicolon)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MAYBE_CALL)][static_cast<size_t>(Token::In)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MAYBE_CALL)][static_cast<size_t>(Token::Assign)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MAYBE_CALL)][static_cast<size_t>(Token::L_Paren)] = SymbolSequence{Token::L_Paren, Rule::EXP_LIST, Token::R_Paren};
	table[static_cast<size_t>(Rule::MAYBE_CALL)][static_cast<size_t>(Token::R_Paren)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MAYBE_CALL)][static_cast<size_t>(Token::Comma)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MAYBE_CALL)][static_cast<size_t>(Token::Or)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MAYBE_CALL)][static_cast<size_t>(Token::And)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MAYBE_CALL)][static_cast<size_t>(Token::Pow)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MAYBE_CALL)][static_cast<size_t>(Token::Dot)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MAYBE_CALL)][static_cast<size_t>(Token::Plus)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MAYBE_CALL)][static_cast<size_t>(Token::Minus)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MAYBE_CALL)][static_cast<size_t>(Token::Times)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MAYBE_CALL)][static_cast<size_t>(Token::Divides)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MAYBE_CALL)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MAYBE_CALL)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MAYBE_CALL)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MAYBE_CALL)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MAYBE_CALL)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MAYBE_CALL)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MAYBE_CALL)][static_cast<size_t>(Token::Then)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MAYBE_CALL)][static_cast<size_t>(Token::Fi)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MAYBE_CALL)][static_cast<size_t>(Token::Else)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MAYBE_CALL)][static_cast<size_t>(Token::Do)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MAYBE_CALL)][static_cast<size_t>(Token::Od)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MAYBE_CALL)][static_cast<size_t>(Token::To)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MAYBE_CALL)][static_cast<size_t>(Token::Step)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::MAYBE_CALL)][static_cast<size_t>(Token::Until)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::EXP_LIST)][static_cast<size_t>(Token::Identifier)] = SymbolSequence{Rule::EXP, Rule::EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::EXP_LIST)][static_cast<size_t>(Token::L_Paren)] = SymbolSequence{Rule::EXP, Rule::EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::EXP_LIST)][static_cast<size_t>(Token::R_Paren)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::EXP_LIST)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{Rule::EXP, Rule::EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::EXP_LIST)][static_cast<size_t>(Token::New)] = SymbolSequence{Rule::EXP, Rule::EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::EXP_LIST)][static_cast<size_t>(Token::Ref)] = SymbolSequence{Rule::EXP, Rule::EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::EXP_LIST)][static_cast<size_t>(Token::Deref)] = SymbolSequence{Rule::EXP, Rule::EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::EXP_LIST)][static_cast<size_t>(Token::Float_L)] = SymbolSequence{Rule::EXP, Rule::EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::EXP_LIST)][static_cast<size_t>(Token::Int_L)] = SymbolSequence{Rule::EXP, Rule::EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::EXP_LIST)][static_cast<size_t>(Token::String_L)] = SymbolSequence{Rule::EXP, Rule::EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::EXP_LIST)][static_cast<size_t>(Token::Bool_L)] = SymbolSequence{Rule::EXP, Rule::EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::EXP_LIST)][static_cast<size_t>(Token::Null)] = SymbolSequence{Rule::EXP, Rule::EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::EXP_LIST_TAIL)][static_cast<size_t>(Token::R_Paren)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::EXP_LIST_TAIL)][static_cast<size_t>(Token::Comma)] = SymbolSequence{Token::Comma, Rule::EXP, Rule::EXP_LIST_TAIL};
	table[static_cast<size_t>(Rule::SUMINUS)][static_cast<size_t>(Token::Plus)] = SymbolSequence{Token::Plus};
	table[static_cast<size_t>(Rule::SUMINUS)][static_cast<size_t>(Token::Minus)] = SymbolSequence{Token::Minus};
	table[static_cast<size_t>(Rule::MULDIV)][static_cast<size_t>(Token::Times)] = SymbolSequence{Token::Times};
	table[static_cast<size_t>(Rule::MULDIV)][static_cast<size_t>(Token::Divides)] = SymbolSequence{Token::Divides};
	table[static_cast<size_t>(Rule::REL_OP)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{Token::Comparison};
	table[static_cast<size_t>(Rule::REL_OP)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{Token::Comparison};
	table[static_cast<size_t>(Rule::REL_OP)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{Token::Comparison};
	table[static_cast<size_t>(Rule::REL_OP)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{Token::Comparison};
	table[static_cast<size_t>(Rule::REL_OP)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{Token::Comparison};
	table[static_cast<size_t>(Rule::REL_OP)][static_cast<size_t>(Token::Comparison)] = SymbolSequence{Token::Comparison};
	table[static_cast<size_t>(Rule::LITERAL)][static_cast<size_t>(Token::Float_L)] = SymbolSequence{Token::Float_L};
	table[static_cast<size_t>(Rule::LITERAL)][static_cast<size_t>(Token::Int_L)] = SymbolSequence{Token::Int_L};
	table[static_cast<size_t>(Rule::LITERAL)][static_cast<size_t>(Token::String_L)] = SymbolSequence{Token::String_L};
	table[static_cast<size_t>(Rule::LITERAL)][static_cast<size_t>(Token::Bool_L)] = SymbolSequence{Token::Bool_L};
	table[static_cast<size_t>(Rule::LITERAL)][static_cast<size_t>(Token::Null)] = SymbolSequence{Token::Null};
	table[static_cast<size_t>(Rule::STMT)][static_cast<size_t>(Token::Identifier)] = SymbolSequence{Rule::CALL_OR_ASSIGNMENT};
	table[static_cast<size_t>(Rule::STMT)][static_cast<size_t>(Token::L_Paren)] = SymbolSequence{Rule::CALL_OR_ASSIGNMENT};
	table[static_cast<size_t>(Rule::STMT)][static_cast<size_t>(Token::New)] = SymbolSequence{Rule::CALL_OR_ASSIGNMENT};
	table[static_cast<size_t>(Rule::STMT)][static_cast<size_t>(Token::Ref)] = SymbolSequence{Rule::CALL_OR_ASSIGNMENT};
	table[static_cast<size_t>(Rule::STMT)][static_cast<size_t>(Token::Deref)] = SymbolSequence{Rule::CALL_OR_ASSIGNMENT};
	table[static_cast<size_t>(Rule::STMT)][static_cast<size_t>(Token::Float_L)] = SymbolSequence{Rule::CALL_OR_ASSIGNMENT};
	table[static_cast<size_t>(Rule::STMT)][static_cast<size_t>(Token::Int_L)] = SymbolSequence{Rule::CALL_OR_ASSIGNMENT};
	table[static_cast<size_t>(Rule::STMT)][static_cast<size_t>(Token::String_L)] = SymbolSequence{Rule::CALL_OR_ASSIGNMENT};
	table[static_cast<size_t>(Rule::STMT)][static_cast<size_t>(Token::Bool_L)] = SymbolSequence{Rule::CALL_OR_ASSIGNMENT};
	table[static_cast<size_t>(Rule::STMT)][static_cast<size_t>(Token::Null)] = SymbolSequence{Rule::CALL_OR_ASSIGNMENT};
	table[static_cast<size_t>(Rule::STMT)][static_cast<size_t>(Token::If)] = SymbolSequence{Rule::IF_STMT};
	table[static_cast<size_t>(Rule::STMT)][static_cast<size_t>(Token::While)] = SymbolSequence{Rule::WHILE_STMT};
	table[static_cast<size_t>(Rule::STMT)][static_cast<size_t>(Token::Do)] = SymbolSequence{Rule::DO_UNTIL_STMT};
	table[static_cast<size_t>(Rule::STMT)][static_cast<size_t>(Token::Return)] = SymbolSequence{Rule::RETURN_STMT};
	table[static_cast<size_t>(Rule::STMT)][static_cast<size_t>(Token::For)] = SymbolSequence{Rule::FOR_STMT};
	table[static_cast<size_t>(Rule::IF_STMT)][static_cast<size_t>(Token::If)] = SymbolSequence{Token::If, Rule::EXP, Token::Then, Rule::STMT_LIST, Rule::IF_ELSE, Token::Fi};
	table[static_cast<size_t>(Rule::IF_ELSE)][static_cast<size_t>(Token::Fi)] = SymbolSequence{};
	table[static_cast<size_t>(Rule::IF_ELSE)][static_cast<size_t>(Token::Else)] = SymbolSequence{Token::Else, Rule::STMT_LIST};
	table[static_cast<size_t>(Rule::WHILE_STMT)][static_cast<size_t>(Token::While)] = SymbolSequence{Token::While, Rule::EXP, Token::Do, Rule::STMT_LIST, Token::Od};
	table[static_cast<size_t>(Rule::RETURN_STMT)][static_cast<size_t>(Token::Return)] = SymbolSequence{Token::Return, Token::L_Paren, Rule::EXP, Token::R_Paren};
	table[static_cast<size_t>(Rule::TYPE)][static_cast<size_t>(Token::Identifier)] = SymbolSequence{Token::Identifier};
	table[static_cast<size_t>(Rule::TYPE)][static_cast<size_t>(Token::Ref)] = SymbolSequence{Token::Ref, Token::L_Paren, Rule::TYPE, Token::R_Paren};
	table[static_cast<size_t>(Rule::TYPE)][static_cast<size_t>(Token::Float)] = SymbolSequence{Token::Float};
	table[static_cast<size_t>(Rule::TYPE)][static_cast<size_t>(Token::Int)] = SymbolSequence{Token::Int};
	table[static_cast<size_t>(Rule::TYPE)][static_cast<size_t>(Token::String)] = SymbolSequence{Token::String};
	table[static_cast<size_t>(Rule::TYPE)][static_cast<size_t>(Token::Bool)] = SymbolSequence{Token::Bool};
	table[static_cast<size_t>(Rule::FOR_STMT)][static_cast<size_t>(Token::For)] = SymbolSequence{Token::For, Rule::PRIMARY, Token::Comparison, Rule::EXP, Token::To, Rule::EXP, Token::Step, Rule::EXP, Token::Do, Rule::STMT_LIST, Token::Od};
	table[static_cast<size_t>(Rule::DO_UNTIL_STMT)][static_cast<size_t>(Token::Do)] = SymbolSequence{Token::Do, Rule::STMT_LIST, Token::Until, Rule::EXP, Token::Od};
    }

    return table;
}
