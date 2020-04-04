#include <stdexcept>

namespace quar {
    struct Error : std::runtime_error {
        using std::runtime_error::runtime_error;
    };
}