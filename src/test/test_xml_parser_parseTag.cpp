#include "unity_fixture.h"
#include "xml_parser.hpp"
#include "test_common.hpp"
#include <string.h>

TEST_GROUP(XML_PARSER_PARSE_TAG);

TEST_SETUP(XML_PARSER_PARSE_TAG) {
  memset(xml_tag_string, 0, sizeof(xml_tag_string));
  tagType = XML_Node_Test::ERROR_UNDEFINED;
  parseEnd = -1;
}

TEST_TEAR_DOWN(XML_PARSER_PARSE_TAG) {
}

TEST(XML_PARSER_PARSE_TAG, OpeningTag) {
  strcpy(xml_tag_string, "<OpeningTag>");
  status = XML_Node_Test::parseTag(xml_tag_string, parseEnd, tagType);
  TEST_ASSERT_EQUAL_MESSAGE(0, status, "Return status");
  TEST_ASSERT_EQUAL_MESSAGE(strlen(xml_tag_string), parseEnd,  "Parse end");
  TEST_ASSERT_EQUAL_MESSAGE(XML_Node_Test::OPENING, tagType, "Tag type");
}

TEST(XML_PARSER_PARSE_TAG, SelfClosingTag) {
  strcpy(xml_tag_string, "<SelfClosingTag/>");
  status = XML_Node_Test::parseTag(xml_tag_string, parseEnd, tagType);
  TEST_ASSERT_EQUAL_MESSAGE(0, status, "Return status");
  TEST_ASSERT_EQUAL_MESSAGE(strlen(xml_tag_string), parseEnd,  "Parse end");
  TEST_ASSERT_EQUAL_MESSAGE(XML_Node_Test::SELF_CLOSING, tagType, "Tag type");
}

TEST(XML_PARSER_PARSE_TAG, ClosingTag) {
  strcpy(xml_tag_string, "</ClosingTag>");
  status = XML_Node_Test::parseTag(xml_tag_string, parseEnd, tagType);
  TEST_ASSERT_EQUAL_MESSAGE(0, status, "Return status");
  TEST_ASSERT_EQUAL_MESSAGE(strlen(xml_tag_string), parseEnd,  "Parse end");
  TEST_ASSERT_EQUAL_MESSAGE(XML_Node_Test::CLOSING, tagType, "Tag type");
}
 
TEST(XML_PARSER_PARSE_TAG, ErrorTagStart) {
  strcpy(xml_tag_string, "Value</MiddleOfNode>");
  status = XML_Node_Test::parseTag(xml_tag_string, parseEnd, tagType);
  TEST_ASSERT_EQUAL_MESSAGE(-1, status, "Return status");
  TEST_ASSERT_EQUAL_MESSAGE(0, parseEnd, "Parse end");
  TEST_ASSERT_EQUAL_MESSAGE(XML_Node_Test::ERROR_ILLEGAL_START_POS, tagType, "Tag type");
}

TEST(XML_PARSER_PARSE_TAG, ErrorTagEnd) {
  strcpy(xml_tag_string, "<Value/a>");
  status = XML_Node_Test::parseTag(xml_tag_string, parseEnd, tagType);
  TEST_ASSERT_EQUAL_MESSAGE(-5, status, "Return status");
  TEST_ASSERT_EQUAL_MESSAGE(XML_Node_Test::ERROR_ILLEGAL_ENDING, tagType, "Tag type");
}

TEST(XML_PARSER_PARSE_TAG, ErrorTagNameXML) {
  strcpy(xml_tag_string, "<xml/>");
  status = XML_Node_Test::parseTag(xml_tag_string, parseEnd, tagType);
  TEST_ASSERT_EQUAL_MESSAGE(-2, status, "Return status");
  TEST_ASSERT_EQUAL_MESSAGE(XML_Node_Test::ERROR_ILLEGAL_NAME, tagType, "Tag type");
}

TEST(XML_PARSER_PARSE_TAG, Name) {
  strcpy(xml_tag_string, "Value/>");
  status = XML_Node_Test::parseTagName(xml_tag_string, parseEnd);
  TEST_ASSERT_EQUAL_MESSAGE(0, status, "Return status");
  TEST_ASSERT_EQUAL_MESSAGE(indexAfterMatch(xml_tag_string, (char*)"Value"), parseEnd, "Parse end");
}

TEST(XML_PARSER_PARSE_TAG, NameSubspace) {
  strcpy(xml_tag_string, "tag:subspace");
  status = XML_Node_Test::parseTagName(xml_tag_string, parseEnd, false, true);
  TEST_ASSERT_EQUAL_MESSAGE(0, status, "Return status");
  TEST_ASSERT_EQUAL_MESSAGE(strlen(xml_tag_string), parseEnd,  "Parse end");
}

TEST(XML_PARSER_PARSE_TAG, ErrorName) {
  strcpy(xml_tag_string, ">IllegalChar");
  status = XML_Node_Test::parseTagName(xml_tag_string, parseEnd, false);
  TEST_ASSERT_NOT_EQUAL_MESSAGE(0, status, "Expected not equal to 0, got 0. Return status");
}

