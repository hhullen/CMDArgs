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
      std::string name_long = "--" + flag.GetLongName();
      std::string name_short = "-" + flag.GetShortName();
      flags_.insert({{name_long, name_short}, flag});
    }
  }

  std::string GetArgument(const std::string& name) {
    if (!IsArgExists(name)) {
      throw std::invalid_argument("Argument " + name + " not specified.");
    }
    return positional_[name];
  }

  void Read(int argc, const char* argv[]) {
    CopyToThis(argc, argv);

    for (; !tokens_.empty(); tokens_.pop()) {
      std::string token = tokens_.front();
      tokens_.pop();
      if (hhullen::Argument::IsArgument(token) && !arguments_.empty()) {
        hhullen::Argument argument = arguments_.front();
        ReadArgumentFromToken(argument, token);
        arguments_.pop();
      } else if (hhullen::Flag::IsFlag(token)) {
        hhullen::Flag flag = GetFlag(token);
        ReadFlag(flag);
      } else {
        throw std::invalid_argument("Unknown argument " + token +
                                    " specified.");
      }
    }
    CheckRemainsArguments();
  }

 private:
  std::map<std::string, std::string> positional_;
  std::map<std::pair<std::string, char>, std::list<std::string>> optional_;

  std::queue<hhullen::Argument> arguments_;
  std::map<std::pair<std::string, std::string>, hhullen::Flag> flags_;

  std::queue<std::string> tokens_;

  void CopyToThis(int argc, const char* argv[]) {
    for (int i = 1; i < argc; ++i) {
      tokens_.push(argv[i]);
    }
  }

  bool IsArgExists(const std::string& name) {
    return positional_.find(name) != positional_.end();
  }

  hhullen::Flag GetFlag(const std::string& token) {
    for (std::pair<std::pair<std::string, std::string>, hhullen::Flag> flag :
         flags_) {
      std::pair<std::string, std::string> names = flag.first;
      if (names.first == token || names.second == token) {
        return flag.second;
      }
    }
    throw std::invalid_argument("Unknown flag " + token + " specified.");
  }

  void ReadFlag(hhullen::Flag& flag) {
    const std::list<hhullen::Argument>& arguments = flag.GetArguments();
    auto iter = arguments.begin();
    for (; !tokens_.empty() && iter != arguments.end(); tokens_.pop()) {
      hhullen::Argument argument = *iter;
      std::string token = tokens_.front();
      tokens_.pop();

      argument.ReadArgument(token);
      // CheckNameUniquenessInContainer(name, optional_);
      optional_[{flag.GetLongName(), flag.GetShortName()}].push_back(token);
      ++iter;
    }
  }

  void ReadArgumentFromToken(hhullen::Argument& argument,
                             const std::string& token) {
    argument.ReadArgument(token);
    std::string value = argument.GetValue();
    CheckNameUniquenessInContainer(token, positional_);
    positional_[token] = value;
  }

  void CheckRemainsArguments() {
    if (!arguments_.empty()) {
      std::string name = arguments_.front().GetName();
      throw std::invalid_argument("Argument " + name + " not specified.");
    }
  }

  void CheckNameUniquenessInContainer(
      const std::string& name,
      const std::map<std::string, std::string>& container) {
    if (container.find(name) != container.end()) {
      throw std::invalid_argument("Name " + name + " already read.");
    }
  }
};

}  // namespace hhullen

#endif  //  SRC_CMD_ARGS_H_
