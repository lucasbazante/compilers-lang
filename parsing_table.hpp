#include <iostream>
#include <variant>
#include <vector>
#include <optional>

#include "tokens.hpp"

// An enum for each rule we have.
// Its integer conversion will index our table.
enum class Rule {
    // Declarations
    PROGRAM = 0,
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

inline ParseTable build_parse_table() {
    const size_t TOKENS_COUNT = static_cast<size_t>(Token::COUNT);
    const size_t RULES_COUNT = static_cast<size_t>(Rule::COUNT);

    ParseTable table(
	RULES_COUNT,
	std::vector<std::optional<SymbolSequence>>(TOKENS_COUNT)
    );

    return table;
}
