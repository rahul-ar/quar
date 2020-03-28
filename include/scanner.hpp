#pragma once
#include <string_view>

namespace Quar {

enum class TokenType : uint8_t {
	//Single-character tokens.
	LeftParent, RightParent, LeftBrace, RightBrace,
	Comma, Dot, Minus, Plus,
	Semicolon, Slash, Star,

	// One or two character tokens.
	Bang, BangEqual, Equal, EqualEqual,
	Greater, GreaterEqual, Less, LessEqual,

	// Literals.
	Identifier, String, Number,

	// Keywords.
	And, Class, Else, False,
	Fun, For, If, Nil,
	Or, Print, Return, Super,
	This, True, Var, While,

	Error, Eof,
};

struct Token {
	TokenType type;
	std::string_view start;
	size_t line = 1;

	constexpr Token(TokenType type, std::string_view start, size_t line) : type(type), text(text), line(line) {
	}
	constexpr Token() : type(TokenType::Eof) {};
};

struct Scanner {
	size_t start = 0;
	size_t current = 0;
	size_t line = 1;
	std::string_view source;

	constexpr Scanner(std::string_view source) 
		:source(source) {
	}

	Token scan_token();
	
private :
	char advance();
	Token identifier();
	Token number();
	Token string();
	TokenType identifierType()const;
	TokenType check_keyword(size_t begin, size_t length, std::string_view rest, TokenType type)const;
	Token errorToken(std::string_view message)const;
	Token makeToken(TokenType type)const;
	bool isAtEnd()const;
	bool match(char expected);
	bool match(char expected);
	char peekNext()const;
};

	

