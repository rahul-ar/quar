#include "chunk.hpp"

namespace quar {
    void Chunk::pushCode(OpCode opcode, int line) {
        this->codes.push_back(static_cast<uint8_t> opCode);
        this->lines.push_back(line);
    }

    std::vector<Value>::size_type Chunk::pushConstant(Value value) {
        this->constants.push_back(value);
        return this->constants.size() - 1;
    }
}