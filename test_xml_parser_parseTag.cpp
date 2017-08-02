#include "unity_fixture.h"
#include "xml_parser.hpp"
#include "test_common.hpp"
#include <string.h>

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
  status = parseTag(xml_tag_string, parseEnd, tagType);
  TEST_ASSERT_EQUAL_MESSAGE(0, status, "Return status");
  TEST_ASSERT_EQUAL_MESSAGE(strlen(xml_tag_string), parseEnd,  "Parse end");
  TEST_ASSERT_EQUAL_MESSAGE(XML_TAG_OPENING, tagType, "Tag type");
}

TEST(XML_PARSER_PARSE_TAG, SelfClosingTag) {
  SET_XML_TAG_STRING("<SelfClosingTag/>");
  status = parseTag(xml_tag_string, parseEnd, tagType);
  TEST_ASSERT_EQUAL_MESSAGE(0, status, "Return status");
  TEST_ASSERT_EQUAL_MESSAGE(strlen(xml_tag_string), parseEnd,  "Parse end");
  TEST_ASSERT_EQUAL_MESSAGE(XML_TAG_SELF_CLOSING, tagType, "Tag type");
}

TEST(XML_PARSER_PARSE_TAG, ClosingTag) {
  SET_XML_TAG_STRING("</ClosingTag>");
  status = parseTag(xml_tag_string, parseEnd, tagType);
  TEST_ASSERT_EQUAL_MESSAGE(0, status, "Return status");
  TEST_ASSERT_EQUAL_MESSAGE(strlen(xml_tag_string), parseEnd,  "Parse end");
  TEST_ASSERT_EQUAL_MESSAGE(XML_TAG_CLOSING, tagType, "Tag type");
}
 
TEST(XML_PARSER_PARSE_TAG, ErrorTagStart) {
  SET_XML_TAG_STRING("Value</MiddleOfNode>");
  status = parseTag(xml_tag_string, parseEnd, tagType);
  TEST_ASSERT_EQUAL_MESSAGE(-1, status, "Return status");
  TEST_ASSERT_EQUAL_MESSAGE(0, parseEnd, "Parse end");
  TEST_ASSERT_EQUAL_MESSAGE(XML_TAG_ERROR_ILLEGAL_START_POS, tagType, "Tag type");
}

TEST(XML_PARSER_PARSE_TAG, ErrorTagEnd) {
  SET_XML_TAG_STRING("<Value/a>");
  status = parseTag(xml_tag_string, parseEnd, tagType);
  TEST_ASSERT_EQUAL_MESSAGE(-5, status, "Return status");
  TEST_ASSERT_EQUAL_MESSAGE(XML_TAG_ERROR_ILLEGAL_ENDING, tagType, "Tag type");
}

TEST(XML_PARSER_PARSE_TAG, ErrorTagNameXML) {
  SET_XML_TAG_STRING("<xml/>");
  status = parseTag(xml_tag_string, parseEnd, tagType);
  TEST_ASSERT_EQUAL_MESSAGE(-2, status, "Return status");
  TEST_ASSERT_EQUAL_MESSAGE(XML_TAG_ERROR_ILLEGAL_NAME, tagType, "Tag type");
}

TEST(XML_PARSER_PARSE_TAG, Name) {
  SET_XML_TAG_STRING("Value/>");
  status = parseTagName(xml_tag_string, parseEnd);
  TEST_ASSERT_EQUAL_MESSAGE(0, status, "Return status");
  TEST_ASSERT_EQUAL_MESSAGE(indexAfterMatch(xml_tag_string, "Value"), parseEnd, "Parse end");
}

TEST(XML_PARSER_PARSE_TAG, WithAttribute) {
  SET_XML_TAG_STRING("<Tag attribute=\"azAZ09.,!@#$%^&*/;'()[]{}|+-_=\">");
  status = parseTag(xml_tag_string, parseEnd, tagType);
  TEST_ASSERT_EQUAL_MESSAGE(0, status, "Return status");
  TEST_ASSERT_EQUAL_MESSAGE(strlen(xml_tag_string), parseEnd,  "Parse end");
  TEST_ASSERT_EQUAL_MESSAGE(XML_TAG_OPENING, tagType, "Tag type");
}

