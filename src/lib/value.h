#pragma once
#include <variant>
#include <memory>
#include <ostream>
#include <vector>
#include "token.h"

struct Value;
class Function {
    public:
        std::vector<Token> arguments;
        std::vector<Token> block;

        Value getValue();
};

struct Value : public std::variant<double, 
                                    bool, 
                                    std::nullptr_t,
                                    std::shared_ptr<Function>, 
                                    std::shared_ptr<std::vector<Value>>> {
    //bool operator == (const Value& other) const;  
    //bool operator != (const Value& other) const;  

};

std::ostream& operator << (std::ostream& stream, const Value& value);


