#pragma once

#include <vector>
#include <unordered_map>
#include <string>
#include <cstdint>
#include <stdexcept>

#include "memory.hpp"
#include "compiler.hpp"

namespace quar {
    class VM {
        private:
            Memory memory;
            Compiler compiler;
            std::vector<Data> stack;
            void run();
        public:
            VM();
            void interpret(std::string);
    };
}