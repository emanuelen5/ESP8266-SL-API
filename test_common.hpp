#ifndef _TEST_COMMON_H_
#define _TEST_COMMON_H_

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
    using XML_Node::E_TAG_TYPE;
    // Constuctors are not inherited
    XML_Node_Test() : XML_Node() {};
    XML_Node_Test(char *string) : XML_Node(string) {};
    XML_Node_Test(char *string, int start, int end) : XML_Node(string, start, end) {};
    char *string;
    int start, end;

    static int parseTag(const char *xmlTagStart, int &parseEnd, enum XML_Node::E_TAG_TYPE &tagType) {
      return XML_Node::parseTag(xmlTagStart, parseEnd, tagType);
    };
    static int parseTagName(const char *xmlTagNameStart, int &parseEnd, bool checkXML = true, bool allowSubspace = true) {
      return XML_Node::parseTagName(xmlTagNameStart, parseEnd, checkXML, allowSubspace);
    };
    static int parseTagAttribute(const char *xmlTagAttributeStart, int &parseEnd) {
      return XML_Node::parseTagAttribute(xmlTagAttributeStart, parseEnd);
    };
    static int parseUntilCharacter(const char *xmlStart, int &parseEnd, const char c) {
      return XML_Node::parseUntilCharacter(xmlStart, parseEnd, c);
    };
    static int parseUntilUnescapedCharacter(const char *xmlStart, int &parseEnd, const char c) {
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

#endif //_TEST_COMMON_H_