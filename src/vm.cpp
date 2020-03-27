#include "vm.h"

namespace quar {
    void run() {
        while(true) {
            switch(*it++) {
                case OP_CONSTANT:
                    //
                    break;
                case OP_ADD:
                    //
                    break;
                case OP_RETURN:
                    return;
            }
        }
    }

    void interpret(std::string source) {
        this->chunk = compile(source);
        this->ip = this->chunk.codes.begin();
        run();
    }
}