#ifndef _XML_PARSER_H_
#define _XML_PARSER_H_

/**
 * A class that points at a specific XML node in a text
 */
class XML_Node {
  public:
    XML_Node();
    XML_Node(char *string);

    /**
     * Finds the outer boundary of a node (including the angle brackets).
     * @param[out]  outNode    Result of search
     * @param[in]   childName  Input string
     * @return      Status (0 if found, non-zero for failure)
     */
    int findChild(XML_Node &outNode, const char *childName);
    int findNextNode(XML_Node &outNode);
    int findFirstChild(XML_Node &outNode);

    char *getString() {return string;};
    char *getStartPtr() {return string+start;};
    char *getEndPtr() {return string+end;};
    int getStart() {return start;};
    int getEnd() {return end;};
    int getLength() {return end - start + 1;};

  protected:
    XML_Node(char *string, int start, int end);
    char *string;
    int start, end;
};

enum E_XML_TAG_TYPE {
  XML_TAG_ERROR_UNDEFINED          = -1,
  XML_TAG_ERROR_ILLEGAL_START_POS  = -2,
  XML_TAG_ERROR_ILLEGAL_NAME       = -3,
  XML_TAG_ERROR_ILLEGAL_ATTRIBUTE  = -4,
  XML_TAG_ERROR_ILLEGAL_ENDING     = -5,
  XML_TAG_SELF_CLOSING             =  1,
  XML_TAG_OPENING                  =  2,
  XML_TAG_CLOSING                  =  3,
};

/**
 * Parses a tag for its type. The tag can contain attributes, bust must follow the name rules for XML.
 * @param  xmlTagStart  XML content
 * @param  parseEnd     End position (index after tag close)
 * @param  tagType      Type of tag that was parsed
 * @return              Status, non-zero if error
 */
int parseTag(char *xmlTagStart, int &parseEnd, enum E_XML_TAG_TYPE &tagType);
int parseTagName(char *xmlTagNameStart, int &parseEnd);
int parseTagAttribute(char *xmlTagAttributeStart, int &parseEnd);
int parseUntilCharacter(char *xmlStart, int &parseEnd, char c);

#endif //_XML_PARSER_H_