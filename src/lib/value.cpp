#include "value.h"
#include <iostream>
#include <string>
#include <variant>
#include <sstream>
#include <map>
#include <vector>
//#include "../scrypt.cpp"

Value Function::getValue(std::vector<Value> argVals){
	//run the code using the arguments
	//std::map<std::string, Value> variables
	//for(int i = 0; i < argVals.size(); i++){
	//variables[arguments[i]] = argVals[i]; 
	//}
	//return parseBlock(block, variables);
	return Value{5.0};//for testing
}


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


