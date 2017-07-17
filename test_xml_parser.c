#include "unity_fixture.h"
#include "xml_parser.h"
#include <stdio.h>

#define MSG_LENGTH 100
FILE *fh_departure, *fh_station;

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

/**
 * Wrapper for getRO_File with assertion
 * @param filepath Path to file
 */
static void testCanOpenFile(const char *filepath) {
  char msg[MSG_LENGTH];
  sprintf(msg, "Could not open '%s'", filepath);
  FILE *fh = getRO_File(filepath);
  TEST_ASSERT_MESSAGE(fh, msg);
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
 * Test that all the offline files are present
 */
TEST(OFFLINE_XML_FILES, FilesArePresent) {
  testCanOpenFile("departure_lookup.xml");
  testCanOpenFile("station_lookup.xml");
}

TEST_GROUP_RUNNER(OFFLINE_XML_FILES) {
  RUN_TEST_CASE(OFFLINE_XML_FILES, FilesArePresent);
}

static void runAllTests() {
  RUN_TEST_GROUP(OFFLINE_XML_FILES);
}

int main(int argc, const char *argv[]) {
  return UnityMain(argc, argv, runAllTests);
}