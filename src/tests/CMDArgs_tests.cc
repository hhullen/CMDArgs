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
       Argument("algotithm", Argument::Type::String, "positional 2")}));
}

TEST(cmd_args_test, read_positional_method) {
  hhullen::CMDArgs R;
  EXPECT_NO_THROW(R.AddArguments(
      {Argument("mode", Argument::Type::Int, "positional 1"),
       Argument("algotithm", Argument::Type::String, "positional 2")}));

  vector<const char*> argv({"utility", "1023", "SLE"});
  int argc = static_cast<int>(argv.size());
  EXPECT_NO_THROW(R.Read(argc, argv.data()));
}

TEST(cmd_args_test, read_incorrect_typed_positional_method) {
  hhullen::CMDArgs R;
  EXPECT_NO_THROW(R.AddArguments(
      {Argument("mode", Argument::Type::String, "positional 1"),
       Argument("algotithm", Argument::Type::String, "positional 2")}));

  vector<const char*> argv({"utility", "1023", "SLE"});
  int argc = static_cast<int>(argv.size());
  EXPECT_THROW(R.Read(argc, argv.data()), invalid_argument);
}

TEST(cmd_args_test, read_positional_excess_method) {
  hhullen::CMDArgs R;
  EXPECT_NO_THROW(R.AddArguments(
      {Argument("mode", Argument::Type::Int, "positional 1"),
       Argument("algotithm", Argument::Type::String, "positional 2")}));

  vector<const char*> argv({"utility", "1023", "SLE", "GOGO"});
  int argc = static_cast<int>(argv.size());
  EXPECT_THROW(R.Read(argc, argv.data()), invalid_argument);
}

TEST(cmd_args_test, read_positional_lack_method) {
  hhullen::CMDArgs R;
  EXPECT_NO_THROW(R.AddArguments(
      {Argument("mode", Argument::Type::Int, "positional 1"),
       Argument("algotithm", Argument::Type::String, "positional 2")}));

  vector<const char*> argv({"utility", "1023"});
  int argc = static_cast<int>(argv.size());
  EXPECT_THROW(R.Read(argc, argv.data()), invalid_argument);
}

TEST(cmd_args_test, read_doble_definition_method) {
  hhullen::CMDArgs R;
  EXPECT_NO_THROW(R.AddArguments(
      {Argument("algotithm", Argument::Type::Int, "positional 1"),
       Argument("algotithm", Argument::Type::String, "positional 2")}));

  vector<const char*> argv({"utility", "1023", "GFG"});
  int argc = static_cast<int>(argv.size());
  EXPECT_THROW(R.Read(argc, argv.data()), invalid_argument);
}

TEST(cmd_args_test, get_positional_method) {
  hhullen::CMDArgs R;
  EXPECT_NO_THROW(R.AddArguments(
      {Argument("mode", Argument::Type::Int, "positional 1"),
       Argument("algotithm", Argument::Type::String, "positional 2")}));

  vector<const char*> argv({"utility", "1023", "SLE"});
  int argc = static_cast<int>(argv.size());
  R.Read(argc, argv.data());

  EXPECT_NO_THROW(R.GetArgument("algotithm"));
  EXPECT_NO_THROW(R.GetArgument("mode"));
  EXPECT_THROW(R.GetArgument("abobus"), invalid_argument);
}

TEST(cmd_args_test, add_flag_method) {
  hhullen::CMDArgs R;
  EXPECT_NO_THROW(R.AddArguments(
      {Argument("mode", Argument::Type::Int, "positional 1"),
       Argument("algotithm", Argument::Type::String, "positional 2")}));

  hhullen::Flag file_f = hhullen::Flag(
      "file", 'f', "flag-1",
      {Argument("path_to_file", Argument::Type::Path, "optional 1")});

  EXPECT_NO_THROW(R.AddFlags({file_f}));

  vector<const char*> argv({"utility", "1023", "SLE"});
  int argc = static_cast<int>(argv.size());
  R.Read(argc, argv.data());

  EXPECT_NO_THROW(R.GetArgument("algotithm"));
  EXPECT_NO_THROW(R.GetArgument("mode"));
  EXPECT_THROW(R.GetArgument("abobus"), invalid_argument);
}

