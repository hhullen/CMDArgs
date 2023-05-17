#ifndef SRC_CMD_ARGS_H_
#define SRC_CMD_ARGS_H_

#include <queue>

#include "arguments/argument.h"

namespace hhullen {

class CMDArgs {
 public:
  CMDArgs() {}
  ~CMDArgs() {}

  void AddArguments(const std::initializer_list<hhullen::Argument>& args) {
    for (hhullen::Argument arg : args) {
      positional_.insert({arg.GetName(), arg});
    }
  }

  std::string GetArgument(const std::string& name) {
    if (!IsArgExists(name)) {
      throw std::invalid_argument("Argument " + name + " not specified.");
    }
    return positional_[name].GetValue();
  }

  void Read(int argc, char* argv[]) {
    for (int i = 0; i < argc; ++i) {
      if (IsArgument(std::string(argv[i]))) {
        !!!
      }
    }
  }

 private:
  std::map<std::string, hhullen::Argument> positional_;
  //   std::map<std::string, Argument> optional_;

  bool IsArgExists(const std::string& name) {
    return positional_.find(name) != positional_.end();
  }

  bool IsArgument(const std::string& arg) {
    return arg.size() > 0 && arg[0] != '-';
  }
};

}  // namespace hhullen

#endif  //  SRC_CMD_ARGS_H_