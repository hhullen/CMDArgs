#ifndef SRC_ARGUMENT_ARGUMENT_H_
#define SRC_ARGUMENT_ARGUMENT_H_

#include <map>
#include <regex>
#include <string>

namespace hhullen {

using Str = std::string;

class Argument {
  using RegEx = std::regex;

 public:
  enum class Type { Int, Float, Str, Path };

  Argument();
  Argument(const Str &name, Argument::Type type, const Str &help);
  ~Argument();

  void ReadArgument(const Str &arg);
  Str GetValue();
  Str GetName();
  Str GetHelp();

  static bool IsArgument(const Str &arg) {
    return arg.size() > 0 && arg[0] != '-';
  }

 private:
  std::map<Type, RegEx> regex_;
  const Str &name_, help_;
  Argument::Type type_;
  Str value_;

  void ValidateArg(const Str &arg, Type type);
  void InitializeRegex();
};

Argument::Argument()
    : name_("default-init"), type_(Type::Str), help_("default-init") {
  InitializeRegex();
}

Argument::Argument(const Str &name, Argument::Type type, const Str &help)
    : name_(name), type_(type), help_(help) {
  InitializeRegex();
}

Argument::~Argument() {}

void Argument::ReadArgument(const Str &arg) {
  ValidateArg(arg, type_);
  value_ = arg;
}

Str Argument::GetValue() { return value_; }

Str Argument::GetName() { return name_; }

Str Argument::GetHelp() { return help_; }

static bool IsArgument(const Str &arg) {
  return arg.size() > 0 && arg[0] != '-';
}

void Argument::ValidateArg(const Str &arg, Type type) {
  if (!std::regex_match(arg, regex_[type])) {
    throw std::invalid_argument("Incorrect value type " + arg + " of option " +
                                name_);
  }
}

void Argument::InitializeRegex() {
  regex_[Type::Int] = RegEx("^[0-9]+$");
  regex_[Type::Float] = RegEx("^[0-9]+\\.[0-9]+$");
  regex_[Type::Str] = RegEx("^[a-zA-Z]+[0-9a-zA-z_-]*[a-zA-Z]*$");
  regex_[Type::Path] =
      RegEx("^[\\w\\/\\.]+[\\.\\\\/\\d\\w\\s\\+\\=\\#\\!\\@\\$\\(\\)\\:_-]*$");
}

}  // namespace hhullen

#endif  //  SRC_ARGUMENT_ARGUMENT_H_
