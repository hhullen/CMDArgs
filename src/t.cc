
#include <iostream>

#include "cmd_args.h"

using hhullen::Argument;
using hhullen::CMDArgs;

int main(int argc, char* argv[]) {
  hhullen::CMDArgs R(argc, argv);

  R.AddArguments({Argument("algorithm", Argument::Type::Int, "positional 1"),
                  Argument("mode", Argument::Type::Str, "positional 2")});

  std::cout << R.GetArgument("algorithm") << " arg\n";
  // R.ValidateArg(".234", hhullen::Argument::Type::Float);
  return 0;
}
