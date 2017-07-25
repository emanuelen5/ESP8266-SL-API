#include "xml_parser.h"
#include "string.h"

void initStringPtrT(string_ptr_t *spt, char *string) {
  spt->start  = 0;
  spt->end    = strlen(string);
  spt->string = string;
}

void zeroStringPtrT(string_ptr_t *spt) {
  spt->start  = 0;
  spt->end    = 0;
  spt->string = NULL;
}

int findNodeBoundary(string_ptr_t *outStr, string_ptr_t *inStr, const char *nodeToFind) {
  char *pos = inStr->string + inStr->start;
  pos = strstr(pos, nodeToFind);
  // Not found
  if (pos == NULL) {
    return -1;
  }

  outStr->start = pos - inStr->string - 1;
  pos = strstr(pos+1, nodeToFind);
  outStr->end = pos - inStr->string + strlen(nodeToFind);
  return 0;
}

int getNodeInner(string_ptr_t *outStr, string_ptr_t *inStr) {
  return 1;
}