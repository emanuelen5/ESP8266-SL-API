#include "unity_fixture.h"
#include "xml_parser.h"
#include "string.h"

#define RESPONSE_MAX_LENGTH 200
extern char *msg;
char text_buffer[RESPONSE_MAX_LENGTH] = "<BareNode>test_value</BareNode><ParentNode>ParentValue<ChildNode>ChildValue</ChildNode></ParentNode>";
string_ptr_t res;

TEST_GROUP(XML_PARSER_BASIC);

TEST_SETUP(XML_PARSER_BASIC) {
  zeroStringPtrT(&res);
  initStringPtrT(&res, text_buffer);
}

TEST_TEAR_DOWN(XML_PARSER_BASIC) {
}

TEST(XML_PARSER_BASIC, initStringPtrT) {
  TEST_ASSERT_EQUAL(text_buffer, res.string);
  TEST_ASSERT_EQUAL(0, res.start);
  TEST_ASSERT_EQUAL(strlen(text_buffer), res.end);
}

TEST(XML_PARSER_BASIC, CanNotFindGibberish) {
  int status;
  status = findNodeBoundary(&res, &res, "Gibberish");
  TEST_ASSERT_EQUAL(-1, status);
}

TEST_GROUP_RUNNER(XML_PARSER_BASIC) {
  RUN_TEST_CASE(XML_PARSER_BASIC, initStringPtrT);
  RUN_TEST_CASE(XML_PARSER_BASIC, CanNotFindGibberish);
}