#include "unity_fixture.h"
#include "xml_parser.hpp"
#include "test_common.hpp"
#include <string.h>

static XML_Node xmlNode, xmlNodeFound;

TEST_GROUP(XML_PARSER_BASIC);

TEST_SETUP(XML_PARSER_BASIC) {
  status = XML_Node::createNode(xmlNode, xml_string);
  xmlNodeFound = XML_Node();
  strcpy(message, "Expected not equal 0 but got 0. Return status");
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
  TEST_IGNORE();
  strcpy(xml_tag_string, "<NODE_0></NODE_1>");
  status = XML_Node::createNode(xmlNodeFound, xml_tag_string);
  TEST_ASSERT_NOT_EQUAL_MESSAGE(0, status, message);
}

TEST(XML_PARSER_BASIC, ConstructorString) {
  TEST_ASSERT_EQUAL_STRING(xml_string, xmlNode.getString());
  TEST_ASSERT_EQUAL_MESSAGE(0, xmlNode.getStart(), "Start index");
  TEST_ASSERT_EQUAL_MESSAGE(strlen(xml_string)-1, xmlNode.getEnd(), "End index");
}

TEST(XML_PARSER_BASIC, FindChild) {
  TEST_ASSERT_EQUAL_MESSAGE(0, xmlNode.findChild(xmlNodeFound, (char*)"NODE_0_0"), "Return status");
  TEST_ASSERT_EQUAL_PTR_MESSAGE(xmlNodeFound.getString(), xml_string, "String unaltered");
  TEST_ASSERT_EQUAL_MESSAGE(indexAtMatch(xml_string, (char*)"<NODE_0_0>"), xmlNodeFound.getStart(), "Start position");
  TEST_ASSERT_EQUAL_MESSAGE(indexAfterMatch(xml_string, (char*)"</NODE_0_0>")-1, xmlNodeFound.getEnd(), "End position");
}

TEST(XML_PARSER_BASIC, ErrorFindChildNotWithinThisNode) {
  TEST_ASSERT_EQUAL_MESSAGE(0, xmlNode.findChild(xmlNode, (char*)"NODE_0_0"), "Creating first node");
  TEST_ASSERT_NOT_EQUAL_MESSAGE(0, xmlNode.findChild(xmlNodeFound, (char*)"NODE_0_1_0"), "Should not find child outside of child");
}

TEST(XML_PARSER_BASIC, ErrorFindChildGibberish) {
  TEST_ASSERT_NOT_EQUAL_MESSAGE(0, xmlNode.findChild(xmlNodeFound, (char*)"Gibberish"), message);
}

TEST(XML_PARSER_BASIC, ErrorFindChildPartialStart) {
  TEST_ASSERT_NOT_EQUAL_MESSAGE(0, xmlNode.findChild(xmlNodeFound, (char*)"NODE_"), message);
}

TEST(XML_PARSER_BASIC, ErrorFindChildPartialEnd) {
  TEST_ASSERT_NOT_EQUAL_MESSAGE(0, xmlNode.findChild(xmlNodeFound, (char*)"ODE_0"), message);
}

TEST(XML_PARSER_BASIC, FindChildFirstIsPartialMatch) {
  strcpy(xml_tag_string, "<NODE_0><ODE_0></ODE_0></NODE_0>");
  TEST_ASSERT_EQUAL_MESSAGE(0, XML_Node::createNode(xmlNode, xml_tag_string), "createNode return status");
  TEST_ASSERT_EQUAL_MESSAGE(0, xmlNode.findChild(xmlNodeFound, (char*)"ODE_0"), "findChild return status");
}

TEST(XML_PARSER_BASIC, IndexAfterMatch) {
  char test_str[] = "0123456789";
  TEST_ASSERT_EQUAL_MESSAGE('2', test_str[indexAfterMatch(test_str, (char*)"1")], "Index position");
}

TEST(XML_PARSER_BASIC, FirstChild) {
  TEST_IGNORE();
  TEST_ASSERT_EQUAL_MESSAGE(0, xmlNode.findFirstChild(xmlNodeFound), "Return status");
  TEST_ASSERT_EQUAL_MESSAGE(indexAfterMatch(xml_string, (char*)"<NODE_0>"), xmlNodeFound.getStart(), "Index position");
}

TEST(XML_PARSER_BASIC, NextNode) {
  TEST_IGNORE();
  xmlNode.findFirstChild(xmlNode);
  TEST_ASSERT_EQUAL_MESSAGE(0, xmlNode.findNextNode(xmlNodeFound), "Return status");
  TEST_ASSERT_EQUAL_MESSAGE(indexAfterMatch(xml_string, (char*)"</NODE_0_0>"), xmlNodeFound.getStart(), "Index position");
}

TEST(XML_PARSER_BASIC, GetInnerXML) {
  int start = -1, length = -1;
  xmlNode.getInnerXML(start, length);

  TEST_ASSERT_EQUAL_MESSAGE(indexAfterMatch(xml_string, (char*)"<NODE_0>"), start, "Start position");
  TEST_ASSERT_EQUAL_MESSAGE(indexAtMatch(xml_string, (char*)"</NODE_0>") - indexAfterMatch(xml_string, (char*)"<NODE_0>"), length, "Length");
}

TEST(XML_PARSER_BASIC, GetInnerXML_SelfClosing) {
  int start = -1, length = -1;
  int status = xmlNode.findChild(xmlNodeFound, (char*)"NODE_0_1_0");
  TEST_ASSERT_EQUAL_MESSAGE(0, status, "Return status");

  xmlNodeFound.getInnerXML(start, length);

  TEST_ASSERT_EQUAL_MESSAGE(indexAfterMatch(xml_string, (char*)"<NODE_0_1_0/>"), start, "Start position");
  TEST_ASSERT_EQUAL_MESSAGE(0, length, "Length");
}

TEST_GROUP_RUNNER(XML_PARSER_BASIC) {
  RUN_TEST_CASE(XML_PARSER_BASIC, CreateNode);
  RUN_TEST_CASE(XML_PARSER_BASIC, ErrorCreateNodeMismatchingTags);
  RUN_TEST_CASE(XML_PARSER_BASIC, ConstructorString);
  RUN_TEST_CASE(XML_PARSER_BASIC, IndexAfterMatch);
  RUN_TEST_CASE(XML_PARSER_BASIC, FirstChild);
  RUN_TEST_CASE(XML_PARSER_BASIC, FindChild);
  RUN_TEST_CASE(XML_PARSER_BASIC, ErrorFindChildNotWithinThisNode);
  RUN_TEST_CASE(XML_PARSER_BASIC, ErrorFindChildGibberish);
  RUN_TEST_CASE(XML_PARSER_BASIC, ErrorFindChildPartialStart);
  RUN_TEST_CASE(XML_PARSER_BASIC, ErrorFindChildPartialEnd);
  RUN_TEST_CASE(XML_PARSER_BASIC, FindChildFirstIsPartialMatch);
  RUN_TEST_CASE(XML_PARSER_BASIC, NextNode);
  RUN_TEST_CASE(XML_PARSER_BASIC, GetInnerXML);
  RUN_TEST_CASE(XML_PARSER_BASIC, GetInnerXML_SelfClosing);
}