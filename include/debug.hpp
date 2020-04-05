#pragma once

#include <string>

#include "memory.hpp"

namespace quar {
    size_t disassembleInstruction(const Memory* memory, size_t offset);
    void disassembleChunk(const Memory* memory, std::string name);
}