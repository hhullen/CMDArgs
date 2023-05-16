#ifndef SRC_ARGUMENT_ARGUMENT_H_
#define SRC_ARGUMENT_ARGUMENT_H_

#include <list>
#include <map>
#include <regex>
#include <string>

namespace hhullen {

class Argument {
 public:
  enum class Type { Int, Float, Str };
  Argument(const std::string &name, Argument::Type type,
           const std::string &help)
      : name_(name), type_(type_), help_(help) {
    regex_[Type::Int] = std::regex("^[0-9]+$");
    regex_[Type::Float] = std::regex("^[0-9]+\\.[0-9]+$");
    regex_[Type::Str] = std::regex("^[a-zA-Z]+[0-9a-zA-z_-]*[a-zA-Z]*$");
  }

  void Read(int argc, char *argv[]) {}

 private:
  std::map<Type, std::regex> regex_;
  const std::string &name_, help_;
  Argument::Type type_;

  void ValidateArg(const std::string &arg, const Type type) {
    if (!std::regex_match(arg, regex_[type])) {
      throw std::invalid_argument("Incorrect option type " + arg);
    }
  }
};

}  // namespace hhullen

#endif  //  SRC_ARGUMENT_ARGUMENT_H_
