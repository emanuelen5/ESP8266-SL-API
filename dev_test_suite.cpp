#include "unity_fixture.h"

#define MSG_LENGTH 100
char msg[MSG_LENGTH];

#define RUN_TESTS(name)

static void runAllDevTests() {
  RUN_TEST_GROUP(OFFLINE_XML_FILES);
  RUN_TEST_GROUP(XML_PARSER_BASIC);
  RUN_TEST_GROUP(XML_PARSER_PARSE_TAG);
  RUN_TEST_GROUP(XML_PARSER_PRIVATE);
}

int main(int argc, const char *argv[]) {
  int status = 0;
  status |= UnityMain(argc, argv, runAllDevTests);
  return status;
}