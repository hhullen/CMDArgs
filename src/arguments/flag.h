#ifndef SRC_ARGUMENT_FLAG_H_
#define SRC_ARGUMENT_FLAG_H_

#include <list>
#include <map>
#include <regex>
#include <string>

#include "argument.h"

namespace hhullen {

class Flag {
 public:
  Flag() : long_name_(""), short_name_('\0') {}

  Flag(const std::string& long_name, const char short_name,
       const std::string& help,
       const std::initializer_list<hhullen::Argument>& args)
      : arguments_(args), long_name_(long_name), short_name_(short_name) {}

  std::string GetLongName() { return long_name_; }

  char GetShortName() { return short_name_; }

  const std::list<hhullen::Argument>& GetArguments() { return arguments_; }

  static bool IsFlag(const std::string& token) {
    return std::regex_match(token, std::regex("(^-\\w$)|(^--\\w+$)"));
  }

 private:
  std::list<hhullen::Argument> arguments_;
  const std::string& long_name_;
  const char short_name_;
};

}  // namespace hhullen

#endif  //  SRC_ARGUMENT_FLAG_H_
