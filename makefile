CC=gcc
CFLAGS=-Wall -std=c99

ifeq ($(OS),Windows_NT)
	EXE_EXT=.exe
else
	EXE_EXT=.out
endif


## Compilation of test dependencies ##
# Unity library
UNITY_OBJ=unity.o
UNITY_SRC=include/Unity/unity.c
UNITY_INCL=-I"include/Unity"
# Unity fixture library
UNITY_FIXTURE_OBJ=unity_fixture.o
UNITY_FIXTURE_SRC=include/Unity_fixture/unity_fixture.c
UNITY_FIXTURE_INCL=$(UNITY_INCL) -I"include/Unity_fixture"
# Test environment dependencies
TDD_OBJS=$(UNITY_OBJ) $(UNITY_FIXTURE_OBJ)
TDD_SRC=$(UNITY_SRC) $(UNITY_FIXTURE_SRC)
TDD_INCL=$(UNITY_INCL) $(UNITY_FIXTURE_INCL) # Double inclusion, but what the hell

$(UNITY_OBJ): $(UNITY_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(UNITY_INCL) -c

$(UNITY_FIXTURE_OBJ): $(UNITY_FIXTURE_SRC)
	$(CC) $(CFLAGS) -o $@ $^ $(UNITY_FIXTURE_INCL) -c


## Library compilation ##
XML_LIBRARY_NAME=xml_parser
XML_LIBRARY_SRCS=$(XML_LIBRARY_NAME).c
XML_LIBRARY_DEPS=$(XML_LIBRARY_NAME).h
XML_LIBRARY_OBJS=$(XML_LIBRARY_NAME).o
IDIR=

$(XML_LIBRARY_OBJS): $(XML_LIBRARY_DEPS) $(XML_LIBRARY_SRCS)
	$(CC) $(CFLAGS) -o $@ -c $(XML_LIBRARY_SRCS)


## Development executable ##
DEV_EXE_NAME=main
DEV_EXE=$(DEV_EXE_NAME)$(EXE_EXT)
DEV_EXE_SRCS=$(DEV_EXE_NAME).c
# DEV_EXE_DEPS=$(DEV_EXE_NAME).h
DEV_EXE_OBJS=$(DEV_EXE_NAME).o
IDIR=

$(DEV_EXE): $(DEV_EXE_OBJS) $(XML_LIBRARY_OBJS)
	$(CC) $(CFLAGS) -o $(DEV_EXE) $(DEV_EXE_OBJS) $(XML_LIBRARY_OBJS)

$(DEV_EXE_OBJS): $(DEV_EXE_DEPS) $(DEV_EXE_SRCS)
	$(CC) $(CFLAGS) -o $@ -c $(DEV_EXE_SRCS)


## Development tests ##
DEV_TEST_NAME=test_xml_parser
DEV_TEST_EXE=test_runner$(EXE_EXT)
DEV_TEST_OBJS=$(DEV_TEST_NAME).o
DEV_TEST_SRCS=$(DEV_TEST_NAME).c

$(DEV_TEST_EXE):  $(DEV_TEST_SRCS) $(TDD_OBJS) $(XML_LIBRARY_OBJS)
	$(CC) $(CFLAGS) -o $@ $^ $(TDD_INCL)
	@$(MAKE) --no-print-directory test


## Run test ##
test: $(DEV_TEST_EXE)
	@echo ""
	@echo "Running tests"
	@- ./$(DEV_TEST_EXE)


## Readme ##
MARKDOWN=Markdown.pl
README_OUT=readme.html
$(README_OUT): readme.md
	@echo ""
	@echo "Generating readme"
	$(MARKDOWN) $< > $@


## Miscellaneous ##
.DEFAULT_GOAL=all
.PHONY=all clean test readme
all: $(DEV_EXE) $(UNITY_OBJ) $(UNITY_FIXTURE_OBJ) $(XML_LIBRARY_OBJS) $(DEV_TEST_EXE) $(README_OUT)
clean:
	@rm -vf $(DEV_EXE) $(DEV_EXE_OBJS) $(XML_LIBRARY_OBJS) $(DEV_TEST_EXE) $(UNITY_OBJ) $(UNITY_FIXTURE_OBJ) $(README_OUT)
readme: $(README_OUT)