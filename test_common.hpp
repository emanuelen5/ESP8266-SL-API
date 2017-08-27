#ifndef _TEST_COMMON_H_
#define _TEST_COMMON_H_

/**
 * Find the index after a match of a strstr match
 */
int indexAfterMatch(char *haystack, char *needle);
int indexAtMatch(char *haystack, char *needle);

/**
 * Test class to expose private methods/functions
 */
class XML_Node_Test : public XML_Node {
  public:
    using XML_Node::E_TAG_TYPE;
    // Constuctors are not inherited
    XML_Node_Test() : XML_Node() {};
    XML_Node_Test(char *string) : XML_Node(string) {};
    XML_Node_Test(char *string, int start, int end) : XML_Node(string, start, end) {};
    char *string;
    int start, end;

    static int parseTag(char *xmlTagStart, int &parseEnd, enum XML_Node::E_TAG_TYPE &tagType) {
      return XML_Node::parseTag(xmlTagStart, parseEnd, tagType);
    };
    static int parseTagName(char *xmlTagNameStart, int &parseEnd, bool checkXML = true, bool allowSubspace = true) {
      return XML_Node::parseTagName(xmlTagNameStart, parseEnd, checkXML, allowSubspace);
    };
    static int parseTagAttribute(char *xmlTagAttributeStart, int &parseEnd) {
      return XML_Node::parseTagAttribute(xmlTagAttributeStart, parseEnd);
    };
    static int parseUntilCharacter(char *xmlStart, int &parseEnd, char c) {
      return XML_Node::parseUntilCharacter(xmlStart, parseEnd, c);
    };
    static int parseUntilUnescapedCharacter(char *xmlStart, int &parseEnd, char c) {
      return XML_Node::parseUntilUnescapedCharacter(xmlStart, parseEnd, c);
    };
};

#define MSG_LENGTH 100
extern char message[MSG_LENGTH];
extern char xml_string[];
extern int parseEnd, status;
extern enum XML_Node_Test::E_TAG_TYPE tagType;

#define TAG_LENGTH_MAX 50
extern char xml_tag_string[TAG_LENGTH_MAX];

// If a precondition is not satisfied, then a dependency has failed instead. Can be used for focusing test cases.
#define ASSERT_DEPENDENT_PRECONDITION(b) if ((b)) TEST_IGNORE();

#endif //_TEST_COMMON_H_