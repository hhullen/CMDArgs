
#include "argument.h"

int main() {
  hhullen::Argument R;
  R.ValidateArg(".234", hhullen::Argument::Type::Float);
  return 0;
}
