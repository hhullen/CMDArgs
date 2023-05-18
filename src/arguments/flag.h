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
  Flag() : long_name_(""), short_name_('\0'), regex_("(^-\\w$)|(^--\\w+$)") {}

  Flag(const std::string& long_name, const char short_name,
       const std::string& help,
       const std::initializer_list<hhullen::Argument>& args)
      : values_(args),
        long_name_(long_name),
        short_name_(short_name),
        regex_("(^-\\w$)|(^--\\w+$)") {}

 private:
  std::list<hhullen::Argument> values_;
  const std::string& long_name_;
  const char short_name_;
  std::regex regex_;
};

}  // namespace hhullen

#endif  //  SRC_ARGUMENT_FLAG_H_
