# CMDArgs
Library to operate CMD arguments. Used C++ 20 std.

# Classes definition
```c++
namespace hhullen {

class CMDArgs {
 public:
  CMDArgs();
  void AddArguments(const std::initializer_list<Argument>& args);
  void AddFlags(const std::initializer_list<Flag>& flags);
  std::string GetArgument(const std::string& name);
  std::list<std::string> GetFlagValues(const std::string& name);
  void Read(int argc, const char* argv[]);
  ...
};

}
```

```c++
namespace hhullen {

class Argument {
 public:
  enum class Type { Int, Float, Str, Path };
  Argument(const std::string &name, Argument::Type type, const std::string &help);
  ...
};

}
```

```c++
namespace hhullen {

class Flag {
 public:
  Flag(const Str& long_name, const char short_name, const Str& help,
       const std::initializer_list<Argument>& args);
  ...
};

}
```

# Installation
This is the one-header library, so `*.h` files all are needed. Try `make lib` command from src directory. It will create `cmd_args` directory with necessary files. This is the directory to be copied to your project.

# Usage
- Once the `cmd_args` directory copied to project, include header file `cmd_args.h`.  
- Create the structure of expected command line arguments. For example the next command line arguments expected:
```
./program SLE recursive --file ../some/file.txt -n 3 2.718 --bool
```
So the next structure has to be written:
```c++
#include "cmd_args/cmd_args.h"

using hhullen::Argument;
using hhullen::CMDArgs;
using hhullen::Flag;

int main(int argc, const char* argv[]) {
  // create some positional arguments:
  Argument alorithm =
      Argument("algorithm", Argument::Type::Str, "algorithm name");
  Argument mode =
      Argument("mode", Argument::Type::Str, "mode name");

  // create argument
  Argument path =
      Argument("path_to_file", Argument::Type::Path, "path to file");
  // create flag and include created argument as its value
  Flag file_flag =
      Flag("file", 'f', "path_to_file", {path});

  // create arguments for another flag
  Argument repeats =
      Argument("repeats", Argument::Type::Int, "repeats of smt");
  Argument number =
      Argument("number", Argument::Type::Float, "number of smt");
  // create flag and include both as its value
  Flag num_flag =
      Flag("num", 'n', "amount of smt", {repeats, number});

  // create flag with no necessary values
  Flag bool_flag =
      Flag("bool", 'b', "true/false", {});

  // create CMDArgs object
  CMDArgs cmd;
  // add arguments
  cmd.AddArguments({alorithm, mode});
  // add flags
  cmd.AddFlags({file_flag, num_flag, bool_flag});
  // read (parse) command line
  cmd.Read(argc, argv);

  ...

  return 0;
}
```
 

- Get parsed arguments/flags when it is needed:
```c++
  std::string algoithm_name = cmd.GetArgument("algorithm");
  std::string mode_name = cmd.GetArgument("mode");

  std::list<std::string> values = cmd.GetFlagValues("-f");
  std::list<std::string> values = cmd.GetFlagValues("--num");
  std::list<std::string> values = cmd.GetFlagValues("--bool");
```
 
## Note
- The exception `std::invalid_argument` will be thrown:
  - two or more arguments or flags with similar names are set  
  - any argument or flag is tried to get but was not specified in command line  
  - any unexpected argument specified in command line
  - any command line argument has incorrect type
- arguments and their order is required
- flags is optional, so any flag may be missing
- flag with no required values return string 'true' (as bool flag at the example above)
- no matter where flag will be set in command. For example: 
```
./program -b SLE --num 1 0.359 recursive
```
- help methods arguments can be empty string now as it have not been implemented yet

# Make targets
- `make lib` - create `cmd_args` directory with necessary files
- `make check` - run cppcheck and linter for whole library code
- `make tests` - run tests
- `make gcov_report` - create tests coverage report in `report` dirctory
- `make clean` - all files could be created by any targets
- `make all` - run targets in order: `clean gcov_report tests check`
