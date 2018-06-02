CC=g++
CFLAGS=-Wall
LDFLAGS=-Wall

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
$(BUILD_PATH)/%.o: %.cpp
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
	$(CC) $(LDFLAGS) -o $@ $^
	@echo ""


## Development tests executable ##
DEV_TEST_EXE=$(BUILD_PATH)/test_runner.$(EXE_EXT)
DEVELOPMENT_OBJS=$(addprefix $(BUILD_PATH)/, dev_test_suite.o test_xml_parser_offline.o test_xml_parser_basic.o test_xml_parser_parseTag.o test_common.o test_xml_parser_private.o)
DEV_TEST_DEPS=$(TDD_OBJS) $(XML_LIBRARY_OBJS) $(DEVELOPMENT_OBJS)
$(DEV_TEST_EXE): $(DEV_TEST_DEPS)
	@echo ""
	@echo "Linking development tests"
	$(CC) $(LDFLAGS) -o $@ $^ $(TDD_INCL)
	@echo ""


## Dependencies from preprocessor inclusions ##
# xml_parser files
$(BUILD_PATH)/xml_parser.o $(DEVELOPMENT_OBJS) $(BUILD_PATH)/test_common.o: xml_parser.hpp
# tests
$(DEVELOPMENT_OBJS) $(BUILD_PATH)/test_common.o: test_common.hpp
# unity
$(BUILD_PATH)/unity.o: $(INCLUDE_PATH)/unity/*.h
# unity.h must be added as dependency as it is implicitly included by unity_fixture.h
$(BUILD_PATH)/xml_parser.o $(DEVELOPMENT_OBJS) $(BUILD_PATH)/unity_fixture.o $(BUILD_PATH)/test_common.o: $(INCLUDE_PATH)/unity_fixture/unity_fixture.h $(INCLUDE_PATH)/unity/unity.h


## Readme ##
MARKDOWN=Markdown.pl
README_OUT=$(BUILD_PATH)/readme.html
$(README_OUT): readme.md
	@echo ""
	@echo "Generating readme"
	$(MARKDOWN) $< > $@


$(BUILD_PATH):
	@echo "Creating output directory"
	@mkdir $(BUILD_PATH)


## Miscellaneous PHONY targets ##
.DEFAULT_GOAL=all
.PHONY=all clean test readme arduino_verify arduino_upload
all: $(BUILD_PATH) $(README_OUT) $(DEV_EXE) test
clean:
	@rm -vf $(XML_LIBRARY_OBJS) $(DEV_TEST_DEPS) $(DEV_EXE_OBJS) $(README_OUT) $(DEV_EXE) $(DEV_TEST_EXE)
	$(MAKE_ARDUINO) clean
readme: $(BUILD_PATH) $(README_OUT)
test: $(BUILD_PATH) $(DEV_TEST_EXE)
	@echo ""
	@echo "Running development tests"
	@- ./$(DEV_TEST_EXE)

# Submake for Arduino sketch
MAKE_ARDUINO=$(MAKE) --no-print-directory -C arduino_app -f arduino.mk
arduino_verify: $(BUILD_PATH) $(XML_LIBRARY_OBJS) $(TDD_OBJS)
	$(MAKE_ARDUINO) verify
