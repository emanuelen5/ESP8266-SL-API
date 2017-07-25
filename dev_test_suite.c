#include "unity_fixture.h"

static void runAllDevTests() {
  RUN_TEST_GROUP(OFFLINE_XML_FILES);
  RUN_TEST_GROUP(XML_PARSER_BASIC);
}

int main(int argc, const char *argv[]) {
  return UnityMain(argc, argv, runAllDevTests);
}