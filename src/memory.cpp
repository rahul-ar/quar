#include "../include/memory.hpp"

namespace quar {
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
        return this->data.size()-1;
    }

    void Memory::storeGlobal(std::string var, Data data) {
        globals[var] = data;
    }

    Data Memory::getGlobal(std::string var) {
        const auto it = globals.find(var);
        if(it == globals.end()) {
            throw Error("Invalid variable name");
        }
        return it->second;
    }

    void Memory::setGlobal(std::string var, Data data) {
        // const auto it = globals.find(var);
        // if(it == globals.end()) {
        //     throw Error("Variable not defined");
        // }
        globals[var] = data;
    }

    const std::vector<uint8_t>& Memory::getCodes() const{
        return this->codes;
    }

    const std::vector<Data>& Memory::getData() const {
        return this->data;
    }

    void Memory::patchMemory(size_t offset, uint8_t byte) {
        codes[offset] = byte;
    }

    void Memory::clearMemory() {
        codes.clear();
        data.clear();
        lines.clear();
    }
}