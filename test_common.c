#include "unity_fixture.h"
#include "xml_parser.h"
#include "test_common.h"
#include "string.h"

int indexAfterMatch(const char *haystack, const char *needle) {
  int matchPos = strstr(haystack, needle) - haystack;
  if (matchPos >= 0 and matchPos < (signed)(strlen(haystack) - strlen(needle))) {
    matchPos += strlen(needle);
  }
  return matchPos;
}