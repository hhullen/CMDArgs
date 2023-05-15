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
  Argument() {
    regex_[Type::Int] = std::regex("^[0-9]+$");
    regex_[Type::Float] = std::regex("^[0-9]+\\.[0-9]+$");
    regex_[Type::Str] = std::regex("^[a-zA-Z]+[0-9a-zA-z_-]*[a-zA-Z]*$");
  }
  ~Argument() {}
  // virtual void Read();

 private:
  std::map<std::string, std::list<std::string>> arguments_;
  std::map<Type, std::regex> regex_;

  void ValidateArg(const std::string &arg, const Type type) {
    if (!std::regex_match(arg, regex_[type])) {
      throw std::invalid_argument("Incorrect option " + arg);
    }
  }
};

}  // namespace hhullen

#endif  //  SRC_ARGUMENT_ARGUMENT_H_
