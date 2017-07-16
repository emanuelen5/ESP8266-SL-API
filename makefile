CC=gcc
CFLAGS=-Wall -std=c99

# Regular compilation
OUTPUT=xml_parser
SRCS=$(OUTPUT).c
DEPS=$(SRCS:.c=.h)
OBJS=$(SRCS:.c=.o)
IDIR=

$(OUTPUT): $(OBJS)
	$(CC) $(CFLAGS) -o $(OUTPUT) $(OBJS)

$(OBJS): $(DEPS)
	$(CC) $(CFLAGS) -o $@ -c $(SRCS)



# Compilation of tests
TEST_OUTPUT=test_runner
TEST_OBJS=test_xml_parser.o
TEST_SRCS=$(TEST_OBJS:.o=.c)
UNITY=include/Unity/unity.c include/Unity_fixture/unity_fixture.c
TEST_INCL=-I"include/Unity" -I"include/Unity_fixture"

$(TEST_OUTPUT):  $(TEST_SRCS) $(UNITY) $(OBJS)
	$(CC) $(CFLAGS) -o $@ $< $(UNITY) $(TEST_INCL)

test: $(TEST_OUTPUT)
	@echo "Running tests"
	@echo ""
	./$(TEST_OUTPUT)

# Miscellaneous
.DEFAULT_GOAL=all
.PHONY=all clean test
all: $(OUTPUT) $(TEST_OUTPUT)
clean:
	@rm -vf $(OBJS) $(OUTPUT) $(TEST_OBJS) $(TEST_OUTPUT)