TEST(XML_PARSER_PARSE_TAG, WithMultipleAttributes) {
  SET_XML_TAG_STRING("<Tag attr1=\"value1\" attr2=\"value2\">");
  status = parseTag(xml_tag_string, parseEnd, tagType);
  TEST_ASSERT_EQUAL_MESSAGE(0, status, "Return status");
  TEST_ASSERT_EQUAL_MESSAGE(strlen(xml_tag_string), parseEnd,  "Parse end");
  TEST_ASSERT_EQUAL_MESSAGE(XML_TAG_OPENING, tagType, "Tag type");
}

TEST(XML_PARSER_PARSE_TAG, Attribute) {
  SET_XML_TAG_STRING("attr=\"Value\"");
  status = parseTagAttribute(xml_tag_string, parseEnd);
  TEST_ASSERT_EQUAL_MESSAGE(0, status, "Return status");
  TEST_ASSERT_EQUAL_MESSAGE(strlen(xml_tag_string), parseEnd,  "Parse end");
}

TEST(XML_PARSER_PARSE_TAG, AttributeEscapedQuote) {
  SET_XML_TAG_STRING("attr=\"\\\"Value\\\"\"");
  status = parseTagAttribute(xml_tag_string, parseEnd);
  TEST_ASSERT_EQUAL_MESSAGE(0, status, "Return status");
  TEST_ASSERT_EQUAL_MESSAGE(strlen(xml_tag_string), parseEnd,  "Parse end");
}

TEST(XML_PARSER_PARSE_TAG, ErrorAttributeEarlyEnding) {
  SET_XML_TAG_STRING("attr=\"Value\0");
  status = parseTagAttribute(xml_tag_string, parseEnd);
  TEST_ASSERT_EQUAL_MESSAGE(-4, status, "Return status");
}

TEST(XML_PARSER_PARSE_TAG, ParseUntilCharacter) {
  SET_XML_TAG_STRING("0123456789\"  ");
  status = parseUntilCharacter(xml_tag_string, parseEnd, '\"');
  TEST_ASSERT_EQUAL_MESSAGE(0, status, "Return status");
  TEST_ASSERT_EQUAL_MESSAGE(10, parseEnd,  "Parse end");
}

TEST(XML_PARSER_PARSE_TAG, ErrorParseUntilCharacterEarlyEnd) {
  SET_XML_TAG_STRING("0123456789\0  ");
  status = parseUntilCharacter(xml_tag_string, parseEnd, '\"');
  TEST_ASSERT_EQUAL_MESSAGE(-1, status, "Return status");
  TEST_ASSERT_EQUAL_MESSAGE(10, parseEnd,  "Parse end");
}

TEST(XML_PARSER_PARSE_TAG, ParseUntilUnescapedCharacter) {
  SET_XML_TAG_STRING("0123456789\\\"\"  ");
  status = parseUntilUnescapedCharacter(xml_tag_string, parseEnd, '\"');
  TEST_ASSERT_EQUAL_MESSAGE(0, status, "Return status");
  TEST_ASSERT_EQUAL_MESSAGE(12, parseEnd,  "Parse end");
}

TEST_GROUP_RUNNER(XML_PARSER_PARSE_TAG) {
  RUN_TEST_CASE(XML_PARSER_PARSE_TAG, OpeningTag);
  RUN_TEST_CASE(XML_PARSER_PARSE_TAG, SelfClosingTag);
  RUN_TEST_CASE(XML_PARSER_PARSE_TAG, ClosingTag);
  RUN_TEST_CASE(XML_PARSER_PARSE_TAG, ErrorTagStart);
  RUN_TEST_CASE(XML_PARSER_PARSE_TAG, ErrorTagEnd);
  RUN_TEST_CASE(XML_PARSER_PARSE_TAG, ErrorTagNameXML);
  RUN_TEST_CASE(XML_PARSER_PARSE_TAG, Name);
  RUN_TEST_CASE(XML_PARSER_PARSE_TAG, WithAttribute);
  RUN_TEST_CASE(XML_PARSER_PARSE_TAG, WithMultipleAttributes);
  RUN_TEST_CASE(XML_PARSER_PARSE_TAG, Attribute);
  RUN_TEST_CASE(XML_PARSER_PARSE_TAG, AttributeEscapedQuote);
  RUN_TEST_CASE(XML_PARSER_PARSE_TAG, ErrorAttributeEarlyEnding);
  RUN_TEST_CASE(XML_PARSER_PARSE_TAG, ParseUntilCharacter);
  RUN_TEST_CASE(XML_PARSER_PARSE_TAG, ErrorParseUntilCharacterEarlyEnd);
  RUN_TEST_CASE(XML_PARSER_PARSE_TAG, ParseUntilUnescapedCharacter);
}