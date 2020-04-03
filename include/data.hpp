#pragma once

#include <string>
#include <variant>
#include <stdexcept>

namespace {
    struct Error : std::runtime_error {
        using std::runtime_error::runtime_error;
    };
}


namespace Quar {
    typedef std::variant<double, int, std::string, nullptr_t> Data;
    void print(const Data);
    Data add(const Data, const Data);
    Data subtract(const Data, const Data);
    Data multiply(const Data, const Data);
    Data divide(const Data, const Data);
}