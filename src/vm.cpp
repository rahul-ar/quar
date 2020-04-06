#include "../include/vm.hpp"

#include <iostream>

namespace quar {
    VM::VM() : compiler(Compiler(&memory)) {
        
    }

    void VM::interpret(std::string source) {
        compiler.compile(source);
        ip = memory.getCodes().begin();
        run();
    }

    void VM::run() {
        while(true) {
            const auto instruction = static_cast<OpCode>(*ip++);
            switch(instruction) {
                case OpCode::OP_POP: {
                    stack.pop_back();
                    break;
                }

                case OpCode::OP_CONSTANT: {
                    stack.push_back(memory.getData().at(*ip++));
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
                    std::string var = std::get<std::string>(memory.getData().at(*ip++));;
                    memory.storeGlobal(var, stack.back());
                    stack.pop_back();
                    break; 
                }

                case OpCode::OP_GET_GLOBAL: {
                    std::string var = std::get<std::string>(memory.getData().at(*ip++));
                    stack.push_back(memory.getGlobal(var));
                    break;
                }

                case OpCode::OP_SET_GLOBAL: {
                    std::string var = std::get<std::string>(memory.getData().at(*ip++));
                    memory.setGlobal(var, stack.back());
                    break;
                }

                case OpCode::OP_JUMP: {
                    const auto jumpLength = *ip;
                    ip += jumpLength + 1;
                    break;
                }

                case OpCode::OP_JUMP_IF_FALSE: {
                    const auto jumpLength = *ip;
                    ip += 1;
                    if(!truey(stack.back())) {
                        ip += jumpLength;
                    }
                    break;
                }

                case OpCode::OP_LOOP: {
                    const auto jumpLength = reinterpret_cast<const uint16_t&>(*ip);
                    ip += - jumpLength - 1;
                    break;
                }

                case OpCode::OP_PRINT: {
                    print(stack.back());
                    std::cout << std::endl;
                    stack.pop_back();
                    break;
                }
                
                case OpCode::OP_RETURN: {
                    memory.clearMemory();
                    return;
                }
            }
        }
    }
}