#include "../include/memory.hpp"

namespace quar {
    Memory::Memory() : ip(codes.begin()) {

    }

    void Memory::pushCode(OpCode opCode, int line) {
        this->codes.push_back(static_cast<uint8_t>(opCode));
        this->lines.push_back(line);
    }

    void Memory::pushCode(uint8_t param, int line) {
        this->codes.push_back(param);
        this->lines.push_back(line);
    }

    size_t Memory::pushData(Data data) {
        this->data.push_back(data);
        return this->data.size() - 1;
    }

    const std::vector<uint8_t>& Memory::getCodes() const {
        return this->codes;
    }

    const std::vector<Data>& Memory::getData() const {
        return this->data;
    }
}