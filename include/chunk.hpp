#pragma once

#include <vector>
#include <cstdint>
#include <variant>
#include <string>

#include "../include/data.hpp"
namespace Quar {
    enum class OpCode : uint8_t {
        OP_CONSTANT,
        OP_ADD,
        OP_SUBTRACT,
        OP_MULTIPLY,
        OP_DIVIDE,
        OP_DEF_GLOBAL,
        OP_GET_GLOBAL,
        OP_SET_GLOBAL,
        OP_PRINT,
        OP_RETURN
    };

    struct Chunk {
        std::vector<uint8_t> codes;
        std::vector<int> lines;
        std::vector<Data> data;
        void pushCode(OpCode, int);
        void pushCode(uint8_t, int);
        size_t pushData(Data);
    };
}