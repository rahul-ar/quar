#include "../include/data.hpp"

#include <iostream>
#include <type_traits>
#include <variant>

#include "../include/memory.hpp"

namespace {
    template<class... Ts> struct overloaded : Ts... { using Ts::operator()...; };
    template<class... Ts> overloaded(Ts...) -> overloaded<Ts...>;
}

namespace quar {
    void print(const Data data) {
        std::visit([](auto&& arg) {
            std::cout << arg;
        }, data);
    }

    bool truey(const Data a) {
        return std::visit(overloaded {
            [] (bool a) -> bool {
                return a;
            },
            [] (auto&& a) -> bool {
                throw Error("Invalid operand type");
            }
        }, a);
    }

    Data nott(const Data a) {
        return std::visit(overloaded {
            [] (bool a) -> Data {
                return Data(!a);
            },
            [] (auto&& a) -> Data {
                throw Error("Invalid operand type");
            }
        }, a);
    }

    Data negate(const Data a) {
        return std::visit(overloaded {
            [] (int a) -> Data {
                return Data(-a);
            },
            [] (double a) -> Data {
                return Data(-a);
            },
            [] (auto&& a) -> Data {
                throw Error("Invalid operand type");
            }
        }, a);
    }

    Data equal(const Data a, const Data b) {
        return std::visit(overloaded {
            [] (int a, int b) -> Data {
                return Data(a == b);
            },
            [] (double a, double b) -> Data {
                return Data(a == b);
            },
            [] (std::string a, std::string b) -> Data {
                return Data(a == b);
            },
            [] (auto&& a, auto&& b) -> Data {
                return Data(false);
            }
         }, a, b);
    }

    Data greater(const Data a, const Data b) {
        return std::visit(overloaded {
            [] (int a, int b) -> Data {
                return Data(a > b);
            },
            [] (double a, double b) -> Data {
                return Data(a > b);
            },
            [] (auto&& a, auto&& b) -> Data {
                throw Error("Invalid operand types");
            }
         }, a, b);
    }

    Data lesser(const Data a, const Data b) {
        return std::visit(overloaded {
            [] (int a, int b) -> Data {
                return Data(a < b);
            },
            [] (double a, double b) -> Data {
                return Data(a < b);
            },
            [] (auto&& a, auto&& b) -> Data {
                throw Error("Invalid operand types");
            }
         }, a, b);
    }

    Data add(const Data a, const Data b) {
        return std::visit(overloaded {
            [] (int a, int b) -> Data {
                return Data(a + b);
            },
            [] (double a, double b) -> Data {
                return Data(a + b);
            },
            [] (std::string a, std::string b) -> Data {
                return Data(a + b);
            },
            [] (auto&& a, auto&& b) -> Data {
                throw Error("Invalid operand types");
            }
        }, a, b);
    }

    Data subtract(const Data a, const Data b) {
        return std::visit(overloaded {
            [] (int a, int b) -> Data {
                return Data(a - b);
            },
            [] (double a, double b) -> Data {
                return Data(a - b);
            },
            [] (auto&& a, auto&& b) -> Data {
                throw Error("Invalid operand types");
            }
        }, a, b);
    }

    Data multiply(const Data a, const Data b) {
        return std::visit(overloaded {
            [] (int a, int b) -> Data {
                return Data(a * b);
            },
            [] (double a, double b) -> Data {
                return Data(a * b);
            },
            [] (auto&& a, auto&& b) -> Data {
                throw Error("Invalid operand types");
            }
        }, a, b);
    }

    Data divide(const Data a, const Data b) {
        return std::visit(overloaded {
            [] (int a, int b) -> Data {
                return Data(a / b);
            },
            [] (double a, double b) -> Data {
                return Data(a / b);
            },
            [] (auto&& a, auto&& b) -> Data {
                throw Error("Invalid operand types");
            }
        }, a, b);
    }
}