TEST(cmd_args_test, add_no_arged_flag_method) {
  hhullen::CMDArgs R;
  EXPECT_NO_THROW(R.AddArguments(
      {Argument("mode", Argument::Type::Int, "positional 1"),
       Argument("algotithm", Argument::Type::String, "positional 2")}));

  hhullen::Flag file_f = hhullen::Flag("bool", 'b', "flag-1", {});

  EXPECT_NO_THROW(R.AddFlags({file_f}));

  vector<const char*> argv({"utility", "1023", "SLE", "-b"});
  int argc = static_cast<int>(argv.size());
  R.Read(argc, argv.data());

  EXPECT_NO_THROW(R.GetArgument("algotithm"));
  EXPECT_NO_THROW(R.GetArgument("mode"));
  EXPECT_NO_THROW(R.GetFlagValues("--bool").front() == "true");
  EXPECT_THROW(R.GetArgument("abobus"), invalid_argument);
}

TEST(cmd_args_test, read_flag_didnot_set_method) {
  hhullen::CMDArgs R;
  EXPECT_NO_THROW(R.AddArguments(
      {Argument("mode", Argument::Type::Int, "positional 1"),
       Argument("algotithm", Argument::Type::String, "positional 2")}));

  hhullen::Flag file_f = hhullen::Flag("bool", 'b', "flag-1", {});

  EXPECT_NO_THROW(R.AddFlags({file_f}));

  vector<const char*> argv({"utility", "1023", "SLE", "-b"});
  int argc = static_cast<int>(argv.size());
  R.Read(argc, argv.data());

  EXPECT_NO_THROW(R.GetArgument("algotithm"));
  EXPECT_NO_THROW(R.GetArgument("mode"));
  EXPECT_THROW(R.GetFlagValues("--avadakedabra"), invalid_argument);
  EXPECT_THROW(R.GetArgument("abobus"), invalid_argument);
}

TEST(cmd_args_test, read_flag_method) {
  hhullen::CMDArgs R;
  EXPECT_NO_THROW(R.AddArguments(
      {Argument("mode", Argument::Type::Int, "positional 1"),
       Argument("algotithm", Argument::Type::String, "positional 2")}));

  hhullen::Flag file_f = hhullen::Flag(
      "file", 'f', "flag-1",
      {Argument("path_to_file", Argument::Type::Path, "optional 1")});

  R.AddFlags({file_f});

  vector<const char*> argv({"utility", "1023", "-f", "/file/f.txt", "SLE"});
  int argc = static_cast<int>(argv.size());
  R.Read(argc, argv.data());

  EXPECT_NO_THROW(R.GetArgument("algotithm"));
  EXPECT_NO_THROW(R.GetArgument("mode"));
  EXPECT_NO_THROW(R.GetFlagValues("--file"));
  EXPECT_THROW(R.GetArgument("abobus"), invalid_argument);
}

TEST(cmd_args_test, read_no_value_flag_method) {
  hhullen::CMDArgs R;
  EXPECT_NO_THROW(R.AddArguments(
      {Argument("mode", Argument::Type::Int, "positional 1"),
       Argument("algotithm", Argument::Type::String, "positional 2")}));

  hhullen::Flag file_f = hhullen::Flag(
      "file", 'f', "flag-1",
      {Argument("path_to_file", Argument::Type::Path, "optional 1")});

  R.AddFlags({file_f});

  vector<const char*> argv({"utility", "1023", "SLE", "-f"});
  int argc = static_cast<int>(argv.size());
  EXPECT_THROW(R.Read(argc, argv.data()), invalid_argument);

  EXPECT_NO_THROW(R.GetArgument("algotithm"));
  EXPECT_NO_THROW(R.GetArgument("mode"));
  EXPECT_THROW(R.GetFlagValues("--file"), invalid_argument);
  EXPECT_THROW(R.GetArgument("abobus"), invalid_argument);
}

