#pragma once
#include <string_view>
#include <array>
#include <optional>
#include <iostream>
#include "vm.hpp"
#include "chunk.hpp"
#include "scanner.hpp"
#include "data.hpp"
#include "debug.hpp"     


namespace quar {
    struct Parser;
    struct Chunk;
    struct VM;
    struct Compiler;

    enum class Precedence :uint8_t {                  
        PREC_NONE,                    
        PREC_ASSIGNMENT,  // =        
        PREC_OR,          // or       
        PREC_AND,         // and      
        PREC_EQUALITY,    // == !=    
        PREC_COMPARISON,  // < > <= >=
        PREC_TERM,        // + -      
        PREC_FACTOR,      // * /      
        PREC_UNARY,       // ! -      
        PREC_CALL,        // . ()     
        PREC_PRIMARY                  
    };

    using ParseFn = void (Compiler::*)(bool can_assign);

    struct ParseRule {
	    ParseFn prefix = nullptr;
	    ParseFn infix = nullptr;
	    Precedence precedence;
    };


    void error(Parser& parser, std::string_view message);
    void errorAtCurrent(Parser& parser, std::string_view message);
    void errorAt(Parser& parser, const Token& token, std::string_view message);
    
    struct Parser {
        Scanner scanner;
	    Token current;
	    Token previous;
	    bool hadError = false;
	    bool panicMode = false;

        Parser(std::string_view source) : scanner(source) {
            std::cout << source;
        }
        void advance();
        void consume(TokenType type, std::string_view message);
    };

    struct Compiler {
        Parser *parser = nullptr; 
        VM& vm;
        Chunk chunk;
        Compiler(VM& vm) : vm(vm) {
        }
           
    private:
        void parsePrecedence(Precedence);
        void expression();
        void grouping(bool can_assign);
        void number(bool can_assign);
        void unary(bool can_assign);
        void binary(bool can_assign);
        void emitByte(uint8_t byte);
        void emitBytes(uint8_t, uint8_t);
        void emitReturn();
        uint8_t makeConstant(Data);
        void emitConstant(Data);
        void endCompiler();
        /*uint8_t parseVariable(std::string_view error);
        uint8_t identifierConstant(const Token& name);
        void varDeclaration();*/
        
        
    public:
        bool compile(std::string_view source);
        constexpr static ParseRule rules[40] = {                                              
            { &Compiler::grouping, nullptr, Precedence::PREC_CALL },       // TokenType::LEFT_PAREN      
            { nullptr,     nullptr,    Precedence::PREC_NONE },       // TokenType::RIGHT_PAREN     
            { nullptr,     nullptr,    Precedence::PREC_NONE },       // TokenType::LEFT_BRACE
            { nullptr,     nullptr,    Precedence::PREC_NONE },       // TokenType::RIGHT_BRACE     
            { nullptr,     nullptr,    Precedence::PREC_NONE },       // TokenType::COMMA           
            { nullptr,     nullptr,    Precedence::PREC_CALL },       // TokenType::DOT             
            { &Compiler::unary, &Compiler::binary, Precedence::PREC_TERM },       // TokenType::MINUS           
            { nullptr,     &Compiler::binary, Precedence::PREC_TERM },       // TokenType::PLUS            
            { nullptr,     nullptr,    Precedence::PREC_NONE },       // TokenType::SEMICOLON       
            { nullptr,     &Compiler::binary,  Precedence::PREC_FACTOR },     // TokenType::SLASH           
            { nullptr,     &Compiler::binary,  Precedence::PREC_FACTOR },     // TokenType::STAR            
            { &Compiler::unary,     nullptr,   Precedence::PREC_NONE },       // TokenType::BANG            
            { nullptr,     &Compiler::binary,  Precedence::PREC_NONE },       // TokenType::BANG_EQUAL      
            { nullptr,     nullptr,    Precedence::PREC_NONE },       // TokenType::EQUAL           
            { nullptr,     &Compiler::binary,  Precedence::PREC_NONE },       // TokenType::EQUAL_EQUAL     
            { nullptr,     &Compiler::binary,  Precedence::PREC_NONE },       // TokenType::GREATER         
            { nullptr,     &Compiler::binary,  Precedence::PREC_NONE },       // TokenType::GREATER_EQUAL   
            { nullptr,     &Compiler::binary,  Precedence::PREC_NONE },       // TokenType::LESS            
            { nullptr,     &Compiler::binary,  Precedence::PREC_NONE },       // TokenType::LESS_EQUAL      
            { nullptr,     nullptr,    Precedence::PREC_NONE },       // TokenType::IDENTIFIER      
            { nullptr,     nullptr,    Precedence::PREC_NONE },       // TokenType::STRING          
            { &Compiler::number,   nullptr,    Precedence::PREC_NONE },       // TokenType::NUMBER          
            { nullptr,     nullptr,    Precedence::PREC_NONE },       // TokenType::AND             
            { nullptr,     nullptr,    Precedence::PREC_NONE },       // TokenType::CLASS           
            { nullptr,     nullptr,    Precedence::PREC_NONE },       // TokenType::ELSE            
            { nullptr,     nullptr,    Precedence::PREC_NONE },       // TokenType::FALSE           
            { nullptr,     nullptr,    Precedence::PREC_NONE },       // TokenType::FOR             
            { nullptr,     nullptr,    Precedence::PREC_NONE },       // TokenType::FUN             
            { nullptr,     nullptr,    Precedence::PREC_NONE },       // TokenType::IF              
            { nullptr,     nullptr,    Precedence::PREC_NONE },       // TokenType::NIL             
            { nullptr,     nullptr,    Precedence::PREC_NONE },       // TokenType::OR              
            { nullptr,     nullptr,    Precedence::PREC_NONE },       // TokenType::PRINT           
            { nullptr,     nullptr,    Precedence::PREC_NONE },       // TokenType::RETURN          
            { nullptr,     nullptr,    Precedence::PREC_NONE },       // TokenType::SUPER           
            { nullptr,     nullptr,    Precedence::PREC_NONE },       // TokenType::THIS            
            { nullptr,     nullptr,    Precedence::PREC_NONE },       // TokenType::TRUE            
            { nullptr,     nullptr,    Precedence::PREC_NONE },       // TokenType::VAR             
            { nullptr,     nullptr,    Precedence::PREC_NONE },       // TokenType::WHILE           
            { nullptr,     nullptr,    Precedence::PREC_NONE },       // TokenType::ERROR           
            { nullptr,     nullptr,    Precedence::PREC_NONE },       // TokenType::EOF             
            };
    };
}
