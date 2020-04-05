#include "../include/debug.hpp"

#include <iostream>
#include <iomanip>
#include <string>

#include "../include/memory.hpp"

namespace quar {
    size_t simpleInstruction(std::string name, size_t offset) {
	    std::cout << name << '\n';
	    return offset + 1;
    }

    size_t constantInstruction(std::string name, const Memory* memory, size_t offset) {
	    auto constant = memory->getData().at(offset + 1);
	    std::cout << std::setfill(' ') << std::left << std::setw(16) << name << ' ';
	    return offset + 2;
    }

    size_t disassembleInstruction(const Memory* memory, size_t offset) {
        std::cout << std::setfill('0') << std::setw(4) << offset << ' ';
        if(offset >= memory->getCodes().size())
            return offset - 1;
        auto instruction = static_cast<OpCode>(memory->getCodes().at(offset));
        switch(instruction) {
            case OpCode::OP_CONSTANT:
                return constantInstruction("OP_CONSTANT", memory, offset);
            case OpCode::OP_GET_GLOBAL:
                return constantInstruction("OP_GET_GLOBAL", memory, offset);
		    case OpCode::OP_DEF_GLOBAL:
                return constantInstruction("OP_DEF_GLOBAL", memory, offset);
		    case OpCode::OP_SET_GLOBAL:
                return constantInstruction("OP_SET_GLOBAL", memory, offset);
            case OpCode::OP_NIL:
                return simpleInstruction("OP_NIL", offset);
		    case OpCode::OP_POP:
                return simpleInstruction("OP_POP", offset);
            case OpCode::OP_ADD:
                return simpleInstruction("OP_ADD", offset);
		    case OpCode::OP_SUBTRACT:
                return simpleInstruction("OP_SUBTRACT", offset);
		    case OpCode::OP_MULTIPLY:
                return simpleInstruction("OP_MULTIPLY", offset);
		    case OpCode::OP_DIVIDE:
                return simpleInstruction("OP_DIVIDE", offset);
		    case OpCode::OP_NOT:
                return simpleInstruction("OP_NOT", offset);
		    case OpCode::OP_NEGATE:    
                return simpleInstruction("OP_NEGATE", offset);
            case OpCode::OP_RETURN :
                return simpleInstruction("OP_RETURN", offset);
            default : 
                std::cout << "Unknown OpCode \n";
			    return offset + 1;
        }
    }

    void disassembleChunk(const Memory* memory, std::string name) {

    }
}
