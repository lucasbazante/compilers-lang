#include <variant>
#include <vector>
#include <string>
#include <memory>

typedef std::variant<
 std::monostate, // nothing
 int,      // integers
 float,        // floats
 std::string,  // strings and identifiers
 bool      // booleans
> Value;

struct Struct {
   std::vector<Variable> vars;
};

struct Variable {
   std::string name;
   Value value;
   Struct struct_value;
   std::shared_ptr<Variable> ref_value;
};

struct ActivationRecord {
   std::vector<Variable> vars;
   void* return_label;
};

std::vector<ActivationRecord> stack;

void stack_push(std::string f_name, ParameterField* params;) {

}