#ifndef _XML_PARSER_H_
#define _XML_PARSER_H_

typedef struct {
  char *string;
  int start, end;
} string_ptr_t;

void initStringPtrT(string_ptr_t *spt, char *string);
void zeroStringPtrT(string_ptr_t *spt);

/**
 * Finds the outer boundary of a node (including the angle brackets). 
 * @param[out]  outStr Result of search (allocated by caller)
 * @param[in]   inStr  Input string
 * @return      Status (0 if found, non-zero for failure)
 */
int findNodeBoundary(string_ptr_t *outStr, string_ptr_t *inStr, const char *nodeToFind);

/**
 * Gets the boundaries of the inner part of the node
 * @param[out]  outStr Result of search (allocated by caller)
 * @param[in]   inStr  Input string
 * @return      Status (non-zero for failure)
 */
int getNodeInner(string_ptr_t *outStr, string_ptr_t *inStr);

#endif