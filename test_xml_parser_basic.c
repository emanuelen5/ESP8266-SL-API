#include "unity_fixture.h"
#include "xml_parser.h"
#include "string.h"

#define RESPONSE_MAX_LENGTH 200
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

/**
 * Find the index after a match of a strstr match
 */
static inline int indexAfterMatch(const char *haystack, const char *needle) {
  int matchPos = strstr(haystack, needle) - haystack;
  if (matchPos >= 0 and matchPos < (signed)(strlen(haystack) - strlen(needle))) {
    matchPos += strlen(needle);
  }
  return matchPos;
}

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

void testParseTag(char *xml_tag_string, int expectedStatus, enum E_XML_TAG_TYPE expectedTagType) {
  enum E_XML_TAG_TYPE tagType = XML_TAG_ERROR_UNDEFINED;
  int parseEnd = -1;
  TEST_ASSERT_EQUAL(expectedStatus, parseTag(xml_tag_string, parseEnd, tagType));
  TEST_ASSERT_EQUAL(strlen(xml_tag_string), parseEnd);
  TEST_ASSERT_EQUAL(expectedTagType, tagType);
}

TEST(XML_PARSER_BASIC, ParseTagOpening) {
  testParseTag((char*)"<OpeningTag>", 0, XML_TAG_OPENING);
}

TEST(XML_PARSER_BASIC, ParseTagSelfClosing) {
  testParseTag((char*)"<SelfClosingTag/>", 0, XML_TAG_SELF_CLOSING);
}

TEST(XML_PARSER_BASIC, ParseTagClosing) {
  testParseTag((char*)"</ClosingTag>", 0, XML_TAG_CLOSING);
}
 
TEST(XML_PARSER_BASIC, ParseTagIllegalStart) {
  char xml_tag_string[] = "Value</MiddleOfNode>";
  enum E_XML_TAG_TYPE tagType = XML_TAG_ERROR_UNDEFINED;
  int parseEnd = -1;
  TEST_ASSERT_EQUAL(-1, parseTag(xml_tag_string, parseEnd, tagType));
  TEST_ASSERT_EQUAL(0, parseEnd);
  TEST_ASSERT_EQUAL(XML_TAG_ERROR_ILLEGAL_START_POS, tagType);
}

TEST(XML_PARSER_BASIC, ParseTagIllegalEnd) {
  char xml_tag_string[] = "<Value/a>";
  enum E_XML_TAG_TYPE tagType = XML_TAG_ERROR_UNDEFINED;
  int parseEnd = -1;
  TEST_ASSERT_EQUAL(-1, parseTag(xml_tag_string, parseEnd, tagType));
  TEST_ASSERT_EQUAL(XML_TAG_ERROR_ILLEGAL_ENDING, tagType);
}

TEST(XML_PARSER_BASIC, ParseTagNotXML) {
  char xml_tag_string[] = "<xml/>";
  enum E_XML_TAG_TYPE tagType = XML_TAG_ERROR_UNDEFINED;
  int parseEnd = -1;
  TEST_ASSERT_EQUAL(-1, parseTag(xml_tag_string, parseEnd, tagType));
  TEST_ASSERT_EQUAL(XML_TAG_ERROR_ILLEGAL_NAME, tagType);
}

TEST(XML_PARSER_BASIC, ParseTagName) {
  char xml_tag_string[] = "Value/>";
  int parseEnd = -1;
  TEST_ASSERT_EQUAL(0, parseTagName(xml_tag_string, parseEnd));
  TEST_ASSERT_EQUAL(indexAfterMatch(xml_tag_string, "Value"), parseEnd);
}

TEST_GROUP_RUNNER(XML_PARSER_BASIC) {
  RUN_TEST_CASE(XML_PARSER_BASIC, ConstructorString);
  RUN_TEST_CASE(XML_PARSER_BASIC, IndexAfterMatch);
  RUN_TEST_CASE(XML_PARSER_BASIC, FirstChild);
  RUN_TEST_CASE(XML_PARSER_BASIC, CanNotFindGibberish);
  RUN_TEST_CASE(XML_PARSER_BASIC, CanNotFindPartialName);
  RUN_TEST_CASE(XML_PARSER_BASIC, NextNode);
  RUN_TEST_CASE(XML_PARSER_BASIC, ParseTagOpening);
  RUN_TEST_CASE(XML_PARSER_BASIC, ParseTagSelfClosing);
  RUN_TEST_CASE(XML_PARSER_BASIC, ParseTagClosing);
  RUN_TEST_CASE(XML_PARSER_BASIC, ParseTagIllegalStart);
  RUN_TEST_CASE(XML_PARSER_BASIC, ParseTagIllegalEnd);
  RUN_TEST_CASE(XML_PARSER_BASIC, ParseTagNotXML);
  RUN_TEST_CASE(XML_PARSER_BASIC, ParseTagName);
}