#ifndef _XML_PARSER_H_
#define _XML_PARSER_H_

/**
 * A class that points at a specific XML node in a text
 */
class XML_Node {
  public:
    enum E_TAG_TYPE {
      ERROR_UNDEFINED          = -1,
      ERROR_ILLEGAL_START_POS  = -2,
      ERROR_ILLEGAL_NAME       = -3,
      ERROR_ILLEGAL_ATTRIBUTE  = -4,
      ERROR_ILLEGAL_ENDING     = -5,
      SELF_CLOSING             =  1,
      OPENING                  =  2,
      CLOSING                  =  3,
    };

    XML_Node();
    /**
     * Creates a node that starts at the given position in a string
     * @param[out]  outNode Created node unless failure
     * @param [in]  string  String pointing to where node starts (unless start position is given)
     * @param[in]   start   Start of the node within the string [Optional]
     * @return      Non-zero if error
     */
    static int createNode(XML_Node &outNode, char *string, int start = 0);

    /**
     * Finds the outer boundary of a node (including the angle brackets).
     * @param[out]  outNode    Result of search
     * @param[in]   childName  Input string
     * @return      Status (0 if found, non-zero for failure)
     */
    int findChild(XML_Node &outNode,  char *childName);
    int findNextNode(XML_Node &outNode);
    int findFirstChild(XML_Node &outNode);
    /**
     * Returns the start and length of the name in the string
     * @param[out] start  Start of the node name in the string
     * @param[out] length Length of the name
     */
    void getNamePos(int &start, int &length);
    void getInnerXML(int &start, int &length); // Inner part = between tags

    void print();

    inline char *getString() {return string;};
    char *getStartPtr() {return string+start;};
    char *getEndPtr() {return string+end;};
    inline int getStart() {return start;};
    inline int getEnd() {return end;};
    inline int getLength() {return end - start + 1;};

  protected:
    char *string;
    int start, end;

    XML_Node(char *string);
    XML_Node(char *string, int start, int end);

    /**
     * Parses a tag for its type. The tag can contain attributes, bust must follow the name rules for XML.
     * @param  xmlTagStart  XML content
     * @param  parseEnd     End position (index after tag close)
     * @param  tagType      Type of tag that was parsed
     * @return              Status, non-zero if error
     */
    static int parseTag( char *xmlTagStart, int &parseEnd, enum XML_Node::E_TAG_TYPE &tagType);
    static int parseTagName( char *xmlTagNameStart, int &parseEnd, bool checkXML = true, bool allowSubspace = true);
    static int parseTagAttribute( char *xmlTagAttributeStart, int &parseEnd);
    static int parseUntilCharacter( char *xmlStart, int &parseEnd,  char c);
    static int parseUntilUnescapedCharacter( char *xmlStart, int &parseEnd,  char c);
};

#endif //_XML_PARSER_H_