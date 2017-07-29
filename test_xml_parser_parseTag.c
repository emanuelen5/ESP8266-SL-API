#include "unity_fixture.h"
#include "xml_parser.h"
#include "test_common.h"
#include "string.h"

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

TEST(XML_PARSER_PARSE_TAG, ParseTagOpening) {
  SET_XML_TAG_STRING("<OpeningTag>");
  TEST_ASSERT_EQUAL(0, parseTag(xml_tag_string, parseEnd, tagType));
  TEST_ASSERT_EQUAL(strlen(xml_tag_string), parseEnd);
  TEST_ASSERT_EQUAL(XML_TAG_OPENING, tagType);
}

TEST(XML_PARSER_PARSE_TAG, ParseTagSelfClosing) {
  SET_XML_TAG_STRING("<SelfClosingTag/>");
  TEST_ASSERT_EQUAL(0, parseTag(xml_tag_string, parseEnd, tagType));
  TEST_ASSERT_EQUAL(strlen(xml_tag_string), parseEnd);
  TEST_ASSERT_EQUAL(XML_TAG_SELF_CLOSING, tagType);
}

TEST(XML_PARSER_PARSE_TAG, ParseTagClosing) {
  SET_XML_TAG_STRING("</ClosingTag>");
  TEST_ASSERT_EQUAL(0, parseTag(xml_tag_string, parseEnd, tagType));
  TEST_ASSERT_EQUAL(strlen(xml_tag_string), parseEnd);
  TEST_ASSERT_EQUAL(XML_TAG_CLOSING, tagType);
}
 
TEST(XML_PARSER_PARSE_TAG, ParseTagIllegalStart) {
  SET_XML_TAG_STRING("Value</MiddleOfNode>");
  TEST_ASSERT_EQUAL(-1, parseTag(xml_tag_string, parseEnd, tagType));
  TEST_ASSERT_EQUAL(0, parseEnd);
  TEST_ASSERT_EQUAL(XML_TAG_ERROR_ILLEGAL_START_POS, tagType);
}

TEST(XML_PARSER_PARSE_TAG, ParseTagIllegalEnd) {
  SET_XML_TAG_STRING("<Value/a>");
  TEST_ASSERT_EQUAL(-1, parseTag(xml_tag_string, parseEnd, tagType));
  TEST_ASSERT_EQUAL(XML_TAG_ERROR_ILLEGAL_ENDING, tagType);
}

TEST(XML_PARSER_PARSE_TAG, ParseTagNotXML) {
  SET_XML_TAG_STRING("<xml/>");
  TEST_ASSERT_EQUAL(-1, parseTag(xml_tag_string, parseEnd, tagType));
  TEST_ASSERT_EQUAL(XML_TAG_ERROR_ILLEGAL_NAME, tagType);
}

TEST(XML_PARSER_PARSE_TAG, ParseTagName) {
  SET_XML_TAG_STRING("Value/>");
  TEST_ASSERT_EQUAL(0, parseTagName(xml_tag_string, parseEnd));
  TEST_ASSERT_EQUAL(indexAfterMatch(xml_tag_string, "Value"), parseEnd);
}

TEST_GROUP_RUNNER(XML_PARSER_PARSE_TAG) {
  RUN_TEST_CASE(XML_PARSER_PARSE_TAG, ParseTagOpening);
  RUN_TEST_CASE(XML_PARSER_PARSE_TAG, ParseTagSelfClosing);
  RUN_TEST_CASE(XML_PARSER_PARSE_TAG, ParseTagClosing);
  RUN_TEST_CASE(XML_PARSER_PARSE_TAG, ParseTagIllegalStart);
  RUN_TEST_CASE(XML_PARSER_PARSE_TAG, ParseTagIllegalEnd);
  RUN_TEST_CASE(XML_PARSER_PARSE_TAG, ParseTagNotXML);
  RUN_TEST_CASE(XML_PARSER_PARSE_TAG, ParseTagName);
}