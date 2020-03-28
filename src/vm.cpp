#include "../include/vm.hpp"

namespace quar {
    void VM::run() {
        while(true) {
            const auto instruction = static_cast<OpCode>(*ip++);
            switch(instruction) {
                case OpCode::OP_CONSTANT:
                    //
                    break;
                case OpCode::OP_ADD:
                    //
                    break;
                case OpCode::OP_RETURN:
                    return;
            }
        }
    }

    void VM::interpret(std::string source) {
        this->chunk = compile(source);
        this->ip = this->chunk.codes.begin();
        run();
    }
}