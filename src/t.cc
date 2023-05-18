
#include <iostream>

#include "cmd_args.h"

using hhullen::Argument;
using hhullen::CMDArgs;
using hhullen::Flag;

int main(int argc, const char* argv[]) {
  hhullen::CMDArgs R;

  R.AddArguments({Argument("algorithm", Argument::Type::Int, "positional 1"),
                  Argument("mode", Argument::Type::Str, "positional 2")});
  R.Read(argc, argv);
  std::cout << R.GetArgument("mode") << " arg\n";

  Flag file_f =
      Flag("file", 'f', "flag-1",
           {Argument("path_to_file", Argument::Type::Path, "optional 1")});
  R.AddFlags({file_f});
  return 0;
}
