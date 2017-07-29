#include "unity_fixture.h"
#include "xml_parser.h"
#include "test_common.h"
#include "string.h"

extern char *msg;
static char xml_string[] = 
  "<NODE_0>"
    "<NODE_0_0>"
      "NODE_0_0_VALUE"
    "</NODE_0_0>"
    "<NODE_0_1>"
      "<NODE_0_1_0/>"
      "<NODE_0_1_1>"
        "NODE_0_1_1_VALUE"
      "</NODE_0_1_1>"
    "</NODE_0_1>"
  "</NODE_0>";
XML_Node xmlNode, xmlNodeFound;

TEST_GROUP(XML_PARSER_BASIC);

TEST_SETUP(XML_PARSER_BASIC) {
  xmlNode = XML_Node(xml_string);
  xmlNodeFound = XML_Node();
}

TEST_TEAR_DOWN(XML_PARSER_BASIC) {
}

TEST(XML_PARSER_BASIC, ConstructorString) {
  TEST_ASSERT_EQUAL_STRING(xml_string, xmlNode.getString());
  TEST_ASSERT_EQUAL(0, xmlNode.getStart());
  TEST_ASSERT_EQUAL(strlen(xml_string), xmlNode.getEnd());
}

TEST(XML_PARSER_BASIC, CanNotFindGibberish) {
  TEST_ASSERT_NOT_EQUAL(0, xmlNode.findChild(xmlNodeFound, "Gibberish"));
}

TEST(XML_PARSER_BASIC, CanNotFindPartialName) {
  const char *message = "Node should not be found! Expected not equal 0 but got 0";
  TEST_ASSERT_NOT_EQUAL_MESSAGE(0, xmlNode.findChild(xmlNodeFound, "NODE_"), message);
  TEST_ASSERT_NOT_EQUAL_MESSAGE(0, xmlNode.findChild(xmlNodeFound, "node_0"), message);
}

TEST(XML_PARSER_BASIC, IndexAfterMatch) {
  char test_str[] = "0123456789";
  TEST_ASSERT_EQUAL('2', test_str[indexAfterMatch(test_str, "1")]);
}

TEST(XML_PARSER_BASIC, FirstChild) {
  TEST_ASSERT_EQUAL(0, xmlNode.findFirstChild(xmlNodeFound));
  TEST_ASSERT_EQUAL(indexAfterMatch(xml_string, "<NODE_0>"), xmlNodeFound.getStart());
}

TEST(XML_PARSER_BASIC, NextNode) {
  xmlNode.findFirstChild(xmlNode);
  TEST_ASSERT_EQUAL(0, xmlNode.findNextNode(xmlNodeFound));
  TEST_ASSERT_EQUAL(indexAfterMatch(xml_string, "</NODE_0_0>"), xmlNodeFound.getStart());
}

TEST_GROUP_RUNNER(XML_PARSER_BASIC) {
  RUN_TEST_CASE(XML_PARSER_BASIC, ConstructorString);
  RUN_TEST_CASE(XML_PARSER_BASIC, IndexAfterMatch);
  RUN_TEST_CASE(XML_PARSER_BASIC, FirstChild);
  RUN_TEST_CASE(XML_PARSER_BASIC, CanNotFindGibberish);
  RUN_TEST_CASE(XML_PARSER_BASIC, CanNotFindPartialName);
  RUN_TEST_CASE(XML_PARSER_BASIC, NextNode);
}