#pragma once
#include <string_view>

namespace quar {
    struct Chunk;
    size_t disassembleInstruction(const Chunk& chunk, size_t offset);
    void dissasembleChunk(const Chunk& chunk, std::string_view name);
}