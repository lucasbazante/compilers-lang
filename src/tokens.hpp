#pragma once

#include <string>
#include <variant>
#include <iostream>

enum class Token {
	// simples
	Int = 0,
	Float = 1,
	Bool = 2,
	String = 3,

	// declarations (program, var, procedures ands)
	Program = 4,
	Begin = 5,
	In = 6,
	End = 7,
	Var = 8,
	Procedure = 9,
	Struct = 10,
	New = 11,

	// references
	Ref = 12,
	Deref = 13,

	// Literals
	Int_L = 14,
	Float_L = 15,
	Bool_L = 16,
	String_L = 17,
	Null = 18,

	// control-flow
	If = 19,
	Then = 20,
	Else = 21,
	Fi = 22,
	While = 23,
	Do = 24,
	Until = 25,
	Od = 26,
	For = 27,
	To = 28,
	Step = 29,
	Return = 30,
	// tem q colocar o '='

	// special characters
	Semicolon = 31,
	Colon = 32,
	Assign = 33,
	L_Paren = 34,
	R_Paren = 35,
	L_Bracket = 36,
	R_Bracket = 37,
	Comma = 38,
	Dot = 39,

	// most generic, the identifier
	Identifier = 40,

	// arithmetic operators
	Plus = 41,
	Minus = 42,
	Times = 43,
	Divides = 44,
	Pow = 45,
	Modulo = 46, // TODO: ei! Não tem isso não...
	// TODO: tem q colocar o unaryminus
	// TODO: tem q colocar o not

	// logical operators && relations
	And = 47,
	Or = 48,
	Comparison = 49,
	Equals = 50,

	// end of file
	TEOF = 51,

	COUNT
};

enum cmp_type {
	Leq,
	Geq,
	Greater,
	Less,
	Neq
};

using TokenData = std::variant<
	std::monostate, // nothing
	int,		// integers
	float,		// floats
	std::string,	// strings and identifiers
	bool,		// booleans
	cmp_type	// comparison
>;

// overload the << operator for cmp_type
inline std::ostream& operator<<(std::ostream& os, const cmp_type& cmp) {
	switch (cmp) {
		case cmp_type::Leq: os << "<="; break;
		case cmp_type::Geq: os << ">="; break;
		case cmp_type::Greater: os << ">"; break;
		case cmp_type::Less: os << "<"; break;
		case cmp_type::Neq: os << "<>"; break;
	}

	return os;
}

// overload the << operator for Token
inline std::ostream& operator<<(std::ostream& os, const Token& token) {
	switch (token) {
		case Token::Int:	os << "int"; break;
		case Token::Float:	os << "float"; break;
		case Token::Bool:	os << "bool"; break;
		case Token::String:	os << "string"; break;
		case Token::Program:	os << "program"; break;
		case Token::Begin:	os << "begin"; break;
		case Token::In:		os << "in"; break;
		case Token::End:	os << "end"; break;
		case Token::Var:	os << "var"; break;
		case Token::Procedure:  os << "procedure"; break;
		case Token::Struct:	os << "struct"; break;
		case Token::New:	os << "new"; break;
		case Token::Ref:	os << "ref"; break;
		case Token::Deref:	os << "deref"; break;
		case Token::Plus:	os << "+"; break;
		case Token::Minus:	os << "-"; break;
		case Token::Times:	os << "*"; break;
		case Token::Divides:	os << "divides"; break;
		case Token::Pow:	os << "^"; break;
		case Token::Modulo:	os << "%"; break;
		case Token::And:	os << "&&"; break;
		case Token::Or:		os << "||"; break;
		case Token::Comparison: os << "comparison"; break;
		case Token::Equals: os << "="; break;
		case Token::Int_L:	os << "int_literal"; break;
		case Token::Float_L:	os << "float literal"; break;
		case Token::Bool_L:	os << "bool_literal"; break;
		case Token::String_L:	os << "string_literal"; break;
		case Token::Null:	os << "null"; break;
		case Token::If:		os << "if"; break;
		case Token::Then:	os << "then"; break;
		case Token::Else:	os << "else"; break;
		case Token::Fi:		os << "fi"; break;
		case Token::While:	os << "while"; break;
		case Token::Do:		os << "do"; break;
		case Token::Until:	os << "until"; break;
		case Token::Od:		os << "od"; break;
		case Token::For:	os << "for"; break;
		case Token::To:		os << "to"; break;
		case Token::Step:	os << "step"; break;
		case Token::Return:	os << "return"; break;
		case Token::Semicolon:	os << ";"; break;
		case Token::Colon:	os << ":"; break;
		case Token::Assign:	os << ":="; break;
		case Token::L_Paren:	os << "("; break;
		case Token::R_Paren:	os << ")"; break;
		case Token::L_Bracket:	os << "{"; break;
		case Token::R_Bracket:	os << "}"; break;
		case Token::Dot:	os << "."; break;
		case Token::Comma:	os << ","; break;
		case Token::Identifier: os << "name"; break;
		case Token::TEOF:	os << "EOF"; break;
	}

	return os;
}
