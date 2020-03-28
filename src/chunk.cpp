#include "../include/chunk.hpp"

namespace quar {
    void Chunk::pushCode(OpCode opCode, int line) {
        this->codes.push_back(static_cast<unsigned int>(opCode));
        this->lines.push_back(line);
    }

    std::vector<Value>::size_type Chunk::pushConstant(Value value) {
        this->constants.push_back(value);
        return this->constants.size() - 1;
    }
}