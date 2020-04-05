#pragma once

#include <string_view>

#include "memory.hpp"
namespace quar {
    size_t disassembleInstruction(const Memory* memory, size_t offset);
    void disassembleChunk(const Memory* memory, std::string_view name);
}