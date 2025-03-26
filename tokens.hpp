#pragma once

#include <string>
#include <variant>
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

enum ComparisonType {
	Leq,
	Geq,
	Greater,
	Less,
	Eq,
	Neq
};

using ComparisonData = ComparisonType;
using IntLiteralData = int;
using FloatLiteralData = float;
using StringLiteralData = std::string;
using BoolLiteralData = bool;

using TokenData = std::variant<
	std::monostate,
	ComparisonData,
	IntLiteralData,
	FloatLiteralData,
	StringLiteralData,
	BoolLiteralData
>;

// use std::variant
class Token {
public:
	Token(TokenType type, std::string lexeme, size_t line, size_t column)
		: type(type),
		  lexeme(lexeme),
		  line(line),
		  column(column),
		  data(std::monostate())
	{}

	TokenType type;
	TokenData data;
	std::string lexeme;
	size_t line;
	size_t column;

	// set token data carrier
	// one for each possible type
	void set_token_data(int value) {
		this->data = IntLiteralData(value);
	}
	
	void set_token_data(float value) {
		this->data = FloatLiteralData(value);
	}

	void set_token_data(bool value) {
		this->data = BoolLiteralData(value);
	}

	void set_token_data(std::string str) {
		this->data = StringLiteralData(str); 
	}

	void set_token_data(ComparisonType op) {
		this->data = ComparisonData(op);
	}


	std::string to_string() {
		return this->lexeme;
	}
};
