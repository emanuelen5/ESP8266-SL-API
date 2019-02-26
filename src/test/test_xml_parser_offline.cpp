#include "unity_fixture.h"
#include "xml_parser.hpp"
#include "test_common.hpp"
#include <string.h>

#define RESPONSE_MAX_LENGTH 7000
FILE *fh_departure, *fh_station;
static char text_buffer_departure[RESPONSE_MAX_LENGTH+1], text_buffer_station[RESPONSE_MAX_LENGTH+1];

static XML_Node xmlNode_station, xmlNode_departure, xmlNode, xmlNodeFound;

TEST_GROUP(OFFLINE_XML_FILES);

TEST_SETUP(OFFLINE_XML_FILES) {}
TEST_TEAR_DOWN(OFFLINE_XML_FILES) {}

/**
 * Wrapper for getRO_File with assertion
 * @param filepath Path to file
 */
static FILE *testCanOpenFile(const char *filepath) {
  sprintf(message, "Could not open file '%s', required for tests", filepath);
  FILE *fh = fopen(filepath, "r");
  TEST_ASSERT_MESSAGE(fh, message);
  return fh;
}

/**
 * Test that all the offline files are present
 */
TEST(OFFLINE_XML_FILES, FilesArePresent) {
  fh_departure = testCanOpenFile("data/departure_lookup.xml");
  fh_station   = testCanOpenFile("data/station_lookup.xml");
}

/**
 * Calls the function bufferFile with the expansion of an XML association (departure/station)
 * @param  NAME Name of an XML file
 */
#define BUFFER_FILE(NAME) bufferFile(text_buffer_##NAME, fh_##NAME, (char*) #NAME)
void bufferFile(char *text_buffer, FILE *fh, char *fileName) {
  size_t nRead;

  nRead = fread((void *) text_buffer, sizeof(char), RESPONSE_MAX_LENGTH, fh);
  sprintf(message, "Could not read from %s", fileName);
  TEST_ASSERT_MESSAGE(nRead > 0, message);
  text_buffer[nRead] = '\0';

  // Try to read again (should return 0 read if whole file was parsed correctly)
  nRead = fread((void *) text_buffer, sizeof(char), RESPONSE_MAX_LENGTH, fh);
  sprintf(message, "Buffer for %s is too small", fileName);
  TEST_ASSERT_EQUAL_MESSAGE(0, nRead, message);
}

TEST(OFFLINE_XML_FILES, CanReadFiles) {
  BUFFER_FILE(departure);
  BUFFER_FILE(station);
}

TEST(OFFLINE_XML_FILES, CanParseFilesAsXML) {
  status = XML_Node::createNode(xmlNode_departure, text_buffer_departure);
  TEST_ASSERT_EQUAL_MESSAGE(0, status, "Creating departure root node");
  
  status = XML_Node::createNode(xmlNode_station, text_buffer_station);
  TEST_ASSERT_EQUAL_MESSAGE(0, status, "Creating departure root node");
}

#define STATUS_IS_ZERO(NAME) statusIsZero(xmlNode_##NAME)
void statusIsZero(XML_Node &xmlNode) {
  int status;
  status = xmlNode.findChild(xmlNodeFound, (char*)"StatusCode");
  TEST_ASSERT_EQUAL(0, status);
  TEST_ASSERT_EQUAL_MESSAGE(121, xmlNodeFound.getStart(), "Start position");
  TEST_ASSERT_EQUAL_MESSAGE(146, xmlNodeFound.getEnd(), "End position");

  int start = -1, length = -1;
  xmlNodeFound.getInnerXML(start, length);
  TEST_ASSERT_EQUAL_MESSAGE(strlen("0"), length, "Inner length");
  char *innerXMLStart = xmlNodeFound.getString() + start;
  TEST_ASSERT_EQUAL_MESSAGE(0, strncmp("0", innerXMLStart, length), "Inner XML is '0'");
}

TEST(OFFLINE_XML_FILES, Departure_StatusCodeIsZero) {
  STATUS_IS_ZERO(departure);
}
TEST(OFFLINE_XML_FILES, Station_StatusCodeIsZero) {
  STATUS_IS_ZERO(station);
}

TEST_GROUP_RUNNER(OFFLINE_XML_FILES) {
  RUN_TEST_CASE(OFFLINE_XML_FILES, FilesArePresent);
  RUN_TEST_CASE(OFFLINE_XML_FILES, CanReadFiles);
  RUN_TEST_CASE(OFFLINE_XML_FILES, CanParseFilesAsXML);
  RUN_TEST_CASE(OFFLINE_XML_FILES, Departure_StatusCodeIsZero);
  RUN_TEST_CASE(OFFLINE_XML_FILES, Station_StatusCodeIsZero);
  fclose(fh_departure);
  fclose(fh_station);
}
