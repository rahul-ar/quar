
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
    if(argc == 1) {
        while(true) {
            std::cout << ">>>";
            std::string source;
            std::cin >> source;
        }
    }
    else if(argc == 2) {
        const auto src = ([&]() {
            std::ifstream in(argv[1]);
            return std::string(std::istreambuf_iterator<char>(in), std::istreambuf_iterator<char>());
        })();
    }
    else {
        std::cout << "Improper usage";
    }
}