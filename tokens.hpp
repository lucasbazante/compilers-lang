#pragma once

#include <string>
#include <variant>
#include <iostream>

enum class Token {
	// simples
	Int = 0,
	Float,
	Bool,
	String,

	// declarations (program, var, procedures ands)
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
	Int_L,
	Float_L,
	Bool_L,
	String_L,
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
	L_Paren,
	R_Paren,
	L_Bracket,
	R_Bracket,
	Dot,

	// most generic, the identifier
	Identifier,

	// end of file
	TEOF
};

enum cmp_type {
	Leq,
	Geq,
	Greater,
	Less,
	Eq,
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
		case cmp_type::Eq: os << "="; break;
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
		case Token::Identifier: os << "name"; break;
		case Token::TEOF:	os << "EOF"; break;
	}

	return os;
}
