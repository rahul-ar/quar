#pragma once

#include <vector>
#include <cstdint>

namespace quar {
    enum class OpCode {
        OP_CONSTANT,
        OP_ADD,
        OP_RETURN
    };

    struct Chunk {
        std::vector<unsigned int> codes;
        std::vector<int> lines;
        std::vector<Value> constants;
        void pushCode(OpCode, int);
        std::vector<Value>::size_type pushConstant(Value);
    };
}