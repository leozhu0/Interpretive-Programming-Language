#include <memory>
#include <vector>
#include <variant>

struct Value : public std::variant<double, bool, std::shared_ptr<Function>, std::shared_ptr<std::vector<Value>>> {
};
