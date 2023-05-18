
#include <iostream>

#include "cmd_args.h"

using hhullen::Argument;
using hhullen::CMDArgs;

int main(int argc, const char* argv[]) {
  hhullen::CMDArgs R;

  R.AddArguments({Argument("algorithm", Argument::Type::Int, "positional 1"),
                  Argument("mode", Argument::Type::Str, "positional 2")});
  R.Read(argc, argv);
  std::cout << R.GetArgument("mode") << " arg\n";
  // R.ValidateArg(".234", hhullen::Argument::Type::Float);
  return 0;
}
