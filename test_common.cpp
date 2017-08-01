#include "unity_fixture.h"
#include "xml_parser.hpp"
#include "test_common.hpp"
#include <string.h>

#define MSG_LENGTH 100
char msg[MSG_LENGTH];
char xml_string[] = 
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
