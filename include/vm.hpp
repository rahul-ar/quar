#pragma once

#include <vector>
#include <string>
#include <cstdint>

#include "../include/chunk.hpp"

namespace quar {
    class VM {
        private:
            Chunk chunk;
            std::vector<unsigned int>::const_iterator ip;
            std::vector<Value> stack;
            void run();

        public:
            void interpret(std::string);
    };
}