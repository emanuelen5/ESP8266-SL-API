CC=gcc
CFLAGS=-Wall -std=c99

ifeq ($(OS),Windows_NT)
	EXE_EXT=.exe
else
	EXE_EXT=.out
endif

# Regular compilation
NAME=xml_parser
EXE=$(NAME)$(EXE_EXT)
SRCS=$(NAME).c
DEPS=$(NAME).h
OBJS=$(NAME).o
IDIR=

$(EXE): $(OBJS)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJS)

$(OBJS): $(DEPS)
	$(CC) $(CFLAGS) -o $@ -c $(SRCS)



# Compilation of tests
TEST_NAME=test_xml_parser
TEST_EXE=test_runner$(EXE_EXT)
TEST_OBJS=$(TEST_NAME).o
TEST_SRCS=$(TEST_NAME).c
UNITY=include/Unity/unity.c include/Unity_fixture/unity_fixture.c
TEST_INCL=-I"include/Unity" -I"include/Unity_fixture"

$(TEST_EXE):  $(TEST_SRCS) $(UNITY) $(OBJS)
	$(CC) $(CFLAGS) -o $@ $< $(UNITY) $(TEST_INCL)
	@$(MAKE) test

test: $(TEST_EXE)
	@echo "Running tests"
	@echo ""
	- ./$(TEST_EXE)

# Miscellaneous
.DEFAULT_GOAL=all
.PHONY=all clean test
all: $(EXE) $(TEST_EXE)
clean:
	@rm -vf $(OBJS) $(EXE) $(TEST_OBJS) $(TEST_EXE)