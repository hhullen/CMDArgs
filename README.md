# CMDArgs
This is one-header library to read CMD arguments.

## Classes definition
```c++
namespace hhullen {

class CMDArgs {
 public:
  CMDArgs();
  ~CMDArgs();

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

## Installation


