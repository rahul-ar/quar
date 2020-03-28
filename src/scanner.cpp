xx#include "../include/scanner.hpp"

#include <iostream>

namespace Quar {

char Scanner::advance() {
	current++;
	return source.at(current - 1);
}

char Scanner::peek() const {
	if (isAtEnd()) return '\0';
	return source.at(current);
}

char Scanner::peekNext() const{
	if (is_at_end() || current + 1 == source.length())
		return '\0';
	return source.at(current + 1);
}

bool Scanner::match(char expected) {
	if (isAtEnd()) return false;
	if (source.at(current) != expected) return false;
	current++;
	return true;
}

Token Scanner::error_token(std::string_view message) const {
	return Token(TokenType::Error, message, line);
}

TokenType Scanner::check_keyword(size_t begin, size_t length, std::string_view rest, TokenType type) const {
	if ((current - start == begin + length) && (source.substr(start + begin, length) == rest))
		return type;
	return TokenType::Identifier;
}

TokenType Scanner::identifierType() const {
	switch (source.at(start))
	{
		case 'a': return check_keyword(1, 2, "nd", TokenType::And);
		case 'c': return check_keyword(1, 4, "lass", TokenType::Class);
		case 'e': return check_keyword(1, 3, "lse", TokenType::Else);
		case 'f':
			if (current - start > 1)
			{
				switch (source.at(start + 1))
				{
					case 'a': return check_keyword(2, 3, "lse", TokenType::False);
					case 'o': return check_keyword(2, 1, "r", TokenType::For);
					case 'u': return check_keyword(2, 1, "n", TokenType::Fun);
				}
			}
			break;
		case 'i': return check_keyword(1, 1, "f", TokenType::If);
		case 'n': return check_keyword(1, 2, "il", TokenType::Nil);
		case 'o': return check_keyword(1, 1, "r", TokenType::Or);
		case 'p': return check_keyword(1, 4, "rint", TokenType::Print);
		case 'r': return check_keyword(1, 5, "eturn", TokenType::Return);
		case 's': return check_keyword(1, 4, "uper", TokenType::Super);
		case 't':
			if (current - start > 1)
			{
				switch (source.at(start + 1))
				{
					case 'h': return check_keyword(2, 2, "is", TokenType::This);
					case 'r': return check_keyword(2, 2, "ue", TokenType::True);
				}
			}
			break;
		case 'v': return check_keyword(1, 2, "ar", TokenType::Var);
		case 'w': return check_keyword(1, 4, "hile", TokenType::While);
		default:
			break;
	}
	return TokenType::Identifier;
}

Token Scanner::scan_token() {
	//skip_whitespace();

	start = current;
	if (is_at_end()) return make_token(TokenType::Eof);

	auto c = advance();
	if (std::isalpha(c)) 
		return identifier();
	if (std::isdigit(c)) 
		return number();
	switch (c) {
		case '(': return makeToken(TokenType::LeftParent);
		case ')': return makeToken(TokenType::RightParent);
		case '{': return makeToken(TokenType::LeftBrace);
		case '}': return makeToken(TokenType::RightBrace);
		case ';': return makeToken(TokenType::Semicolon);
		case ',': return makeToken(TokenType::Comma);
		case '.': return makeToken(TokenType::Dot);
		case '-': return makeToken(TokenType::Minus);
		case '+': return makeToken(TokenType::Plus);
		case '/': return makeToken(TokenType::Slash);
		case '*': return makeToken(TokenType::Star);
		case '!':
			return makeToken(match('=') ? TokenType::BangEqual : TokenType::Bang);
		case '=':
			return makeToken(match('=') ? TokenType::EqualEqual : TokenType::Equal);
		case '<':
			return makeToken(match('=') ? TokenType::LessEqual : TokenType::Less);
		case '>':
			return makeToken(match('=') ? TokenType::GreaterEqual : TokenType::Greater);
		case '"': return string();
	}
	return error_token("Unexpected character.");
}

Token Scanner::string()
{
	while (peek() != '"' && !isAtEnd())
	{
		if (peek() == '\n') line++;
		advance();
	}

	if (isAtEnd())
		return error_token("Unterminated string.");

	advance();
	return make_token(TokenType::String);
}

Token Scanner::number()
{
	while (std::isdigit(peek()))
		advance();

	if (peek() == '.' && std::isdigit(peek_next()))
	{
		advance();
		while (std::isdigit(peek()))
			advance();
	}

	return makeToken(TokenType::Number);
}

Token Scanner::identifier()
{
	while (std::isalnum(peek()))
		advance();
	return makeToken(identifierType());
}


