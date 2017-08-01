#ifndef _TEST_COMMON_H_
#define _TEST_COMMON_H_

extern char msg[];
extern char xml_string[];

/**
 * Find the index after a match of a strstr match
 */
int indexAfterMatch(const char *haystack, const char *needle);
int indexAtMatch(const char *haystack, const char *needle);

/**
 * Test class to expose private methods/functions
 */
class XML_Node_Test : public XML_Node {
  public:
    // Constuctors are not inherited
    XML_Node_Test() : XML_Node() {};
    XML_Node_Test(char *string) : XML_Node(string) {};
    XML_Node_Test(char *string, int start, int end) : XML_Node(string, start, end) {};
    char *string;
    int start, end;
};

#endif //_TEST_COMMON_H_