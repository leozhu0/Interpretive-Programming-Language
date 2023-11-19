#include "value.h"
#include <iostream>
#include <string>
#include <variant>
#include <sstream>


std::ostream& operator << (std::ostream& stream, const Value& value) {
    stream << std::visit([](const auto& arg) -> std::string{
        std::stringstream ss;
        ss << arg;
        return ss.str();
    }, value);
    return stream;
}

bool operator==(const Value& lhs, const Value& rhs) {
    if (lhs.index() != rhs.index()) {
        // Different types, return false
        return false;
    }

    // Same types, compare values
    bool result = std::visit([](const auto& a, const auto& b) -> bool {
        return static_cast<bool>(a == b);
    }, lhs, rhs);
    
    return result;
}

bool operator!=(const Value& lhs, const Value& rhs) {
    if (lhs.index() != rhs.index()) {
        // Different types, return false
        return false;
    }

    // Same types, compare values
    bool result = std::visit([](const auto& a, const auto& b) -> bool {
        return static_cast<bool>(a != b);
    }, lhs, rhs);
    
    return result;
}



//std::ostream& operator << (std::ostream& stream, const Value& value);


