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
  Flag() : regex_("(^-\\w$)|(^--\\w+$)") {}

 private:
  std::regex regex_;
  std::list<hhullen::Argument> values_;
};

}  // namespace hhullen

#endif  //  SRC_ARGUMENT_FLAG_H_
