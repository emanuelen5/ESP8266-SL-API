#include "unity_fixture.h"
#include "xml_parser.hpp"
#include "test_common.hpp"
#include <string.h>

int indexAfterMatch(const char *haystack, const char *needle) {
  int matchPos = strstr(haystack, needle) - haystack;
  if (matchPos >= 0 and matchPos < (signed)(strlen(haystack) - strlen(needle))) {
    matchPos += strlen(needle);
  }
  return matchPos;
}

int indexAtMatch(const char *haystack, const char *needle) {
  int matchPos = strstr(haystack, needle) - haystack;
  return matchPos;
}
