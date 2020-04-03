#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <cstdint>
#include <stdexcept>

#include "../include/chunk.hpp"

namespace Quar {
    class VM {
        private:
            Chunk chunk;
            std::vector<uint8_t>::const_iterator ip;
            std::vector<Data> stack;
            std::unordered_map<std::string, Data> globals;
            void run();

        public:
            void interpret(Chunk);
    };
}