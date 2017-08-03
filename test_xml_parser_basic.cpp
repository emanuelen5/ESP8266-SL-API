#include "unity_fixture.h"
#include "xml_parser.hpp"
#include "test_common.hpp"
#include <string.h>

static XML_Node xmlNode, xmlNodeFound;

TEST_GROUP(XML_PARSER_BASIC);

TEST_SETUP(XML_PARSER_BASIC) {
  xmlNode = XML_Node(xml_string);
  xmlNodeFound = XML_Node();
}

TEST_TEAR_DOWN(XML_PARSER_BASIC) {
}

TEST(XML_PARSER_BASIC, CreateNode) {
  status = XML_Node::createNode(xmlNodeFound, xml_string);
  TEST_ASSERT_EQUAL_MESSAGE(0, status, "Return status");
  TEST_ASSERT_EQUAL_MESSAGE(0, xmlNodeFound.getStart(), "Start index");
  TEST_ASSERT_EQUAL_MESSAGE(strlen(xml_string)-1, xmlNodeFound.getEnd(), "End index");
}

TEST(XML_PARSER_BASIC, ErrorCreateNodeMismatchingTags) {
  char xml_string_temp[] = "<NODE_0></NODE_1>";
  const char *message = "Expected not equal 0 but got 0. Return status";
  status = XML_Node::createNode(xmlNodeFound, xml_string_temp);
  TEST_ASSERT_NOT_EQUAL_MESSAGE(0, status, message);
}

TEST(XML_PARSER_BASIC, ConstructorString) {
  TEST_ASSERT_EQUAL_STRING(xml_string, xmlNode.getString());
  TEST_ASSERT_EQUAL_MESSAGE(0, xmlNode.getStart(), "Start index");
  TEST_ASSERT_EQUAL_MESSAGE(strlen(xml_string)-1, xmlNode.getEnd(), "End index");
}

TEST(XML_PARSER_BASIC, CanNotFindGibberish) {
  TEST_ASSERT_NOT_EQUAL_MESSAGE(0, xmlNode.findChild(xmlNodeFound, "Gibberish"), "Should not be found!");
}

TEST(XML_PARSER_BASIC, CanNotFindPartialName) {
  const char *message = "Expected not equal 0 but got 0. Return status";
  TEST_ASSERT_NOT_EQUAL_MESSAGE(0, xmlNode.findChild(xmlNodeFound, "NODE_"), message);
  TEST_ASSERT_NOT_EQUAL_MESSAGE(0, xmlNode.findChild(xmlNodeFound, "node_0"), message);
}

TEST(XML_PARSER_BASIC, IndexAfterMatch) {
  char test_str[] = "0123456789";
  TEST_ASSERT_EQUAL_MESSAGE('2', test_str[indexAfterMatch(test_str, "1")], "Index position");
}

TEST(XML_PARSER_BASIC, FirstChild) {
  TEST_ASSERT_EQUAL_MESSAGE(0, xmlNode.findFirstChild(xmlNodeFound), "Return status");
  TEST_ASSERT_EQUAL_MESSAGE(indexAfterMatch(xml_string, "<NODE_0>"), xmlNodeFound.getStart(), "Index position");
}

TEST(XML_PARSER_BASIC, NextNode) {
  xmlNode.findFirstChild(xmlNode);
  TEST_ASSERT_EQUAL_MESSAGE(0, xmlNode.findNextNode(xmlNodeFound), "Return status");
  TEST_ASSERT_EQUAL_MESSAGE(indexAfterMatch(xml_string, "</NODE_0_0>"), xmlNodeFound.getStart(), "Index position");
}

TEST_GROUP_RUNNER(XML_PARSER_BASIC) {
  RUN_TEST_CASE(XML_PARSER_BASIC, CreateNode);
  RUN_TEST_CASE(XML_PARSER_BASIC, ErrorCreateNodeMismatchingTags);
  RUN_TEST_CASE(XML_PARSER_BASIC, ConstructorString);
  RUN_TEST_CASE(XML_PARSER_BASIC, IndexAfterMatch);
  RUN_TEST_CASE(XML_PARSER_BASIC, FirstChild);
  RUN_TEST_CASE(XML_PARSER_BASIC, CanNotFindGibberish);
  RUN_TEST_CASE(XML_PARSER_BASIC, CanNotFindPartialName);
  RUN_TEST_CASE(XML_PARSER_BASIC, NextNode);
}