#include "unity_fixture.h"
#include "xml_parser.h"
#include "string.h"

#define RESPONSE_MAX_LENGTH 200
extern char *msg;
char text_buffer[RESPONSE_MAX_LENGTH] = "<MainNode><BareNode>test_value</BareNode><ParentNode><SimpleNode/><ChildNode>ChildValue</ChildNode></ParentNode></MainNode>";
XML_Node xmlNode, xmlNodeFound;

/**
 * Find the index after a match of a strstr match
 */
static inline int indexAfterMatch(const char *haystack, const char *needle) {
  int matchPos = haystack - strstr(haystack, needle);
  if (matchPos >= 0 and matchPos < (signed)(strlen(haystack) - strlen(needle))) {
    matchPos += sizeof(needle) + 1;
  }
  return matchPos;
}

TEST_GROUP(XML_PARSER_BASIC);

TEST_SETUP(XML_PARSER_BASIC) {
  xmlNode = XML_Node(text_buffer);
  xmlNodeFound = XML_Node();
}

TEST_TEAR_DOWN(XML_PARSER_BASIC) {
}

TEST(XML_PARSER_BASIC, ConstructorString) {
  TEST_ASSERT_EQUAL_STRING(text_buffer, xmlNode.getString());
  TEST_ASSERT_EQUAL(0, xmlNode.getStart());
  TEST_ASSERT_EQUAL(strlen(text_buffer), xmlNode.getEnd());
}

TEST(XML_PARSER_BASIC, CanNotFindGibberish) {
  TEST_ASSERT_NOT_EQUAL(0, xmlNode.findChild(xmlNodeFound, "Gibberish"));
}

TEST(XML_PARSER_BASIC, CanNotFindPartialName) {
  const char *message = "Node should not be found (0)";
  TEST_ASSERT_NOT_EQUAL_MESSAGE(0, xmlNode.findChild(xmlNodeFound, "Bare"), message);
  TEST_ASSERT_NOT_EQUAL_MESSAGE(0, xmlNode.findChild(xmlNodeFound, "Node"), message);
}

TEST(XML_PARSER_BASIC, FirstChild) {
  TEST_ASSERT_EQUAL(0, xmlNode.findFirstChild(xmlNodeFound));
  TEST_ASSERT_EQUAL(indexAfterMatch(text_buffer, "<MainNode>"), xmlNodeFound.getStart());
}

TEST(XML_PARSER_BASIC, NextNode) {
  TEST_ASSERT_EQUAL(0, xmlNode.findNextNode(xmlNodeFound));
  TEST_ASSERT_EQUAL(indexAfterMatch(text_buffer, "<MainNode>"), xmlNodeFound.getStart());
}

TEST_GROUP_RUNNER(XML_PARSER_BASIC) {
  RUN_TEST_CASE(XML_PARSER_BASIC, ConstructorString);
  RUN_TEST_CASE(XML_PARSER_BASIC, FirstChild);
  RUN_TEST_CASE(XML_PARSER_BASIC, CanNotFindGibberish);
  RUN_TEST_CASE(XML_PARSER_BASIC, CanNotFindPartialName);
  RUN_TEST_CASE(XML_PARSER_BASIC, NextNode);
}