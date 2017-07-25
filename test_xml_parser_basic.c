#include "unity_fixture.h"
#include "xml_parser.h"
#include "string.h"

#define MSG_LENGTH 100
#define RESPONSE_MAX_LENGTH 2
char msg[MSG_LENGTH];
char text_buffer[RESPONSE_MAX_LENGTH];
string_ptr_t res;

TEST_GROUP(XML_PARSER_BASIC);

TEST_SETUP(XML_PARSER_BASIC) {
  zeroStringPtrT(&res);
  initStringPtrT(&res, text_buffer);
  memset(text_buffer, 0, sizeof(text_buffer));
  strcpy(text_buffer, "<TestNode></TestNode>");
}

TEST_TEAR_DOWN(XML_PARSER_BASIC) {
}

TEST(XML_PARSER_BASIC, initStringPtrT) {
  string_ptr_t res;
  initStringPtrT(&res, text_buffer);
  TEST_ASSERT_EQUAL(text_buffer, res.string);
  TEST_ASSERT_EQUAL(0, res.start);
  TEST_ASSERT_EQUAL(strlen(text_buffer), res.end);
}

TEST(XML_PARSER_BASIC, CanNotFindGibberish) {
  int status;
  string_ptr_t res;
  initStringPtrT(&res, text_buffer);
  status = findNodeBoundary(&res, &res, "Gibberish");
  TEST_ASSERT_EQUAL(-1, status);
}

TEST_GROUP_RUNNER(XML_PARSER_BASIC) {
  RUN_TEST_CASE(XML_PARSER_BASIC, initStringPtrT);
  RUN_TEST_CASE(XML_PARSER_BASIC, CanNotFindGibberish);
}