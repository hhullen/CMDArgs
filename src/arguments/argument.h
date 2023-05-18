#ifndef SRC_ARGUMENT_ARGUMENT_H_
#define SRC_ARGUMENT_ARGUMENT_H_

#include <map>
#include <regex>
#include <string>

namespace hhullen {

class Argument {
 public:
  enum class Type { Int, Float, Str, Path };

  Argument() : name_(""), type_(Type::Str), help_("") { InitializeRegex(); }

  ~Argument() {}

  Argument(const std::string &name, Argument::Type type,
           const std::string &help)
      : name_(name), type_(type), help_(help) {
    InitializeRegex();
  }

  void ReadArgument(const std::string &arg) {
    ValidateArg(arg, type_);
    value_ = arg;
  }

  std::string GetValue() { return value_; }

  std::string GetName() { return name_; }

  std::string GetHelp() { return help_; }

 private:
  std::map<Type, std::regex> regex_;
  const std::string &name_, help_;
  Argument::Type type_;
  std::string value_;

  void ValidateArg(const std::string &arg, Type type) {
    if (!std::regex_match(arg, regex_[type])) {
      throw std::invalid_argument("Incorrect value type " + arg +
                                  " of option " + name_);
    }
  }

  void InitializeRegex() {
    regex_[Type::Int] = std::regex("^[0-9]+$");
    regex_[Type::Float] = std::regex("^[0-9]+\\.[0-9]+$");
    regex_[Type::Str] = std::regex("^[a-zA-Z]+[0-9a-zA-z_-]*[a-zA-Z]*$");
    regex_[Type::Path] = std::regex(
        "^[\\w\\/]+[\\.\\\\/\\d\\w\\s\\+\\=\\#\\!\\@\\$\\(\\)\\:_-]*$");
  }
};

}  // namespace hhullen

#endif  //  SRC_ARGUMENT_ARGUMENT_H_
