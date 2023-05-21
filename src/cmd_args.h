#ifndef SRC_CMD_ARGS_H_
#define SRC_CMD_ARGS_H_

#include <queue>

#include "arguments/argument.h"
#include "arguments/flag.h"

namespace hhullen {

class CMDArgs {
  using Argument = hhullen::Argument;
  using Flag = hhullen::Flag;
  using Str = std::string;
  using StrStrPair = std::pair<Str, Str>;
  using Tokens = std::queue<Str>;

  using ArgumentsStruct = std::queue<Argument>;
  using ParsedArguments = std::map<Str, Str>;
  using ArgumentsListIterator = std::list<Argument>::const_iterator;

  using FlagsStruct = std::map<StrStrPair, Flag>;
  using FlagsStructElement = std::pair<StrStrPair, Flag>;
  using ParsedFlags = std::map<StrStrPair, std::list<Str>>;
  using ParsedFlagsElement = std::pair<StrStrPair, std::list<Str>>;
  using FlagsStructIterator = FlagsStruct::iterator;
  using ParsedFlagsIterator = ParsedFlags::iterator;

 public:
  CMDArgs() {
    class_flag_search_func = [this](const FlagsStructElement& element) {
      return element.first.first == search_token_ ||
             element.first.second == search_token_;
    };
    parsed_flag_search_func = [this](const ParsedFlagsElement& element) {
      return element.first.first == search_token_ ||
             element.first.second == search_token_;
    };
  }
  ~CMDArgs() {}

  void AddArguments(const std::initializer_list<Argument>& args) {
    for (Argument arg : args) {
      arguments_.push(arg);
    }
  }

  void AddFlags(const std::initializer_list<Flag>& flags) {
    for (Flag flag : flags) {
      Str name_long = Str("--") + flag.GetLongName();
      Str name_short = Str("-") + flag.GetShortName();
      flags_.insert({{name_long, name_short}, flag});
    }
  }

  Str GetArgument(const Str& name) {
    if (!IsArgExists(name)) {
      throw std::invalid_argument("\"" + name + "\" was not specified.");
    }
    return positional_[name];
  }

  std::list<Str> GetFlagValues(const Str& name) {
    search_token_ = name;
    ParsedFlagsIterator iter = std::find_if(optional_.begin(), optional_.end(),
                                            parsed_flag_search_func);
    search_token_.clear();
    if (iter == optional_.end()) {
      throw std::invalid_argument("\"" + name + " was not specified.");
    }
    return (*iter).second;
  }

  void Read(int argc, const char* argv[]) {
    CopyToThis(argc, argv);
    for (; !tokens_.empty();) {
      Str token = tokens_.front();
      if (Argument::IsArgument(token) && !arguments_.empty()) {
        Argument argument = arguments_.front();
        ReadArgumentFromToken(argument, token);
        arguments_.pop();
        tokens_.pop();
      } else if (Flag::IsFlag(token)) {
        Flag flag = GetFlagFromToken(token);
        ReadFlag(flag);
      } else {
        throw std::invalid_argument("Unknown argument \"" + token +
                                    "\" specified.");
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
  Str search_token_;

  std::function<bool(const FlagsStructElement&)> class_flag_search_func;
  std::function<bool(const ParsedFlagsElement&)> parsed_flag_search_func;

  void CopyToThis(int argc, const char* argv[]) {
    for (int i = 1; i < argc; ++i) {
      tokens_.push(argv[i]);
    }
  }

  bool IsArgExists(const Str& name) {
    return positional_.find(name) != positional_.end();
  }

  Flag GetFlagFromToken(const Str& token) {
    search_token_ = token;
    FlagsStructIterator iter =
        std::find_if(flags_.begin(), flags_.end(), class_flag_search_func);
    search_token_.clear();

    if (iter == flags_.end()) {
      throw std::invalid_argument("Unknown flag \"" + token + "\" specified.");
    }
    return (*iter).second;
  }

  void ReadFlag(Flag& flag) {
    const std::list<Argument>& arguments = flag.GetArguments();
    ArgumentsListIterator iter = arguments.begin();
    tokens_.pop();

    CheckFlagValuesAbsence(flag);
    for (; !tokens_.empty() && iter != arguments.end(); tokens_.pop()) {
      Argument argument = *iter;
      Str token = tokens_.front();
      argument.ReadArgument(token);

      Str value = argument.GetValue();
      SetParsedValueForFlag(value, flag);
      ++iter;
    }

    if (iter != arguments.end()) {
      Argument argument = *iter;
      throw std::invalid_argument("Value \"" + argument.GetName() +
                                  "\" of flag [--" + flag.GetLongName() + " -" +
                                  flag.GetShortName() + "] was not specified.");
    }
  }

  void CheckFlagValuesAbsence(Flag& flag) {
    const std::list<Argument>& arguments = flag.GetArguments();
    if (arguments.empty()) {
      SetParsedValueForFlag("true", flag);
    }
  }

  void SetParsedValueForFlag(const Str& value, Flag& flag) {
    Str name_long = Str("--") + flag.GetLongName();
    Str name_short = Str("-") + flag.GetShortName();
    // CheckNameUniquenessInContainer(name, optional_); !!!!!!!
    optional_[{name_long, name_short}].push_back(value);
  }

  void ReadArgumentFromToken(Argument& argument, const Str& token) {
    argument.ReadArgument(token);
    Str value = argument.GetValue();
    Str name = argument.GetName();
    if (positional_.find(name) != positional_.end()) {
      throw std::invalid_argument("Name \"" + name + "\" already read.");
    }
    positional_[name] = value;
  }

  void CheckRemainsArguments() {
    if (!arguments_.empty()) {
      Str name = arguments_.front().GetName();
      throw std::invalid_argument("\"" + name + " was not specified.");
    }
  }
};

}  // namespace hhullen

#endif  //  SRC_CMD_ARGS_H_
