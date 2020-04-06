#include "../include/scanner.hpp"

#include <iostream>

namespace quar {
	Scanner::Scanner() {

	}
		
	Scanner::Scanner(std::string& source) 
		: source(source),
		start(0),
		current(0),
		line(1) {

	}

	Token Scanner::makeToken(TokenType type) const {
		size_t length = current - start;
		return Token(type, source.substr(start, length), line);
	}

	void Scanner::skipWhitespace() {
		while(peek() == ' ' or peek() == '\n') {
			if(peek() == '\n') {
				line++;
			}
				advance();
		}
	}

	bool Scanner::isAtEnd() const {
		return current == source.length();
	}

	char Scanner::advance() {
		current++;
		return source.at(current - 1);
	}

	char Scanner::peek() const {
		if (isAtEnd()) return '\0';
		return source.at(current);
	}

	char Scanner::peekNext() const{
		if (isAtEnd() || current + 1 == source.length())
			return '\0';
		return source.at(current + 1);
	}

	bool Scanner::match(char expected) {
		if (isAtEnd()) return false;
		if (source.at(current) != expected) return false;
		current++;
		return true;
	}

	Token Scanner::errorToken(std::string message) const {
		return Token(TokenType::TOKEN_ERROR, message, line);
	}

	TokenType Scanner::checkKeyword(size_t begin, size_t length, std::string rest, TokenType type) const {
		if ((current - start == begin + length) && (source.substr(start + begin, length) == rest))
			return type;
		return TokenType::TOKEN_IDENTIFIER;
	}

	TokenType Scanner::identifierType() const {
		switch (source.at(start)) {
			case 'a': return checkKeyword(1, 2, "nd", TokenType::TOKEN_AND);
			case 'e': return checkKeyword(1, 3, "lse", TokenType::TOKEN_ELSE);
			case 'f':
				if (current - start > 1) {
					switch (source.at(start + 1)) {
						case 'a': return checkKeyword(2, 3, "lse", TokenType::TOKEN_FALSE);
						case 'o': return checkKeyword(2, 1, "r", TokenType::TOKEN_FOR);
					}
				}
				break;
			case 'i': return checkKeyword(1, 1, "f", TokenType::TOKEN_IF);
			case 'n': return checkKeyword(1, 2, "il", TokenType::TOKEN_NIL);
			case 'o': return checkKeyword(1, 1, "r", TokenType::TOKEN_OR);
			case 'p': return checkKeyword(1, 4, "rint", TokenType::TOKEN_PRINT);
			case 'r': return checkKeyword(1, 5, "eturn", TokenType::TOKEN_RETURN);
			case 't':
				if (current - start > 1) {
					switch (source.at(start + 1)) {
						case 'h': return checkKeyword(2, 2, "is", TokenType::TOKEN_THIS);
						case 'r': return checkKeyword(2, 2, "ue", TokenType::TOKEN_TRUE);
					}
				}
				break;
			case 'v': return checkKeyword(1, 2, "ar", TokenType::TOKEN_VAR);
			case 'w': return checkKeyword(1, 4, "hile", TokenType::TOKEN_WHILE);
			default:
				break;
		}
		return TokenType::TOKEN_IDENTIFIER;
	}

	Token Scanner::scanToken() {
		skipWhitespace();
		start = current;
		if (isAtEnd()) return makeToken(TokenType::TOKEN_EOF);

		auto c = advance();
		if (std::isalpha(c)) 
			return identifier();
		if (std::isdigit(c))
			return number();
		switch (c) {
			case '(': return makeToken(TokenType::TOKEN_LEFT_PAREN);
			case ')': return makeToken(TokenType::TOKEN_RIGHT_PAREN);
			case '{': return makeToken(TokenType::TOKEN_LEFT_BRACE);
			case '}': return makeToken(TokenType::TOKEN_RIGHT_BRACE);
			case ';': return makeToken(TokenType::TOKEN_SEMICOLON);
			case ',': return makeToken(TokenType::TOKEN_COMMA);
			case '.': return makeToken(TokenType::TOKEN_DOT);
			case '-': return makeToken(TokenType::TOKEN_MINUS);
			case '+': return makeToken(TokenType::TOKEN_PLUS);
			case '/': return makeToken(TokenType::TOKEN_SLASH);
			case '*': return makeToken(TokenType::TOKEN_STAR);
			case '!':
				return makeToken(match('=') ? TokenType::TOKEN_BANG_EQUAL : TokenType::TOKEN_BANG);
			case '=':
				return makeToken(match('=') ? TokenType::TOKEN_EQUAL_EQUAL : TokenType::TOKEN_EQUAL);
			case '<':
				return makeToken(match('=') ? TokenType::TOKEN_LESS_EQUAL : TokenType::TOKEN_LESS);
			case '>':
				return makeToken(match('=') ? TokenType::TOKEN_GREATER_EQUAL : TokenType::TOKEN_GREATER);
			case '"': return string();
		}
		return errorToken("Unexpected character.");
	}

	
	
	Token Scanner::number() {
		while (std::isdigit(peek()))
			advance();
		if (peek() == '.' && std::isdigit(peekNext())) {
			advance();
			while (std::isdigit(peek()))
				advance();
		}
		return makeToken(TokenType::TOKEN_NUMBER);
	}

	Token Scanner::identifier() {
		while (std::isalnum(peek()))
			advance();
		return makeToken(identifierType());
	}

	Token Scanner::string() {
    	while (peek() != '"' && !isAtEnd()) {
        	if (peek() == '\n') line++;
        	advance();
    	}
    	if (isAtEnd()) return errorToken("Unterminated string.");
    	advance();
    	return makeToken(TokenType::TOKEN_STRING);
	}
}



