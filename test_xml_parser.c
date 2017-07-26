#include "unity_fixture.h"
#include "xml_parser.h"
#include <stdio.h>

#define RESPONSE_MAX_LENGTH 7000
extern char *msg;
FILE *fh_departure, *fh_station;
char text_buffer_departure[RESPONSE_MAX_LENGTH], text_buffer_station[RESPONSE_MAX_LENGTH];

TEST_GROUP(OFFLINE_XML_FILES);

/**
 * Test if a file can be open in read mode. If so, returns the file handle to the opened file.
 * @param filepath Path to file
 * @return         Handle to file open in RO mode
 */
static FILE *getRO_File(const char *filepath) {
  FILE *file_handle = fopen(filepath, "r");
  return file_handle;
}

TEST_SETUP(OFFLINE_XML_FILES) {
  fh_departure = getRO_File("departure_lookup.xml");
  fh_station   = getRO_File("station_lookup.xml");
}

TEST_TEAR_DOWN(OFFLINE_XML_FILES) {
  fclose(fh_departure);
  fclose(fh_station);
}

/**
 * Wrapper for getRO_File with assertion
 * @param filepath Path to file
 */
static void testCanOpenFile(const char *filepath) {
  sprintf(msg, "Could not file '%s', required for tests", filepath);
  FILE *fh = getRO_File(filepath);
  TEST_ASSERT_MESSAGE(fh, msg);
}

/**
 * Test that all the offline files are present
 */
TEST(OFFLINE_XML_FILES, FilesArePresent) {
  testCanOpenFile("departure_lookup.xml");
  testCanOpenFile("station_lookup.xml");
}

/**
 * Calls the function bufferFile with the expansion of an XML association (departure/station)
 * @param  NAME Name of an XML file
 */
#define CALL_BUFFER_FILE(NAME) bufferFile(text_buffer_##NAME, fh_##NAME, (char*) #NAME)

void bufferFile(char *text_buffer, FILE *fh, char *fileName) {
  size_t nRead;

  nRead = fread((void *) text_buffer, sizeof(char), RESPONSE_MAX_LENGTH, fh);
  sprintf(msg, "Could not read from %s", fileName);
  TEST_ASSERT_MESSAGE(nRead > 0, msg);

  nRead = fread((void *) text_buffer, sizeof(char), RESPONSE_MAX_LENGTH, fh);
  sprintf(msg, "Buffer for %s is too small", fileName);
  TEST_ASSERT_EQUAL_MESSAGE(0, nRead, msg);
}

TEST(OFFLINE_XML_FILES, CanReadFiles) {
  CALL_BUFFER_FILE(departure);
  CALL_BUFFER_FILE(station);
}

TEST(OFFLINE_XML_FILES, CanFindStatusCode) {
  int status;
  XML_Node xmlNode;

  xmlNode = XML_Node(text_buffer_departure);
  status = xmlNode.findChild(xmlNode, "StatusCode");
  TEST_ASSERT_EQUAL(0, status);
  TEST_ASSERT_EQUAL(121, xmlNode.getStart());
  TEST_ASSERT_EQUAL(146, xmlNode.getEnd());

  xmlNode = XML_Node(text_buffer_station);
  status = xmlNode.findChild(xmlNode, "StatusCode");
  TEST_ASSERT_EQUAL(0, status);
  TEST_ASSERT_EQUAL(121, xmlNode.getStart());
  TEST_ASSERT_EQUAL(146, xmlNode.getEnd());
}

TEST_GROUP_RUNNER(OFFLINE_XML_FILES) {
  RUN_TEST_CASE(OFFLINE_XML_FILES, FilesArePresent);
  RUN_TEST_CASE(OFFLINE_XML_FILES, CanReadFiles);
  RUN_TEST_CASE(OFFLINE_XML_FILES, CanFindStatusCode);
}