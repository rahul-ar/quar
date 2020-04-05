#include <iostream>
#include <fstream>
#include <iterator>
#include <string>

#include "../include/vm.hpp"

using namespace quar;

int main(int argc, const char* argv[]) {
    VM vm;
    if(argc == 1) {
        while(true) {
            std::cout << ">>>";
            std::string source;
            std::getline(std::cin, source);
            vm.interpret(source);
        }
    }
    else if(argc == 2) {
        const auto source = ([&]() {
            std::ifstream in(argv[1]);
            return std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
        })();
        vm.interpret(source);
    }
    else {
        std::cout << "Improper usage";
    }
}