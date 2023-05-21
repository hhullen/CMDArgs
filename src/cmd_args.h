#ifndef SRC_CMD_ARGS_H_
#define SRC_CMD_ARGS_H_

#include <queue>
#include <vector>

#include "arguments/argument.h"
#include "arguments/flag.h"

namespace hhullen {

class CMDArgs {
  using Tokens = std::queue<std::string>;
  using FlagsStruct =
      std::map<std::pair<std::string, std::string>, hhullen::Flag>;
  using FlagsStructElement =
      std::pair<std::pair<std::string, std::string>, hhullen::Flag>;
  using ArgumentsStruct = std::queue<hhullen::Argument>;

  using ParsedArguments = std::map<std::string, std::string>;
  using ParsedFlags =
      std::map<std::pair<std::string, char>, std::list<std::string>>;

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
      std::string name_short = std::string("-") + flag.GetShortName();
      flags_.insert({{name_long, name_short}, flag});
    }
  }

  std::string GetArgument(const std::string& name) {
    if (!IsArgExists(name)) {
      throw std::invalid_argument(name + " was not specified.");
    }
    return positional_[name];
  }

  void Read(int argc, const char* argv[]) {
    CopyToThis(argc, argv);
    for (; tokens_.size() > 0;) {
      std::string token = tokens_.front();
      if (hhullen::Argument::IsArgument(token) && !arguments_.empty()) {
        hhullen::Argument argument = arguments_.front();
        ReadArgumentFromToken(argument, token);
        arguments_.pop();
        tokens_.pop();
      } else if (hhullen::Flag::IsFlag(token)) {
        hhullen::Flag flag = GetFlagFromToken(token);
        ReadFlag(flag);
      } else {
        throw std::invalid_argument("Unknown argument " + token +
                                    " specified.");
      }
    }
    CheckRemainsArguments();
  }

 private:
  ParsedArguments positional_;
  ParsedFlags optional_;

  ArgumentsStruct arguments_;
  FlagsStruct flags_;

  Tokens tokens_;
  std::string search_token_;

  std::function<bool(const FlagsStructElement&)> search_function =
      [this](const FlagsStructElement& element) {
        return element.first.first == search_token_ ||
               element.first.second == search_token_;
      };

  void CopyToThis(int argc, const char* argv[]) {
    for (int i = 1; i < argc; ++i) {
      tokens_.push(argv[i]);
    }
  }

  bool IsArgExists(const std::string& name) {
    return positional_.find(name) != positional_.end();
  }

  hhullen::Flag GetFlagFromToken(const std::string& token) {
    search_token_ = token;
    auto iter = std::find_if(flags_.begin(), flags_.end(), search_function);
    search_token_.clear();

    if (iter == flags_.end()) {
      throw std::invalid_argument("Unknown flag " + token + " specified.");
    }
    return (*iter).second;
  }

  void ReadFlag(hhullen::Flag& flag) {
    const std::list<hhullen::Argument>& arguments = flag.GetArguments();
    auto iter = arguments.begin();
    tokens_.pop();

    for (; !tokens_.empty() && iter != arguments.end(); tokens_.pop()) {
      hhullen::Argument argument = *iter;
      std::string token = tokens_.front();

      argument.ReadArgument(token);
      std::string value = argument.GetValue();
      // CheckNameUniquenessInContainer(name, optional_);
      std::string name_long = flag.GetLongName();
      char name_short = flag.GetShortName();
      optional_[{name_long, name_short}].push_back(value);
      ++iter;
    }
  }

  void ReadArgumentFromToken(hhullen::Argument& argument,
                             const std::string& token) {
    argument.ReadArgument(token);
    std::string value = argument.GetValue();
    std::string name = argument.GetName();
    CheckNameUniquenessInContainer(name, positional_);
    positional_[name] = value;
  }

  void CheckRemainsArguments() {
    if (!arguments_.empty()) {
      std::string name = arguments_.front().GetName();
      throw std::invalid_argument(name + " was not specified.");
    }
  }

  template <class Container>
  void CheckNameUniquenessInContainer(const std::string& name,
                                      const Container& container) {
    if (container.find(name) != container.end()) {
      throw std::invalid_argument("Name " + name + " already read.");
    }
  }
};

}  // namespace hhullen

#endif  //  SRC_CMD_ARGS_H_
