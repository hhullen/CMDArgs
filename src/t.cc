
#include <iostream>

#include "cmd_args.h"

using hhullen::Argument;
using hhullen::CMDArgs;
using hhullen::Flag;

int main(int argc, const char* argv[]) {
  hhullen::CMDArgs R;

  R.AddArguments({Argument("algorithm", Argument::Type::Int, "positional 1"),
                  Argument("mode", Argument::Type::Str, "positional 2")});

  Flag file_f =
      Flag("file", 'f', "flag-1",
           {Argument("path_to_file", Argument::Type::Path, "optional 1")});

  Flag file_f2 =
      Flag("file-d", 'f', "flag-1",
           {Argument("path_to_file", Argument::Type::Path, "optional 2")});

  R.AddFlags({file_f, file_f2});
  R.Read(argc, argv);
  std::list<std::string> values;
  values = R.GetFlagValues("--file");
  std::cout << R.GetArgument("mode") << " | " << R.GetArgument("algorithm")
            << " | " << values.front() << "\n";
  return 0;
}
