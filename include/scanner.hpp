#pragma once

#include <string>

namespace quar {
	enum class TokenType : uint8_t {
	// Single-character tokens.                         
  		TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN,                
  		TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,                
  		TOKEN_COMMA, TOKEN_DOT, TOKEN_MINUS, TOKEN_PLUS,    
  		TOKEN_SEMICOLON, TOKEN_SLASH, TOKEN_STAR,

  // One or two character tokens.                     
  		TOKEN_BANG, TOKEN_BANG_EQUAL,                       
 	 	TOKEN_EQUAL, TOKEN_EQUAL_EQUAL,                     
  		TOKEN_GREATER, TOKEN_GREATER_EQUAL,                 
  		TOKEN_LESS, TOKEN_LESS_EQUAL,                       

  // Literals.                                        
  		TOKEN_IDENTIFIER, TOKEN_STRING, TOKEN_NUMBER,       

  // Keywords.                                        
  		TOKEN_AND, TOKEN_CLASS, TOKEN_ELSE, TOKEN_FALSE,    
  		TOKEN_FOR, TOKEN_FUN, TOKEN_IF, TOKEN_NIL, TOKEN_OR,
  		TOKEN_PRINT, TOKEN_RETURN, TOKEN_SUPER, TOKEN_THIS, 
  		TOKEN_TRUE, TOKEN_VAR, TOKEN_WHILE,                 

  		TOKEN_ERROR,                                        
  		TOKEN_EOF
	};

	struct Token {
		TokenType type;
		std::string start;
		size_t line = 1;

		Token(TokenType type, std::string start, size_t line) {
			this->type = type;
			this->start = start;
			this->line = line;
		}
		Token() : type(TokenType::TOKEN_EOF) {

		}
	};

	class Scanner {
	private :
		void skipWhitespace();
		char advance();
		bool isAtEnd()const;
		bool match(char);
		char peekNext()const;
		char peek()const;
		TokenType identifierType()const;
		TokenType checkKeyword(size_t, size_t, std::string, TokenType)const;
		Token identifier();
		Token number();
		Token string();
		Token errorToken(std::string)const;
		Token makeToken(TokenType)const;
	public:
		Scanner();
		Scanner(std::string&);
		size_t start;
		size_t current;
		size_t line;
		std::string source;
		Token scanToken();
	};
}

	

