#pragma once

#include <string>
#include <variant>

#include "error.hpp"

namespace quar {
    typedef std::variant<double, int, std::string, bool> Data;
    void print(const Data);
    bool truey(const Data);
    Data nott(const Data);
    Data negate(const Data);
    Data equal(const Data, const Data);
    Data greater(const Data, const Data);
    Data lesser(const Data, const Data);
    Data add(const Data, const Data);
    Data subtract(const Data, const Data);
    Data multiply(const Data, const Data);
    Data divide(const Data, const Data);
}