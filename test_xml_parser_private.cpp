#include "unity_fixture.h"
#include "xml_parser.hpp"
#include "test_common.hpp"
#include <string.h>

static XML_Node_Test xmlNode, xmlNodeFound;

TEST_GROUP(XML_PARSER_PRIVATE);

TEST_SETUP(XML_PARSER_PRIVATE) {
  xmlNode = XML_Node_Test(xml_string);
  xmlNodeFound = XML_Node_Test();
  xmlNode = XML_Node_Test(xml_string, indexAtMatch(xml_string, (char*)"<NODE_0_0>"), indexAfterMatch(xml_string, (char*)"</NODE_0_0>")-1);
}

TEST_TEAR_DOWN(XML_PARSER_PRIVATE) {
}

TEST(XML_PARSER_PRIVATE, GetNamePos) {
  int start = 0, end = 0;
  xmlNode.getNamePos(start, end);
  TEST_ASSERT_EQUAL(xmlNode.getStart()+1, start);
  TEST_ASSERT_EQUAL(strlen("NODE_0_0"), end);
}

TEST_GROUP_RUNNER(XML_PARSER_PRIVATE) {
  RUN_TEST_CASE(XML_PARSER_PRIVATE, GetNamePos);
}