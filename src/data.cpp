#include "../include/data.hpp"

#include <iostream>
#include <type_traits>
#include <variant>

#include "../include/chunk.hpp"

namespace {
    template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
    template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
}

namespace Quar {
    void print(const Data data) {
        std::visit([](auto&& arg) {std::cout << arg;}, data);
    }

    Data add(const Data a, const Data b) {
        return std::visit(overloaded {
            [] (int arg1, int arg2) -> Data {
                return Data(arg1 + arg2);
            },
            [](double arg1, double arg2) -> Data {
                return Data(arg1 + arg2);
            },
            [](std::string arg1, std::string arg2) -> Data {
                return Data(arg1 + arg2);
            },
            [](auto&& arg1, auto&& arg2) -> Data {
                throw Error{"Invalid operand types"};
            }
        }, a, b);
    }

    Data subtract(const Data a, const Data b) {
        return std::visit(overloaded {
            [] (int arg1, int arg2) -> Data {
                return Data(arg1 - arg2);
            },
            [](double arg1, double arg2) -> Data {
                return Data(arg1 - arg2);
            },
            [](auto&& arg1, auto&& arg2) -> Data {
                throw Error{"Invalid operand types"};
            }
        }, a, b);
    }

    Data multiply(const Data a, const Data b) {
        return std::visit(overloaded {
            [] (int arg1, int arg2) -> Data {
                return Data(arg1 * arg2);
            },
            [](double arg1, double arg2) -> Data {
                return Data(arg1 * arg2);
            },
            [](auto&& arg1, auto&& arg2) -> Data {
                throw Error{"Invalid operand types"};
            }
        }, a, b);
    }

    Data divide(const Data a, const Data b) {
        return std::visit(overloaded {
            [] (int arg1, int arg2) -> Data {
                return Data(arg1 / arg2);
            },
            [](double arg1, double arg2) -> Data {
                return Data(arg1 / arg2);
            },
            [](auto&& arg1, auto&& arg2) -> Data {
                throw Error{"Invalid operand types"};
            }
        }, a, b);
    }
}