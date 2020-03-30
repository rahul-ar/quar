#include <iostream>
#include <iomanip>
#include <string_view>
#include "../include/debug.hpp"
#include "../include/chunk.hpp"


namespace quar {
    size_t simpleInstruction(std::string_view name, size_t offset) {
	    std::cout << name << '\n';
	    return offset + 1;
    }

    size_t disassembleInstruction(const Chunk& chunk, size_t offset) {
        std::cout << std::setfill('0') << std::setw(4) << offset << ' ';
        if(offset >= chunk.codes.size())
            return offset - 1;
        auto instruction = static_cast<OpCode>(chunk.codes.at(offset));
        switch(instruction) {
            case OpCode::OP_RETURN :
                return simpleInstruction("OP_METHOD", offset);
            default : 
                std::cout << "Unknown OpCode ";
			    return offset + 1;
        }
    }

    void dissasembleChunk(const Chunk& chunk, std::string_view name) {
        std::cout << "== " << name << " ==\n";
	    for (size_t offset = 0; offset < chunk.codes.size();) {
		    offset = disassembleInstruction(chunk, offset);
	    }
    }

}