TEST(XML_PARSER_PARSE_TAG, WithAttribute) {
  strcpy(xml_tag_string, "<Tag attribute=\"azAZ09.,!@#$%^&*/;'()[]{}|+-_=\">");
  status = XML_Node_Test::parseTag(xml_tag_string, parseEnd, tagType);
  TEST_ASSERT_EQUAL_MESSAGE(0, status, "Return status");
  TEST_ASSERT_EQUAL_MESSAGE(strlen(xml_tag_string), parseEnd,  "Parse end");
  TEST_ASSERT_EQUAL_MESSAGE(XML_Node_Test::OPENING, tagType, "Tag type");
}

TEST(XML_PARSER_PARSE_TAG, WithAttributeAndSpace) {
  strcpy(xml_tag_string, "<Tag attribute=\"value\" />");
  status = XML_Node_Test::parseTag(xml_tag_string, parseEnd, tagType);
  TEST_ASSERT_EQUAL_MESSAGE(0, status, "Return status");
  TEST_ASSERT_EQUAL_MESSAGE(strlen(xml_tag_string), parseEnd,  "Parse end");
  TEST_ASSERT_EQUAL_MESSAGE(XML_Node_Test::SELF_CLOSING, tagType, "Tag type");
}

TEST(XML_PARSER_PARSE_TAG, WithMultipleAttributes) {
  strcpy(xml_tag_string, "<Tag attr1=\"value1\" attr2=\"value2\">");
  status = XML_Node_Test::parseTag(xml_tag_string, parseEnd, tagType);
  TEST_ASSERT_EQUAL_MESSAGE(0, status, "Return status");
  TEST_ASSERT_EQUAL_MESSAGE(strlen(xml_tag_string), parseEnd,  "Parse end");
  TEST_ASSERT_EQUAL_MESSAGE(XML_Node_Test::OPENING, tagType, "Tag type");
}

TEST(XML_PARSER_PARSE_TAG, Attribute) {
  strcpy(xml_tag_string, "attr=\"Value\"");
  status = XML_Node_Test::parseTagAttribute(xml_tag_string, parseEnd);
  TEST_ASSERT_EQUAL_MESSAGE(0, status, "Return status");
  TEST_ASSERT_EQUAL_MESSAGE(strlen(xml_tag_string), parseEnd,  "Parse end");
}

TEST(XML_PARSER_PARSE_TAG, AttributeEscapedQuote) {
  strcpy(xml_tag_string, "attr=\"\\\"Value\\\"\"");
  status = XML_Node_Test::parseTagAttribute(xml_tag_string, parseEnd);
  TEST_ASSERT_EQUAL_MESSAGE(0, status, "Return status");
  TEST_ASSERT_EQUAL_MESSAGE(strlen(xml_tag_string), parseEnd,  "Parse end");
}

TEST(XML_PARSER_PARSE_TAG, ErrorAttributeEarlyEnding) {
  strcpy(xml_tag_string, "attr=\"Value\0");
  status = XML_Node_Test::parseTagAttribute(xml_tag_string, parseEnd);
  TEST_ASSERT_EQUAL_MESSAGE(-5, status, "Return status");
}

TEST(XML_PARSER_PARSE_TAG, ParseUntilCharacter) {
  strcpy(xml_tag_string, "0123456789\"  ");
  status = XML_Node_Test::parseUntilCharacter(xml_tag_string, parseEnd, '\"');
  TEST_ASSERT_EQUAL_MESSAGE(0, status, "Return status");
  TEST_ASSERT_EQUAL_MESSAGE(10, parseEnd,  "Parse end");
}

TEST(XML_PARSER_PARSE_TAG, ErrorParseUntilCharacterEarlyEnd) {
  strcpy(xml_tag_string, "0123456789\0  ");
  status = XML_Node_Test::parseUntilCharacter(xml_tag_string, parseEnd, '\"');
  TEST_ASSERT_EQUAL_MESSAGE(-1, status, "Return status");
  TEST_ASSERT_EQUAL_MESSAGE(10, parseEnd,  "Parse end");
}

TEST(XML_PARSER_PARSE_TAG, ParseUntilUnescapedCharacter) {
  strcpy(xml_tag_string, "0123456789\\\"\"  ");
  status = XML_Node_Test::parseUntilUnescapedCharacter(xml_tag_string, parseEnd, '\"');
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
  RUN_TEST_CASE(XML_PARSER_PARSE_TAG, NameSubspace);
  RUN_TEST_CASE(XML_PARSER_PARSE_TAG, ErrorName);
  RUN_TEST_CASE(XML_PARSER_PARSE_TAG, WithAttribute);
  RUN_TEST_CASE(XML_PARSER_PARSE_TAG, WithAttributeAndSpace);
  RUN_TEST_CASE(XML_PARSER_PARSE_TAG, WithMultipleAttributes);
  RUN_TEST_CASE(XML_PARSER_PARSE_TAG, Attribute);
  RUN_TEST_CASE(XML_PARSER_PARSE_TAG, AttributeEscapedQuote);
  RUN_TEST_CASE(XML_PARSER_PARSE_TAG, ErrorAttributeEarlyEnding);
  RUN_TEST_CASE(XML_PARSER_PARSE_TAG, ParseUntilCharacter);
  RUN_TEST_CASE(XML_PARSER_PARSE_TAG, ErrorParseUntilCharacterEarlyEnd);
  RUN_TEST_CASE(XML_PARSER_PARSE_TAG, ParseUntilUnescapedCharacter);
}