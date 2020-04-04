
#include <iostream>
#include <fstream>
#include <iterator>
#include <string>


#include "../include/chunk.hpp"
#include "../include/vm.hpp"
#include "../include/scanner.hpp"
#include "../include/debug.hpp"

using namespace quar;

int main(int argc, const char* argv[]) {
    VM vm;
    if(argc == 1) {
        /*while(true) {
            std::cout << ">>>";
            std::string source;
            std::cin >> source;
        }*/

        // Code written for testing
        //
        Chunk chunk;
        //chunk.pushCode(OpCode::OP_CONSTANT, 1);
        chunk.pushCode(OpCode::OP_RETURN, 3);
        disassembleChunk(chunk, "test chunk");
        Compiler c(vm);
        c.compile("t=9;");

        //std::cout << c.compile'("1+2");
        /*size_t offset = chunk.pushData(Data(5));
        chunk.pushCode(OpCode::OP_CONSTANT, 1);
        chunk.pushCode(offset, 1);
        offset = chunk.pushData(Data("a"));
        chunk.pushCode(OpCode::OP_DEF_GLOBAL, 1);
        chunk.pushCode(offset, 1);
        offset = chunk.pushData(Data("a"));
        chunk.pushCode(OpCode::OP_GET_GLOBAL, 2);
        chunk.pushCode(offset, 2);
        offset = chunk.pushData(Data("a"));
        chunk.pushCode(OpCode::OP_GET_GLOBAL, 2);
        chunk.pushCode(offset, 2);
        chunk.pushCode(OpCode::OP_ADD, 1);
        chunk.pushCode(OpCode::OP_PRINT, 2);
        chunk.pushCode(OpCode::OP_RETURN, 3);
        vm.interpret(chunk);*/

    }
    else if(argc == 2) {
        /*const auto source = ([&]() {
            std::ifstream in(argv[1]);
            return std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
        })();
        // vm.interpret(source);
    }
    else {
        std::cout << "Improper usage";*/
    }
}