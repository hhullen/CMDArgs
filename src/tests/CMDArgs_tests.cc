#include <gtest/gtest.h>

#include <string>
#include <vector>

#include "../cmd_args.h"

using hhullen::Argument;
using hhullen::CMDArgs;
using std::invalid_argument;
using std::string;
using std::vector;

TEST(cmd_args_test, constructor) { EXPECT_NO_THROW(hhullen::CMDArgs R); }

TEST(cmd_args_test, add_arguments_method) {
  hhullen::CMDArgs R;
  EXPECT_NO_THROW(R.AddArguments(
      {Argument("mode", Argument::Type::Int, "positional 1"),
       Argument("algotithm", Argument::Type::Str, "positional 2")}));
}

TEST(cmd_args_test, read_positional_method) {
  hhullen::CMDArgs R;
  EXPECT_NO_THROW(R.AddArguments(
      {Argument("mode", Argument::Type::Int, "positional 1"),
       Argument("algotithm", Argument::Type::Str, "positional 2")}));

  vector<const char*> argv({"utility", "1023", "SLE"});
  int argc = argv.size();
  EXPECT_NO_THROW(R.Read(argc, argv.data()));
}

TEST(cmd_args_test, read_positional_excess_method) {
  hhullen::CMDArgs R;
  EXPECT_NO_THROW(R.AddArguments(
      {Argument("mode", Argument::Type::Int, "positional 1"),
       Argument("algotithm", Argument::Type::Str, "positional 2")}));

  vector<const char*> argv({"utility", "1023", "SLE", "GOGO"});
  int argc = argv.size();
  EXPECT_THROW(R.Read(argc, argv.data()), invalid_argument);
}

TEST(cmd_args_test, read_positional_lack_method) {
  hhullen::CMDArgs R;
  EXPECT_NO_THROW(R.AddArguments(
      {Argument("mode", Argument::Type::Int, "positional 1"),
       Argument("algotithm", Argument::Type::Str, "positional 2")}));

  vector<const char*> argv({"utility", "1023"});
  int argc = argv.size();
  EXPECT_THROW(R.Read(argc, argv.data()), invalid_argument);
}

TEST(cmd_args_test, get_positional_method) {
  hhullen::CMDArgs R;
  EXPECT_NO_THROW(R.AddArguments(
      {Argument("mode", Argument::Type::Int, "positional 1"),
       Argument("algotithm", Argument::Type::Str, "positional 2")}));

  vector<const char*> argv({"utility", "1023", "SLE"});
  int argc = argv.size();
  R.Read(argc, argv.data());

  EXPECT_NO_THROW(R.GetArgument("algotithm"));
  EXPECT_NO_THROW(R.GetArgument("mode"));
  EXPECT_THROW(R.GetArgument("abobus"), invalid_argument);
}

TEST(cmd_args_test, get_positional_method) {
  // hhullen::CMDArgs R;
  // EXPECT_NO_THROW(R.AddArguments(
  //     {Argument("mode", Argument::Type::Int, "positional 1"),
  //      Argument("algotithm", Argument::Type::Str, "positional 2")}));

  // vector<const char*> argv({"utility", "1023", "SLE"});
  // int argc = argv.size();
  // R.Read(argc, argv.data());

  // EXPECT_NO_THROW(R.GetArgument("algotithm"));
  // EXPECT_NO_THROW(R.GetArgument("mode"));
  // EXPECT_THROW(R.GetArgument("abobus"), invalid_argument);
}