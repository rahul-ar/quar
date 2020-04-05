#pragma once

#include <vector>
#include <unordered_map>
#include <cstdint>
#include <variant>
#include <string>

#include "data.hpp"

namespace quar {
    enum class OpCode : uint8_t {
        OP_POP,
        OP_CONSTANT,
        OP_NOT,
        OP_EQUAL,
        OP_GREATER,
        OP_LESSER,
        OP_NEGATE,
        OP_NIL,
        OP_ADD,
        OP_SUBTRACT,
        OP_MULTIPLY,
        OP_DIVIDE,
        OP_DEF_GLOBAL,
        OP_GET_GLOBAL,
        OP_SET_GLOBAL,
        OP_JUMP,
        OP_JUMP_IF_FALSE,
        OP_LOOP,
        OP_PRINT,
        OP_RETURN
    };

    class Memory {
        private:
            std::vector<uint8_t> codes;
            std::vector<int> lines;
            std::vector<Data> data;
        public:
            // Memory();
            std::vector<uint8_t>::const_iterator ip;
            std::unordered_map<std::string, Data> globals;
            void pushCode(OpCode, int);
            void pushCode(uint8_t, int);
            size_t pushData(Data);
            const std::vector<uint8_t>& getCodes() const;
            const std::vector<Data>& getData() const;
            void clearMemory();
    };
}