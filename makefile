CC=gcc
CFLAGS=-Wall -std=c99

ifeq ($(OS),Windows_NT)
	EXE_EXT=exe
else
	EXE_EXT=out
endif

# Paths must be specified before they are resolved: Bug?
BUILD_PATH=output
INCLUDE_PATH=include

## General compilation of dependencies unless further specified ##
# %.o: 
	# Fix: Otherwise, test.o etc. matches the rule below
	# @echo "Strange, $@ triggered this rule"
$(BUILD_PATH)/%.o: %.c
	@echo "Compiling dependency"
	$(CC) $(CFLAGS) -o $@ $< $(TDD_INCL) -c


## Compilation of test dependencies ##
TDD_OBJS=$(BUILD_PATH)/unity.o $(BUILD_PATH)/unity_fixture.o
TDD_INCL=-I"include/unity" -I"include/unity_fixture"
$(BUILD_PATH)/%.o: $(INCLUDE_PATH)/unity/%.c
	@echo "Compiling Unity"
	$(CC) $(CFLAGS) -o $@ $< $(TDD_INCL) -c
$(BUILD_PATH)/%.o: $(INCLUDE_PATH)/unity_fixture/%.c $(BUILD_PATH)/unity.o
	@echo "Compiling Unity fixture"
	$(CC) $(CFLAGS) -o $@ $< $(TDD_INCL) -c


## Development executable ##
DEV_EXE_NAME=main
DEV_EXE=$(BUILD_PATH)/$(DEV_EXE_NAME).$(EXE_EXT)
DEV_EXE_OBJS=$(BUILD_PATH)/main.o
XML_LIBRARY_OBJS=$(BUILD_PATH)/xml_parser.o
$(DEV_EXE): $(DEV_EXE_OBJS) $(XML_LIBRARY_OBJS)
	@echo ""
	@echo "Linking main executable"
	$(CC) $(CFLAGS) -o $@ $^
	@echo ""


## Development tests executable ##
DEV_TEST_EXE=$(BUILD_PATH)/test_runner.$(EXE_EXT)
DEVELOPMENT_OBJS=$(BUILD_PATH)/dev_test_suite.o $(BUILD_PATH)/test_xml_parser.o $(BUILD_PATH)/test_xml_parser_basic.o
DEV_TEST_DEPS=$(TDD_OBJS) $(XML_LIBRARY_OBJS) $(DEVELOPMENT_OBJS)
$(DEV_TEST_EXE): $(DEV_TEST_DEPS)
	@echo ""
	@echo "Linking development tests"
	$(CC) $(CFLAGS) -o $@ $^ $(TDD_INCL)
	@echo ""
	@$(MAKE) --no-print-directory test


## Run test ##
test: $(DEV_TEST_EXE)
	@echo ""
	@echo "Running tests"
	@- ./$(DEV_TEST_EXE)


## Readme ##
MARKDOWN=Markdown.pl
README_OUT=$(BUILD_PATH)/readme.html
$(README_OUT): readme.md
	@echo ""
	@echo "Generating readme"
	$(MARKDOWN) $< > $@

$(BUILD_PATH):
	@mkdir $(BUILD_PATH)

## Miscellaneous ##
.DEFAULT_GOAL=all
.PHONY=all clean test readme arduino_verify arduino_upload
all: $(DEV_EXE) $(DEV_TEST_EXE) $(README_OUT)
clean:
	@rm -vf $(XML_LIBRARY_OBJS) $(DEV_TEST_DEPS) $(DEV_EXE_OBJS) $(README_OUT) $(DEV_EXE) $(DEV_TEST_EXE)
	$(MAKE_ARDUINO) clean
readme: $(README_OUT)

# Submake for Arduino sketch
MAKE_ARDUINO=$(MAKE) --no-print-directory -C arduino_app -f arduino.mk
arduino_verify: $(XML_LIBRARY_OBJS) $(TDD_OBJS)
	$(MAKE_ARDUINO) verify