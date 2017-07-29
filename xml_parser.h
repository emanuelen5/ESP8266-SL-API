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

  private:
    XML_Node(char *string, int start, int end);
    char *string;
    int start, end;
};

enum E_XML_TAG_TYPE {
  XML_TAG_ERROR_UNDEFINED,
  XML_TAG_ERROR_ILLEGAL_START_POS,
  XML_TAG_ERROR_ILLEGAL_NAME,
  XML_TAG_ERROR_ILLEGAL_ENDING,
  XML_TAG_SELF_CLOSING,
  XML_TAG_OPENING,
  XML_TAG_CLOSING,
};

/**
 * Parses a tag for its type
 * @param  xmlTagStart  XML content
 * @param  parseEnd     End position (index after tag close)
 * @param  tagType      Type of tag that was parsed
 * @return              Status, non-zero if error
 */
int parseTag(char *xmlTagStart, int &parseEnd, enum E_XML_TAG_TYPE &tagType);
int parseTagName(char *xmlTagStart, int &parseEnd);

#endif //_XML_PARSER_H_