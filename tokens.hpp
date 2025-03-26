#pragma once

#include <string>
enum class TokenType {
	// simple types
	Int,
	Float,
	Bool,
	String,

	// declarations (program, var, procedures and types)
	Program,
	Begin,
	In,
	End,
	Var,
	Procedure,
	Struct,
	New,

	// references
	Ref,
	Deref,
	
	// arithmetic operators
	Plus,
	Minus,
	Times,
	Divides,
	Pow,
	Modulo,

	// logical operators && relations
	And,
	Or,
	Comparison,

	// Literals
	IntL,
	FloatL,
	BoolL,
	StringL,
	Null,

	// control-flow
	If,
	Then,
	Else,
	Fi,
	While,
	Do,
	Until,
	Od,
	For,
	To,
	Step,
	Return,

	// special characters
	Semicolon,
	Colon,
	Assign,
	LParen,
	RParen,
	LBracket,
	RBracket,
	Dot,

	// most generic, the identifier
	Identifier
};

// use std::variant
class Token {
public:
	Token(TokenType type, std::string lexeme, size_t line, size_t column)
		: type(type), lexeme(lexeme), line(line), column(column) {}

	TokenType type;
	std::string lexeme;
	size_t line;
	size_t column;

	std::string toString() {
		return this->lexeme;
	}
};
