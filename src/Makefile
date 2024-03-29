MAIN_PROJ_NAME=CMDArgs
TEST_DIR=tests
ARGUMENTS_DIR=arguments
ARGUMENTS_BUILD_FILES=cmd_args.h
TESTBUILD_FILES=$(TEST_DIR)$(SEP)$(MAIN_PROJ_NAME)_tests.cc
TEST_EXECUTABLE=$(MAIN_PROJ_NAME)_test.out
COMPILER=g++
STD=--std=c++17
CODE_DIAG_FLAGS=-Wall -Wextra -Werror -Wpedantic -Wshadow -Wconversion -Wnull-dereference -Wsign-conversion
TEST_FLAGS=-lgtest -pthread -lgtest_main
CLANG_FILE=.clang-format
CLANG_FILE_WAY=..$(SEP)materials$(SEP)linters$(SEP)$(CLANG_FILE)
CHECK_FILES= $(ARGUMENTS_DIR)$(SEP)*.h \
			$(ARGUMENTS_BUILD_FILES)
CPPCH_SETUP=--enable=all --suppress=missingInclude \
			--suppress=unusedFunction --suppress=noExplicitConstructor -v --language=c++ $(STD)
BUILD_DIR=..$(SEP)build
TO_DELETE_FILES=*.o *.out *.dSYM *.gch *.gcda *.gcno .DS_Store $(TEST_EXECUTABLE) \
				$(CLANG_FILE) *.info
TO_DELETE_FOLDERS=$(BUILD_DIR) report a.out.dSYM cmd_args

#Crossplatform specs
SEP='\'
MAKEDIR=md
DELETE_FOLDER=RD /S/Q
DELETE_FILE=DEL /F
COPY=copy -Recurse
OPEN=start
OS=$(shell uname)

ifeq ($(OS), Linux)
	OPEN=xdg-open
	MAKEDIR=mkdir -p
	SEP=/
	DELETE_FOLDER=rm -rf
	DELETE_FILE=rm -f
	COPY=cp -r
endif

ifeq ($(OS), Darwin)
	OPEN=open
	MAKEDIR=mkdir -p
	SEP=/
	DELETE_FOLDER=rm -rf
	DELETE_FILE=rm -f
	COPY=cp -r
endif

.PHONY: all check tests clean lib

all: clean gcov_report tests check

check:
	cppcheck $(CPPCH_SETUP) $(CHECK_FILES)
	$(COPY) $(CLANG_FILE_WAY) $(CLANG_FILE)
	clang-format -i --style=Google $(CHECK_FILES)
	clang-format -n --style=Google $(CHECK_FILES)

tests:
	$(COMPILER) $(STD) $(CODE_DIAG_FLAGS) $(TESTBUILD_FILES) $(TEST_FLAGS) \
	-o $(TEST_EXECUTABLE)
	.$(SEP)$(TEST_EXECUTABLE)

gcov_report: clean gcov_build
	lcov --ignore-errors mismatch -t "test" -o test.info -c -d .
	genhtml --ignore-errors mismatch -o report test.info

gcov_build:
	$(COMPILER) $(STD) $(CODE_DIAG_FLAGS) \
	$(TESTBUILD_FILES) $(TEST_FLAGS) \
	-o $(TEST_EXECUTABLE) --coverage
	.$(SEP)$(TEST_EXECUTABLE)

lib:
	$(MAKEDIR) cmd_args
	$(COPY) $(ARGUMENTS_DIR) cmd_args$(SEP)
	$(COPY) cmd_args.h cmd_args$(SEP)
clean:
	$(DELETE_FOLDER) $(TO_DELETE_FOLDERS)
	$(DELETE_FILE) $(TO_DELETE_FILES)
