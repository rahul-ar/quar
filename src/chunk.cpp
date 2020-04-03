#include "../include/chunk.hpp"

namespace Quar {
    void Chunk::pushCode(OpCode opCode, int line) {
        this->codes.push_back(static_cast<uint8_t>(opCode));
        this->lines.push_back(line);
    }

    void Chunk::pushCode(uint8_t param, int line) {
        this->codes.push_back(param);
        this->lines.push_back(line);
    }

    size_t Chunk::pushData(Data data) {
        this->data.push_back(data);
        return this->data.size()-1;
    }
}