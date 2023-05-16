
#include "positional_arg.h"

int main() {
  hhullen::PositionalArg R("mode", hhullen::Argument::Type::Str, "patameter",
                           std::list<std::string>({"val1", "val2", "val3"}));
  // R.ValidateArg(".234", hhullen::Argument::Type::Float);
  return 0;
}
