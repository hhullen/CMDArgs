#ifndef SRC_CMD_ARGS_H_
#define SRC_CMD_ARGS_H_

#include <queue>
#include <vector>

#include "arguments/argument.h"
#include "arguments/flag.h"

namespace hhullen {

class CMDArgs {
 public:
  CMDArgs() {}
  ~CMDArgs() {}

  void AddArguments(const std::initializer_list<hhullen::Argument>& args) {
    for (hhullen::Argument arg : args) {
      arguments_.push(arg);
    }
  }

  void AddFlags(const std::initializer_list<hhullen::Flag>& flags) {
    for (hhullen::Flag flag : flags) {
      .push(flag);
    }
  }

  std::string GetArgument(const std::string& name) {
    if (!IsArgExists(name)) {
      throw std::invalid_argument("Argument " + name + " not specified.");
    }
    return positional_[name];
  }

  void Read(int argc, const char* argv[]) {
    for (int i = 1; i < argc; ++i) {
      std::string token = argv[i];
      if (IsArgument(token) && !arguments_.empty()) {
        hhullen::Argument argument = arguments_.front();
        ReadArgumentFromToken(argument, token);
        arguments_.pop();
      } else if (IsFlag(token)) {
      } else {
        throw std::invalid_argument("Unknown argument " + token +
                                    " specified.");
      }
    }
    CheckRemainsArguments();
  }

 private:
  std::map<std::string, std::string> positional_;
  //   std::map<std::string, Argument> optional_;

  std::queue<hhullen::Argument> arguments_;
  std::map<std::string, hhullen::Flag> flags_;
  ? ? ? ?

        bool
        IsArgExists(const std::string& name) {
    return positional_.find(name) != positional_.end();
  }

  bool IsArgument(const std::string& arg) {
    return arg.size() > 0 && arg[0] != '-';
  }

  bool IsFlag(const std::string& arg) { return false; }

  void ReadArgumentFromToken(hhullen::Argument& argument,
                             const std::string& token) {
    argument.ReadArgument(token);
    std::string name = argument.GetName();
    std::string value = argument.GetValue();
    positional_[name] = value;
  }

  void CheckRemainsArguments() {
    if (!arguments_.empty()) {
      std::string name = arguments_.front().GetName();
      throw std::invalid_argument("Argument " + name + " not specified.");
    }
  }
};

}  // namespace hhullen

#endif  //  SRC_CMD_ARGS_H_