TEST(cmd_args_test, read_two_flags_method) {
  hhullen::CMDArgs R;
  EXPECT_NO_THROW(R.AddArguments(
      {Argument("mode", Argument::Type::Int, "positional 1"),
       Argument("algotithm", Argument::Type::String, "positional 2")}));

  hhullen::Flag file_f = hhullen::Flag(
      "file", 'f', "flag-1",
      {Argument("path_to_file", Argument::Type::Path, "optional 1")});

  hhullen::Flag god_f =
      hhullen::Flag("god-mode", 'g', "flag-1",
                    {Argument("god-mode", Argument::Type::Path, "optional 2")});

  R.AddFlags({file_f, god_f});

  vector<const char*> argv(
      {"utility", "1023", "SLE", "-f", "/file", "-g", "./another/one"});
  int argc = static_cast<int>(argv.size());
  R.Read(argc, argv.data());

  EXPECT_NO_THROW(R.GetArgument("algotithm"));
  EXPECT_NO_THROW(R.GetArgument("mode"));
  EXPECT_NO_THROW(R.GetFlagValues("-f"));
  EXPECT_NO_THROW(R.GetFlagValues("--god-mode"));
  EXPECT_THROW(R.GetArgument("abobus"), invalid_argument);
}

TEST(cmd_args_test, read_two_eq_named_flags_method) {
  hhullen::CMDArgs R;
  EXPECT_NO_THROW(R.AddArguments(
      {Argument("mode", Argument::Type::Int, "positional 1"),
       Argument("algotithm", Argument::Type::String, "positional 2")}));

  hhullen::Flag file_f = hhullen::Flag(
      "file", 'f', "flag-1",
      {Argument("path_to_file", Argument::Type::Path, "optional 1")});

  hhullen::Flag god_f =
      hhullen::Flag("file", 'g', "flag-1",
                    {Argument("god-mode", Argument::Type::Path, "optional 2")});

  R.AddFlags({file_f, god_f});

  vector<const char*> argv(
      {"utility", "1023", "SLE", "-f", "/file", "-g", "./another/one"});
  int argc = static_cast<int>(argv.size());
  EXPECT_THROW(R.Read(argc, argv.data()), invalid_argument);

  EXPECT_NO_THROW(R.GetArgument("algotithm"));
  EXPECT_NO_THROW(R.GetArgument("mode"));
  EXPECT_NO_THROW(R.GetFlagValues("-f"));
  EXPECT_THROW(R.GetFlagValues("--god-mode"), invalid_argument);
  EXPECT_THROW(R.GetArgument("abobus"), invalid_argument);
}

TEST(cmd_args_test, read_incorrect_typed_flags_method) {
  hhullen::CMDArgs R;
  EXPECT_NO_THROW(R.AddArguments(
      {Argument("mode", Argument::Type::Int, "positional 1"),
       Argument("algotithm", Argument::Type::String, "positional 2")}));

  hhullen::Flag file_f = hhullen::Flag(
      "file", 'f', "flag-1",
      {Argument("path_to_file", Argument::Type::Int, "optional 1")});

  hhullen::Flag god_f =
      hhullen::Flag("file", 'g', "flag-1",
                    {Argument("god-mode", Argument::Type::Int, "optional 2")});

  R.AddFlags({file_f, god_f});

  vector<const char*> argv(
      {"utility", "1023", "SLE", "-f", "/file", "-g", "./another/one"});
  int argc = static_cast<int>(argv.size());
  EXPECT_THROW(R.Read(argc, argv.data()), invalid_argument);

  EXPECT_NO_THROW(R.GetArgument("algotithm"));
  EXPECT_NO_THROW(R.GetArgument("mode"));
  EXPECT_THROW(R.GetFlagValues("-f"), invalid_argument);
  EXPECT_THROW(R.GetFlagValues("--god-mode"), invalid_argument);
  EXPECT_THROW(R.GetArgument("abobus"), invalid_argument);
}
