#include "../include/debug.hpp"

#include <iostream>
#include <iomanip>
#include <string_view>

#include "../include/memory.hpp"

namespace quar {
    size_t simpleInstruction(std::string_view name, size_t offset) {
	    std::cout << name << '\n';
	    return offset + 1;
    }

    size_t constant_instruction(std::string_view name, const Memory* memory, size_t offset) {
	    auto constant = memory->getCodes().at(offset + 1);
	    std::cout << std::setfill(' ') << std::left << std::setw(16) << name << ' ';
	    std::cout << std::setw(4) << static_cast<unsigned>(constant) << '\n';
	    return offset + 2;
    }

    size_t disassembleInstruction(const Memory* memory, size_t offset) {
        std::cout << std::setfill('0') << std::setw(4) << offset << ' ';
        if(offset >= memory->getCodes().size())
            return offset - 1;
        auto instruction = static_cast<OpCode>(memory->getCodes().at(offset));
        switch(instruction) {
            case OpCode::OP_CONSTANT:
            case OpCode::OP_GET_GLOBAL:
		    case OpCode::OP_DEF_GLOBAL:
		    case OpCode::OP_SET_GLOBAL:
                return constant_instruction("OP_METHOD", memory, offset);
            case OpCode::OP_NIL:
		    case OpCode::OP_POP:
            case OpCode::OP_ADD:
		    case OpCode::OP_SUBTRACT:
		    case OpCode::OP_MULTIPLY:
		    case OpCode::OP_DIVIDE:
		    case OpCode::OP_NOT:
		    case OpCode::OP_NEGATE:    
            case OpCode::OP_RETURN :
                return simpleInstruction("OP_METHOD", offset);
            default : 
                std::cout << "Unknown OpCode \n";
			    return offset + 1;
        }
    }

    void disassembleChunk(const Memory* memory, std::string_view name) {
        std::cout << "== " << name << " ==\n";
	    for (size_t offset = 0; offset < memory->getCodes().size();) {
		    offset = disassembleInstruction(memory, offset);
	    }
    }
}
