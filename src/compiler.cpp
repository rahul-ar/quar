#include "../include/compiler.hpp"

#include <string>
#include <string_view>
#include <cstdlib>
#include <iostream>

namespace quar {
    const ParseRule& getRule(TokenType type) {
	    return Compiler::rules[static_cast<size_t>(type)];
    }

    void error(Parser& parser, std::string message) {
	    errorAt(parser, parser.previous, message);
    }

    void errorAt(Parser& parser, const Token& token, std::string message) {
        if (parser.panicMode) 
            return;
	    parser.panicMode = true;
	    std::cerr << "[line " << token.line << "] Error";
        if(token.type == TokenType::TOKEN_EOF)
            std::cerr << " at end";
        else if(token.type == TokenType::TOKEN_ERROR) {
        }
        else
            std::cerr << " at " << token.start;
        std::cerr << ":" << message << std::endl;
        parser.hadError = true;
    }

    void errorAtCurrent(Parser& parser, std::string message) {
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

    void Parser::consume(TokenType type, std::string message) {
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

    bool Compiler::compile(std::string source) {
        parser = Parser(source);
        parser.advance();
        while (!parser.match(TokenType::TOKEN_EOF)) {
            declaration();
        }
        parser.consume(TokenType::TOKEN_EOF, "Expect end of Expression");
        endCompiler();
        return !parser.hadError;
    }

    void Compiler::emitByte(uint8_t byte) {
        memory->pushCode(byte, parser.previous.line);
    }

    void Compiler::emitByte(OpCode code) {
        memory->pushCode(code, parser.previous.line);
    }

    void Compiler::emitBytes(OpCode code, uint8_t byte) {
        emitByte(code);
        emitByte(byte);
    }

    void Compiler::emitReturn() {
        emitByte(OpCode::OP_RETURN);
    }

    size_t Compiler::makeConstant(Data data) {
        auto val = memory->pushData(data);
        if(val > UINT8_MAX) {
            error(parser, "Too many constants in one chunk.");
            return 0;
        }
        return val;
    }

    void Compiler::emitConstant(Data data) {
        emitBytes(OpCode::OP_CONSTANT, makeConstant(data));
    }

    void Compiler::number(bool can_assign) {
	    emitConstant(Data(stod(parser.previous.start)));
    }

    void Compiler::string(bool canAssign) {
        std::string_view str = parser.previous.start;
        str.remove_prefix(1);
        str.remove_suffix(1);
        emitConstant(Data(std::string(str)));
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
	    }
    }   

    void Compiler::expression() {
        parsePrecedence(Precedence::PREC_ASSIGNMENT);
    }

    void Compiler::declaration() {
        if (parser.match(TokenType::TOKEN_VAR)) {
		    varDeclaration();
        }
	    else {
		    statement();
        }
    }

    void Compiler::statement() {
        if(parser.match(TokenType::TOKEN_PRINT))
            printStatement();
        else if (parser.match(TokenType::TOKEN_LEFT_BRACE))
		    block();
        else if (parser.match(TokenType::TOKEN_IF))
		    ifStatement();
        else
		    expressionStatement();
    }

    size_t Compiler::emitJump(OpCode code) {
        emitByte(code);
		emitByte(static_cast<uint8_t>(0xff));
		emitByte(static_cast<uint8_t>(0xff));
		return memory->getCodes().size() - 2;
    }

    void Compiler::patchJump(size_t offset) {
        auto jump = memory->getCodes().size() - 2 - offset;
        if (jump > UINT16_MAX)
		    error(parser, "Too much code to jump over.");
        memory->patchMemory(offset, static_cast<uint8_t>((jump >> 8) & 0xff));
	    memory->patchMemory(offset + 1, static_cast<uint8_t>(jump & 0xff)); 
    }

    void Compiler::ifStatement() {
        parser.consume(TokenType::TOKEN_LEFT_PAREN, "Expect '(' after 'if'.");
	    expression();
	    parser.consume(TokenType::TOKEN_RIGHT_PAREN, "Expect ')' after condition.");
        auto thenJump = emitJump(OpCode::OP_JUMP_IF_FALSE);
	    emitByte(OpCode::OP_POP);
	    statement();
        auto elseJump = emitJump(OpCode::OP_JUMP);
        patchJump(thenJump);
        if (parser.match(TokenType::TOKEN_ELSE))
		    statement();
	    patchJump(elseJump);
	    emitByte(OpCode::OP_POP);
    }

    void Compiler::block() {
        while (!(parser.current.type == TokenType::TOKEN_RIGHT_BRACE) && !(parser.current.type == TokenType::TOKEN_EOF))
		    declaration();
	    parser.consume(TokenType::TOKEN_RIGHT_BRACE, "Expect '}' after block.");
    }

    void Compiler::expressionStatement() {
        expression();
	    parser.consume(TokenType::TOKEN_SEMICOLON, "Expect ';' after expression.");
	    emitByte(OpCode::OP_POP);
    }

    void Compiler::printStatement() {
        expression();
        parser.consume(TokenType::TOKEN_SEMICOLON, "Expect ';' after value.");
	    emitByte(OpCode::OP_PRINT);
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
            case TokenType::TOKEN_BANG_EQUAL: emitByte(OpCode::OP_EQUAL); emitByte(OpCode::OP_NOT); break;
		    case TokenType::TOKEN_EQUAL_EQUAL: emitByte(OpCode::OP_EQUAL); break;
		    case TokenType::TOKEN_GREATER: std::cout << "DDD"; emitByte(OpCode::OP_GREATER); break;
		    case TokenType::TOKEN_GREATER_EQUAL: emitByte(OpCode::OP_GREATER), emitByte(OpCode::OP_NOT); break;
		    case TokenType::TOKEN_LESS: emitByte(OpCode::OP_LESSER); break;
		    case TokenType::TOKEN_LESS_EQUAL: emitByte(OpCode::OP_GREATER), emitByte(OpCode::OP_NOT); break;
		    case TokenType::TOKEN_PLUS: emitByte(OpCode::OP_ADD); break;
		    case TokenType::TOKEN_MINUS: emitByte(OpCode::OP_SUBTRACT); break;
		    case TokenType::TOKEN_STAR: emitByte(OpCode::OP_MULTIPLY); break;
		    case TokenType::TOKEN_SLASH: emitByte(OpCode::OP_DIVIDE); break;
            
	    }
    }

    void Compiler::namedVariable(std::string name, bool canAssign) {
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
	    if (parser.match(TokenType::TOKEN_EQUAL)) {
		    expression();
        }
	    parser.consume(TokenType::TOKEN_SEMICOLON, "Expect ';' after variable declaration.");
	    defineVariable(global);
    }

    uint8_t Compiler::identifierConstant(Data name) {
	    return makeConstant(name);
    }

    uint8_t Compiler::parseVariable(std::string error) {
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
        int i = 0;
	    while (precedence <= getRule(parser.current.type).precedence) {
		    parser.advance();
		    auto infix_rule = getRule(parser.previous.type).infix;
		    (this->*infix_rule)(can_assign);
            if (can_assign && parser.match(TokenType::TOKEN_EQUAL)) {
			    error(parser, "Invalid assignment target.");
			    expression();
		    }
	    }
    }
}                                                 
