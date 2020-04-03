#include "../include/vm.hpp"

#include <iostream>

namespace Quar {
    void VM::run() {
        while(true) {
            const auto instruction = static_cast<OpCode>(*this->ip++);
            switch(instruction) {
                case OpCode::OP_CONSTANT:
                    this->stack.push_back(this->chunk.data.at(*this->ip++));
                    break;

                case OpCode::OP_DEF_GLOBAL: {
                    std::string var = std::get<std::string>(this->chunk.data.at(*this->ip++));;
                    globals[var] = stack.back();
                    stack.pop_back();
                    break; 
                }

                case OpCode::OP_GET_GLOBAL: {
                    std::string var = std::get<std::string>(this->chunk.data.at(*this->ip++));
                    const auto it = globals.find(var);
                    if(it == globals.end()) {
                        throw Error {"Invalid variable name"};
                    }
                    stack.push_back(it->second);
                    break;
                }

                case OpCode::OP_SET_GLOBAL: {
                    std::string var = std::get<std::string>(this->chunk.data.at(*this->ip++));
                    const auto it = globals.find(var);
                    if(it == globals.end()) {
                        throw Error {"Variable not defined"};
                    }
                    it->second = stack.back();
                    stack.pop_back();
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

                case OpCode::OP_PRINT: 
                    print(stack.back());
                    std::cout << std::endl;
                    stack.pop_back();
                    break;
                
                case OpCode::OP_RETURN:
                    return;
            }
        }
    }

    void VM::interpret(std::string source) {
        this->chunk = chunk;
        this->ip = this->chunk.codes.begin();
        run();
    }

    // Code written for testing
    //
    // void VM::interpret(Chunk chunk) {
    //     this->chunk = chunk;
    //     this->ip = this->chunk.codes.begin();
    //     run();
    // }
}