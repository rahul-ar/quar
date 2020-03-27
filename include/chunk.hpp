#pragma once

namespace quar {
    enum class OpCode {
        OP_CONSTANT,
        OP_ADD,
        OP_RETURN
    };

    struct Chunk {
        std::vector<std::uint8_t> codes;
        std::vector<int> lines;
        std::vector<Value> constants;
        void pushCode(OpCode, int);
        std::vector<Value>::size_type pushConstant(Value);
    };
}