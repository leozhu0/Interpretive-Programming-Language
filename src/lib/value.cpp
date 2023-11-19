#include "value.h"
#include <iostream>
#include <string>
#include <variant>
#include <sstream>

std::ostream& operator << (std::ostream& stream, const Value& value) {
    stream << visit([](const auto& arg) -> std::string {
        std::stringstream ss;
        ss << arg;
        return ss.str();
    }, value);
    return stream;
}