#include "../include/vm.hpp"

#include <iostream>

namespace quar {
    VM::VM() : compiler(Compiler(&memory)) {
        
    }

    void VM::interpret(std::string source) {
        compiler.compile(source);
        run();
    }

    void VM::run() {
        while(true) {
            const auto instruction = static_cast<OpCode>(*memory.ip++);
            switch(instruction) {
                case OpCode::OP_POP: {
                    stack.pop_back();
                    break;
                }

                case OpCode::OP_CONSTANT: {
                    stack.push_back(memory.getData().at(*memory.ip++));
                    break;
                }

                case OpCode::OP_NOT: {
                    const auto a = stack.back();
                    stack.pop_back();
                    stack.push_back(nott(a));
                    break;
                }

                case OpCode::OP_NEGATE: {
                    const auto a = stack.back();
                    stack.pop_back();
                    stack.push_back(negate(a));
                    break;
                }

                case OpCode::OP_EQUAL: {
                    const auto a = std::move(stack.back());
                    stack.pop_back();
                    const auto b = std::move(stack.back());
                    stack.pop_back();
                    stack.push_back(equal(a, b));
                    break;
                }                

                case OpCode::OP_GREATER: {
                    const auto a = std::move(stack.back());
                    stack.pop_back();
                    const auto b = std::move(stack.back());
                    stack.pop_back();
                    stack.push_back(greater(a, b));
                    break;
                }         

                case OpCode::OP_LESSER: {
                    const auto a = std::move(stack.back());
                    stack.pop_back();
                    const auto b = std::move(stack.back());
                    stack.pop_back();
                    stack.push_back(lesser(a, b));
                    break;
                }                         

                case OpCode::OP_ADD: {
                    const auto a = std::move(stack.back());
                    stack.pop_back();
                    const auto b = std::move(stack.back());
                    stack.pop_back();
                    stack.push_back(add(a, b));
                    break;
                }

                case OpCode::OP_SUBTRACT: {
                    const auto a = std::move(stack.back());
                    stack.pop_back();
                    const auto b = std::move(stack.back());
                    stack.pop_back();
                    stack.push_back(subtract(a, b));
                    break;
                }

                case OpCode::OP_MULTIPLY: {
                    const auto a = std::move(stack.back());
                    stack.pop_back();
                    const auto b = std::move(stack.back());
                    stack.pop_back();
                    stack.push_back(multiply(a, b));
                    break;
                }

                case OpCode::OP_DIVIDE: {
                    const auto a = std::move(stack.back());
                    stack.pop_back();
                    const auto b = std::move(stack.back());
                    stack.pop_back();
                    stack.push_back(divide(a, b));
                    break;
                }

                case OpCode::OP_DEF_GLOBAL: {
                    std::string var = std::get<std::string>(memory.getData().at(*memory.ip++));;
                    memory.globals[var] = stack.back();
                    stack.pop_back();
                    break; 
                }

                case OpCode::OP_GET_GLOBAL: {
                    std::string var = std::get<std::string>(memory.getData().at(*memory.ip++));
                    const auto it = memory.globals.find(var);
                    if(it == memory.globals.end()) {
                        throw Error("Invalid variable name");
                    }
                    stack.push_back(it->second);
                    break;
                }

                case OpCode::OP_SET_GLOBAL: {
                    std::string var = std::get<std::string>(memory.getData().at(*memory.ip++));
                    const auto it = memory.globals.find(var);
                    if(it == memory.globals.end()) {
                        throw Error("Variable not defined");
                    }
                    it->second = stack.back();
                    stack.pop_back();
                    break;
                }

                case OpCode::OP_JUMP: {
                    const auto jumpLength = reinterpret_cast<const uint16_t&>(*memory.ip);
                    memory.ip += jumpLength + 2;
                    break;
                }

                case OpCode::OP_JUMP_IF_FALSE: {
                    const auto jumpLength = reinterpret_cast<const uint16_t&>(*memory.ip);
                    memory.ip += 2;
                    if(!truey(stack.back())) {
                        memory.ip += jumpLength;
                    }
                    break;
                }

                case OpCode::OP_LOOP: {
                    const auto jumpLength = reinterpret_cast<const uint16_t&>(*memory.ip);
                    memory.ip += - jumpLength - 1;
                    break;
                }

                case OpCode::OP_PRINT: {
                    print(stack.back());
                    std::cout << std::endl;
                    stack.pop_back();
                    break;
                }
                
                case OpCode::OP_RETURN: {
                    return;
                }

                case OpCode::OP_NIL: {
                    stack.push_back(0);
                    break;
                }
            }
        }
    }
}