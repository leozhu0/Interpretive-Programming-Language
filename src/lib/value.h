#pragma once
#include <variant>
#include <memory>
#include <ostream>
#include <vector>
#include "token.h"
#include <sstream>

struct Value;

class Function {
    public:
        std::vector<Token> arguments;
        std::vector<Token> block;

        Value getValue(std::vector<Value> argVals, std::map<std::string, Value> variables);
};

struct Value : public std::variant<double, 
                                    bool,
				    std::shared_ptr<Function>
                                    //std::nullptr_t,
                                   // Function,
                                    //std::shared_ptr<std::vector<Value>>
                                    > {
    using variant::variant;
};


std::ostream& operator << (std::ostream& stream, const Value& value);

bool operator==(const Value& lhs, const Value& rhs);

bool operator!=(const Value& lhs, const Value& rhs);


