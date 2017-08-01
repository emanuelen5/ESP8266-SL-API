#include "unity_fixture.h"
#include "xml_parser.hpp"
#include "test_common.hpp"
#include <string.h>

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
static XML_Node_Test xmlNode, xmlNodeFound;

TEST_GROUP(XML_PARSER_PRIVATE);

TEST_SETUP(XML_PARSER_PRIVATE) {
  xmlNode = XML_Node_Test(xml_string);
  xmlNodeFound = XML_Node_Test();
}

TEST_TEAR_DOWN(XML_PARSER_PRIVATE) {
}

TEST(XML_PARSER_PRIVATE, FindClosingTag) {
  xmlNode = XML_Node_Test(xml_string, indexAtMatch(xml_string, "<NODE_0_0>"), indexAfterMatch(xml_string, "</NODE_0_0>")-1);
  TEST_ASSERT_EQUAL_MESSAGE(0, xmlNode.findNextNode(xmlNodeFound), "Return status");
  TEST_ASSERT_EQUAL_MESSAGE(indexAfterMatch(xml_string, "</NODE_0_0>"), xmlNodeFound.getStart(), "Index position");
}

TEST_GROUP_RUNNER(XML_PARSER_PRIVATE) {
  RUN_TEST_CASE(XML_PARSER_PRIVATE, FindClosingTag);
}