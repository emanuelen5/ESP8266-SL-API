#include "unity_fixture.h"
#include "xml_parser.hpp"
#include "test_common.hpp"
#include <string.h>

#define TAG_LENGTH_MAX 50
static char xml_tag_string[TAG_LENGTH_MAX];
static enum E_XML_TAG_TYPE tagType;
static int parseEnd;
#define SET_XML_TAG_STRING(string)   strcpy(xml_tag_string, (string))

TEST_GROUP(XML_PARSER_PARSE_TAG);

TEST_SETUP(XML_PARSER_PARSE_TAG) {
  memset(xml_tag_string, 0, sizeof(xml_tag_string));
  tagType = XML_TAG_ERROR_UNDEFINED;
  parseEnd = -1;
}

TEST_TEAR_DOWN(XML_PARSER_PARSE_TAG) {
}

TEST(XML_PARSER_PARSE_TAG, OpeningTag) {
  SET_XML_TAG_STRING("<OpeningTag>");
  TEST_ASSERT_EQUAL_MESSAGE(0, parseTag(xml_tag_string, parseEnd, tagType), "Return status");
  TEST_ASSERT_EQUAL_MESSAGE(strlen(xml_tag_string), parseEnd,  "Parse end");
  TEST_ASSERT_EQUAL_MESSAGE(XML_TAG_OPENING, tagType, "Tag type");
}

TEST(XML_PARSER_PARSE_TAG, SelfClosingTag) {
  SET_XML_TAG_STRING("<SelfClosingTag/>");
  TEST_ASSERT_EQUAL_MESSAGE(0, parseTag(xml_tag_string, parseEnd, tagType), "Return status");
  TEST_ASSERT_EQUAL_MESSAGE(strlen(xml_tag_string), parseEnd,  "Parse end");
  TEST_ASSERT_EQUAL_MESSAGE(XML_TAG_SELF_CLOSING, tagType, "Tag type");
}

TEST(XML_PARSER_PARSE_TAG, ClosingTag) {
  SET_XML_TAG_STRING("</ClosingTag>");
  TEST_ASSERT_EQUAL_MESSAGE(0, parseTag(xml_tag_string, parseEnd, tagType), "Return status");
  TEST_ASSERT_EQUAL_MESSAGE(strlen(xml_tag_string), parseEnd,  "Parse end");
  TEST_ASSERT_EQUAL_MESSAGE(XML_TAG_CLOSING, tagType, "Tag type");
}
 
TEST(XML_PARSER_PARSE_TAG, ErrorTagStart) {
  SET_XML_TAG_STRING("Value</MiddleOfNode>");
  TEST_ASSERT_EQUAL_MESSAGE(-1, parseTag(xml_tag_string, parseEnd, tagType), "Return status");
  TEST_ASSERT_EQUAL_MESSAGE(0, parseEnd, "Parse end");
  TEST_ASSERT_EQUAL_MESSAGE(XML_TAG_ERROR_ILLEGAL_START_POS, tagType, "Tag type");
}

TEST(XML_PARSER_PARSE_TAG, ErrorTagEnd) {
  SET_XML_TAG_STRING("<Value/a>");
  TEST_ASSERT_EQUAL_MESSAGE(-1, parseTag(xml_tag_string, parseEnd, tagType), "Return status");
  TEST_ASSERT_EQUAL_MESSAGE(XML_TAG_ERROR_ILLEGAL_ENDING, tagType, "Tag type");
}

TEST(XML_PARSER_PARSE_TAG, TagNameNotXML) {
  SET_XML_TAG_STRING("<xml/>");
  TEST_ASSERT_EQUAL_MESSAGE(-1, parseTag(xml_tag_string, parseEnd, tagType), "Return status");
  TEST_ASSERT_EQUAL_MESSAGE(XML_TAG_ERROR_ILLEGAL_NAME, tagType, "Tag type");
}

TEST(XML_PARSER_PARSE_TAG, Name) {
  SET_XML_TAG_STRING("Value/>");
  TEST_ASSERT_EQUAL_MESSAGE(0, parseTagName(xml_tag_string, parseEnd), "Return status");
  TEST_ASSERT_EQUAL_MESSAGE(indexAfterMatch(xml_tag_string, "Value"), parseEnd, "Parse end");
}

TEST(XML_PARSER_PARSE_TAG, WithAttribute) {
  SET_XML_TAG_STRING("<Tag attribute=\"azAZ09.,!@#$%^&*/;'()[]{}|+-_=\\/><");
  TEST_ASSERT_EQUAL_MESSAGE(0, parseTag(xml_tag_string, parseEnd, tagType), "Return status");
  TEST_ASSERT_EQUAL_MESSAGE(strlen(xml_tag_string), parseEnd,  "Parse end");
  TEST_ASSERT_EQUAL_MESSAGE(XML_TAG_OPENING, tagType, "Tag type");
}

TEST(XML_PARSER_PARSE_TAG, Attribute) {
  SET_XML_TAG_STRING("attr=\"Value\\\"\"");
  TEST_ASSERT_EQUAL_MESSAGE(0, parseTagAttribute(xml_tag_string, parseEnd), "Return status");
  TEST_ASSERT_EQUAL_MESSAGE(strlen(xml_tag_string), parseEnd,  "Parse end");
}

TEST(XML_PARSER_PARSE_TAG, AttributeEscapedQuote) {
  SET_XML_TAG_STRING("attr=\"\\\"Value\\\"\"");
  TEST_ASSERT_EQUAL_MESSAGE(0, parseTagAttribute(xml_tag_string, parseEnd), "Return status");
  TEST_ASSERT_EQUAL_MESSAGE(strlen(xml_tag_string), parseEnd,  "Parse end");
}

TEST(XML_PARSER_PARSE_TAG, ErrorAttributeEarlyEnding) {
  SET_XML_TAG_STRING("attr=\"Value\0");
  TEST_ASSERT_EQUAL_MESSAGE(-4, parseTagAttribute(xml_tag_string, parseEnd), "Return status");
}

TEST_GROUP_RUNNER(XML_PARSER_PARSE_TAG) {
  RUN_TEST_CASE(XML_PARSER_PARSE_TAG, OpeningTag);
  RUN_TEST_CASE(XML_PARSER_PARSE_TAG, SelfClosingTag);
  RUN_TEST_CASE(XML_PARSER_PARSE_TAG, ClosingTag);
  RUN_TEST_CASE(XML_PARSER_PARSE_TAG, ErrorTagStart);
  RUN_TEST_CASE(XML_PARSER_PARSE_TAG, ErrorTagEnd);
  RUN_TEST_CASE(XML_PARSER_PARSE_TAG, TagNameNotXML);
  RUN_TEST_CASE(XML_PARSER_PARSE_TAG, Name);
  RUN_TEST_CASE(XML_PARSER_PARSE_TAG, WithAttribute);
  RUN_TEST_CASE(XML_PARSER_PARSE_TAG, Attribute);
  RUN_TEST_CASE(XML_PARSER_PARSE_TAG, AttributeEscapedQuote);
  RUN_TEST_CASE(XML_PARSER_PARSE_TAG, ErrorAttributeEarlyEnding);
}