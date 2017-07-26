#include "unity_fixture.h"
#include "xml_parser.h"
#include "string.h"
#include <string>

#define RESPONSE_MAX_LENGTH 200
extern char *msg;
char text_buffer[RESPONSE_MAX_LENGTH] = "<BareNode>test_value</BareNode><ParentNode>ParentValue<ChildNode>ChildValue</ChildNode></ParentNode>";
XML_Node xmlNode;

TEST_GROUP(XML_PARSER_BASIC);

TEST_SETUP(XML_PARSER_BASIC) {
  xmlNode = XML_Node(text_buffer);
}

TEST_TEAR_DOWN(XML_PARSER_BASIC) {
}

TEST(XML_PARSER_BASIC, ConstructorString) {
  TEST_ASSERT_EQUAL_STRING(text_buffer, xmlNode.getString());
  TEST_ASSERT_EQUAL(0, xmlNode.getStart());
  TEST_ASSERT_EQUAL(strlen(text_buffer), xmlNode.getEnd());
}

TEST(XML_PARSER_BASIC, CanNotFindGibberish) {
  int status;
  status = xmlNode.findChild(xmlNode, "Gibberish");
  TEST_ASSERT_NOT_EQUAL(0, status);
}

TEST_GROUP_RUNNER(XML_PARSER_BASIC) {
  RUN_TEST_CASE(XML_PARSER_BASIC, ConstructorString);
  RUN_TEST_CASE(XML_PARSER_BASIC, CanNotFindGibberish);
}