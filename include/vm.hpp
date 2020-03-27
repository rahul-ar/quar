#pragma once

namespace quar {
    class VM {
        private:
            Chunk chunk;
            std::vector<Chunk>::const_iterator ip;
            std::vector<Value> stack;
            void run();

        public:
            void interpret(std::string);
    };
}