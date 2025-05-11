#ifndef PARSER_UTILS_HPP
#define PARSER_UTILS_HPP
#include "lexer.hpp"
#include "parse_table.hpp"

void log_expected_another_token_error(Token received) {
    std::cerr << "[SYNTAX ERROR]\nExpected token `"
		  << received
		  << "` but got `"
		  << yytext
		  << "` at ("
		  << yylineno << "," << yycolumn
		  << ").\n";
}

inline const std::string get_expected_from(Rule rule, Token unexpected, const ParseTable& table) {
	std::ostringstream oss;
	oss << "Expected one of:\n";

	bool found = false;
	for (size_t i = 0; i < static_cast<size_t>(Token::COUNT); ++i) {
		Token t = static_cast<Token>(i);

		if (t == unexpected) continue;

		if (table[static_cast<size_t>(rule)][i].has_value()) {
			found = true;
			oss << "  - " << t << "\n";
		}
	}

	if (!found)
		oss << "  (no valid tokens found)\n";

	return oss.str();
}

void log_unexpected_token_error(Rule rule, Token unexpected, const ParseTable &table) {
    std::cerr << "[SYNTAX ERROR]\nUnexpected token `"
        << yytext
        << "` at ("
        << yylineno << "," << yycolumn
        << ").\n"
        << get_expected_from(rule, unexpected, table)
        << "\n";
}

void log_unexpected_token_error(Rule rule, Token unexpected) {
    ParseTable p = build_parse_table();
    log_unexpected_token_error(rule, unexpected, p);
}

#endif