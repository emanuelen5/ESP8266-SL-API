#include "unity_fixture.h"
#include "xml_parser.h"

TEST_GROUP(OFFLINE_XML_FILES);

TEST_SETUP(OFFLINE_XML_FILES) {
}

TEST_TEAR_DOWN(OFFLINE_XML_FILES) {
}

TEST(OFFLINE_XML_FILES, CanReadFile) {
  // TEST_FAIL_MESSAGE("Start here");
}

TEST_GROUP_RUNNER(OFFLINE_XML_FILES) {
  // RUN_TEST_CASE(OFFLINE_XML_FILES, CanReadFile);
}

static void runAllTests() {
  RUN_TEST_GROUP(OFFLINE_XML_FILES);
}

int main(int argc, const char *argv[]) {
  return UnityMain(argc, argv, runAllTests);
}