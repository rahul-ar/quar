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
            std::unordered_map<std::string, Data> globals;
        public:
            void pushCode(OpCode, int);
            void pushCode(uint8_t, int);
            size_t pushData(Data);
            void storeGlobal(std::string, Data);
            Data getGlobal(std::string);
            void setGlobal(std::string, Data);
            const std::vector<uint8_t>& getCodes() const;
            const std::vector<Data>& getData() const;
            void patchMemory(size_t, uint8_t);
            void clearMemory();
    };
}