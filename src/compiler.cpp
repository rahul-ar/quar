#include "../include/compiler.hpp"

#include <string_view>
#include <cstdlib>
#include <iostream>

 
 namespace quar {

    const ParseRule& getRule(TokenType type) {
	    return Compiler::rules[static_cast<size_t>(type)];
    }

    void error(Parser& parser, std::string_view message) {
	    errorAt(parser, parser.previous, message);
    }

    void errorAt(Parser& parser, const Token& token, std::string_view message) {
        if (parser.panicMode) 
            return;
	    parser.panicMode = true;
	    std::cerr << "[line " << token.line << "] Error";
        if(token.type == TokenType::TOKEN_EOF)
            std::cerr << "at end";
        else if(token.type == TokenType::TOKEN_ERROR) {
        }
        else
            std::cerr << " at " << token.start;
        std::cerr << ":" << message << std::endl;
        parser.hadError = true;
    }

    void errorAtCurrent(Parser& parser, std::string_view message) {
        errorAt(parser, parser.current, message);
    }

    void Parser::advance() {
        previous = current;
        while(true) {
            current = scanner.scanToken();
            if (current.type != TokenType::TOKEN_ERROR) 
                break;  
            errorAtCurrent(*this, current.start);         
        } 
    }

    void Parser::consume(TokenType type, std::string_view message) {
	    if (current.type == type) {
		    advance();
		    return;
	    }
	    errorAtCurrent(*this, message);
    }

    bool Parser::match(TokenType type) {
        if (!(current.type == type)) return false;
        advance();
        return true;
    }
 
    Compiler::Compiler(Memory* memory) : memory(memory) {
        
    }

    void Compiler::emitByte(uint8_t byte) {
        memory->pushCode(byte, parser.previous.line);
    }

    void Compiler::emitByte(OpCode code) {
        memory->pushCode(code, parser.previous.line);
    }

    void Compiler::emitBytes(OpCode code, uint8_t byte) {
        //std::cout<<"in";
        emitByte(code);
        emitByte(byte);
    }


    bool Compiler::compile(std::string_view source) {
        parser = Parser(source);
        parser.advance();
        while (!parser.match(TokenType::TOKEN_EOF))
		    declaration();
        parser.consume(TokenType::TOKEN_EOF, "Expect end of Expression");
        endCompiler();
        return !parser.hadError;
        //return 1;
    }
        

    void Compiler::emitReturn() {
        emitByte(OpCode::OP_RETURN);
    }

    uint8_t Compiler::makeConstant(Data data) {
        auto val = memory->pushData(data);
        if(val > UINT8_MAX) {
            error(parser, "Too many constants in one chunk.");
            return 0;
        }
        return static_cast<uint8_t>(val);
    }

    void Compiler::emitConstant(Data data) {
        //std::cout << "inC";
        emitBytes(OpCode::OP_CONSTANT, makeConstant(data));
    }

    void Compiler::number(bool can_assign) {
        //std::cout << "number";
        auto data = std::strtod(parser.previous.start.data(), nullptr);
	    emitConstant(data);
    }

    void Compiler::grouping(bool can_assign) {                                     
        expression();                                              
        parser.consume(TokenType::TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
    }

    void Compiler::unary(bool can_assign) {
	    auto op = parser.previous.type;
        parsePrecedence(Precedence::PREC_UNARY);
	    switch (op) {
		    case (TokenType::TOKEN_MINUS) : 
                emitByte(OpCode::OP_NEGATE); 
                break;
		    default:
			    break;
	    }
    }   

    void Compiler::expression() {
        parsePrecedence(Precedence::PREC_ASSIGNMENT);
    }

    void Compiler::declaration() {
        if (parser.match(TokenType::TOKEN_VAR)) {

            //std::cout << "var";
		    varDeclaration();
        }
	    else {
		    expression();
	        parser.consume(TokenType::TOKEN_SEMICOLON, "Expect ';' after expression.");
	        emitByte(OpCode::OP_POP);
        }
    }

    void Compiler::endCompiler() {
        emitReturn();                   
        if (!parser.hadError) {                    
            disassembleChunk(memory, "code");
        }   
    }

    void Compiler::binary(bool can_assign) {
	    auto op = parser.previous.type;
	    const auto& rule = getRule(op);
	    parsePrecedence(static_cast<Precedence>(static_cast<uint8_t>(rule.precedence) + 1));
        switch (op) {
		    case TokenType::TOKEN_PLUS: emitByte(OpCode::OP_ADD); break;
		    case TokenType::TOKEN_MINUS: emitByte(OpCode::OP_SUBTRACT); break;
		    case TokenType::TOKEN_STAR: emitByte(OpCode::OP_MULTIPLY); break;
		    case TokenType::TOKEN_SLASH: emitByte(OpCode::OP_DIVIDE); break;
		    default:
			    break;
	    }
    }

    void Compiler::namedVariable(std::string_view name, bool canAssign) {
        auto arg = makeConstant((std::string) name);
        if (canAssign && parser.match(TokenType::TOKEN_EQUAL)) {
            expression();
            emitBytes(OpCode::OP_SET_GLOBAL, (uint8_t)arg);
        } 
        else {
            emitBytes(OpCode::OP_GET_GLOBAL, (uint8_t)arg);
        }
    }

    void Compiler::variable(bool canAssign) {
        namedVariable(std::string(parser.previous.start), canAssign);
    }

    void Compiler::defineVariable(uint8_t global) {
        emitBytes(OpCode::OP_DEF_GLOBAL, static_cast<uint8_t>(global));
    }

    void Compiler::varDeclaration() {
	    auto global = parseVariable("Expect variable name.");
	    if (parser.match(TokenType::TOKEN_EQUAL))
		    expression();
	    parser.consume(TokenType::TOKEN_SEMICOLON, "Expect ';' after variable declaration.");
	    defineVariable(global);
    }

    uint8_t Compiler::identifierConstant(Data name) {
	    return makeConstant(name);
    }

    uint8_t Compiler::parseVariable(std::string_view error) {
	    parser.consume(TokenType::TOKEN_IDENTIFIER, error);
        return identifierConstant(std::string(parser.previous.start));
    }

    
    void Compiler::parsePrecedence(Precedence precedence) {
	    parser.advance();
        
	    auto prefixRule = getRule(parser.previous.type).prefix;
	    if (prefixRule == nullptr) {
		    error(parser, "Expect expression.");
		    return;
	    }

	    auto can_assign = precedence <= Precedence::PREC_ASSIGNMENT;
	    (this->*prefixRule)(can_assign);

	    while (precedence <= getRule(parser.current.type).precedence) {
		    parser.advance();
		    auto infix_rule = getRule(parser.previous.type).infix;
		    (this->*infix_rule)(can_assign);
	    }
    }

}                                                 

    