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

    void Compiler::emitByte(uint8_t byte) {
        //std::cout << "in";
        chunk.lines.push_back(parser->previous.line);
        chunk.codes.push_back(static_cast<uint8_t>(byte));
    }

    void Compiler::emitBytes(uint8_t byte1, uint8_t byte2) {
        //std::cout<<"in";
        emitByte(byte1);
        emitByte(byte2);
    }


    bool Compiler::compile(std::string_view source) {
        this->parser = new Parser(source);
        parser->advance();
        expression();
        parser->consume(TokenType::TOKEN_EOF, "Expect end of Expression");
        endCompiler();
        return !parser->hadError;
        //return 1;
    }
        

    void Compiler::emitReturn() {
        emitByte(static_cast<uint8_t>(OpCode::OP_RETURN));
    }

    uint8_t Compiler::makeConstant(Data data) {
        auto val = chunk.pushData(data);
        if(val > UINT8_MAX) {
            error(*parser, "Too many constants in one chunk.");
            return 0;
        }
        return static_cast<uint8_t>(val);
    }

    void Compiler::emitConstant(Data data) {
        //std::cout << "inC";
        emitBytes(static_cast<uint8_t>(OpCode::OP_CONSTANT), makeConstant(data));
    }

    void Compiler::number(bool can_assign) {
        //std::cout << "number";
        auto data = std::strtod(parser->previous.start.data(), nullptr);
	    emitConstant(data);
    }

    void Compiler::grouping(bool can_assign) {                                     
        expression();                                              
        parser->consume(TokenType::TOKEN_RIGHT_PAREN, "Expect ')' after expression.");
    }

    void Compiler::unary(bool can_assign) {
	    auto op = parser->previous.type;
        parsePrecedence(Precedence::PREC_UNARY);
	    switch (op) {
		    case (TokenType::TOKEN_MINUS) : 
                emitByte(static_cast<uint8_t>(OpCode::OP_NEGATE)); 
                break;
		    default:
			    break;
	    }
    }   

    void Compiler::expression() {
        parsePrecedence(Precedence::PREC_ASSIGNMENT);
    }

    void Compiler::endCompiler() {
        emitReturn();                   
        if (!parser->hadError) {                    
            disassembleChunk(chunk, "code");
        }   
    }

    void Compiler::binary(bool can_assign) {
	    auto op = parser->previous.type;
	    const auto& rule = getRule(op);
	    parsePrecedence(static_cast<Precedence>(static_cast<uint8_t>(rule.precedence) + 1));
        switch (op) {
		    case TokenType::TOKEN_PLUS: emitByte(static_cast<uint8_t>(OpCode::OP_ADD)); break;
		    case TokenType::TOKEN_MINUS: emitByte(static_cast<uint8_t>(OpCode::OP_SUBTRACT)); break;
		    case TokenType::TOKEN_STAR: emitByte(static_cast<uint8_t>(OpCode::OP_MULTIPLY)); break;
		    case TokenType::TOKEN_SLASH: emitByte(static_cast<uint8_t>(OpCode::OP_DIVIDE)); break;
		    default:
			    break;
	    }
    }

    /*void Compiler::varDeclaration() {
	    auto global = parseVariable("Expect variable name.");
	    if (parser->match(TokenType::TOKEN_EQUAL))
		    expression();
	    else
		    emitByte(OpCode::OP_NIL);

	    parser->consume(TokenType::TOKEN_SEMICOLON, "Expect ';' after variable declaration.");
	    defineVariable(global);
    }

    uint8_t Compiler::parseVariable(std::string_view error) {
	    parser->consume(TokenType::TOKEN_IDENTIFIER, error);

	    declareVariable();
	    if (current->scope_depth > 0) return 0;

	    return identifierConstant(parser->previous);
    }

    uint8_t Compiler::identifierConstant(const Token& name) {
	    return makeConstant(createObjString(name.start, vm));
    }*/

    void Compiler::parsePrecedence(Precedence precedence) {
	    parser->advance();
        
	    auto prefixRule = getRule(parser->previous.type).prefix;
	    if (prefixRule == nullptr) {
		    error(*parser, "Expect expression.");
		    return;
	    }

	    auto can_assign = precedence <= Precedence::PREC_ASSIGNMENT;
	    (this->*prefixRule)(can_assign);

	    while (precedence <= getRule(parser->current.type).precedence) {
		    parser->advance();
		    auto infix_rule = getRule(parser->previous.type).infix;
		    (this->*infix_rule)(can_assign);
	    }
    }

}                                                 

    