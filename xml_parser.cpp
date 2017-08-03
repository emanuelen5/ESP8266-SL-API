#include "xml_parser.hpp"
#include <string.h>

XML_Node::XML_Node() {
  this->start  = 0;
  this->end    = 0;
  this->string = NULL;
}

XML_Node::XML_Node(char *string) {
  this->start  = 0;
  this->end    = strlen(string)-1;
  this->string = string;
}

XML_Node::XML_Node(char *string, int start, int end) {
  this->start  = start;
  this->end    = end;
  this->string = string;
}

void XML_Node::getNamePos(int &start, int &length) {
  // Points to a opening/self-closing tag => second index is start of name
  start = this->getStart()+1;

  // This should not be able to fail since we already shall have parsed the name successfully!
  parseTagName(this->getString() + start, length);
}

int XML_Node::createNode(XML_Node &outNode, char *string, int start /*=0*/) {
  // Check that we point at the start of a node
  int parseEnd = start, parseEndTemp, depth = 0;
  enum E_XML_TAG_TYPE tagType;
  int status = parseTag(string + parseEnd, parseEndTemp, tagType);
  parseEnd += parseEndTemp;
  if (!status && tagType == XML_TAG_OPENING) {
    depth += 1;
  } else if (!status && tagType == XML_TAG_SELF_CLOSING) {
  } else {
    return -1;
  }

  // Stop parsing when the corresponding closing tag has been found
  while (depth != 0 && !status) {
    // Skip all the inner text of a node (angle brackets in inner text should be "&lt;")
    status = parseUntilCharacter(string + parseEnd, parseEndTemp, '<');
    parseEnd += parseEndTemp;
    if (status) { // No angle bracket found
      return -2;
    }

    status = parseTag(string + parseEnd, parseEndTemp, tagType);
    parseEnd += parseEndTemp;
    if (status) { // Error while parsing the tag
      return -3;
    } else if (tagType == XML_TAG_OPENING) {
      depth += 1;
    } else if (tagType == XML_TAG_SELF_CLOSING) {
      // Depth stays the same if self-closing
    } else if (tagType == XML_TAG_CLOSING) {
      depth -= 1;
    } else {
      return -4;
    }
  }

  outNode = XML_Node(string, start, parseEnd-1);
  return 0;
}

int XML_Node::findNextNode(XML_Node &outNode) {
  return 0;
  outNode = XML_Node(this->getString(), this->getStart(), this->getEnd());
}

int XML_Node::findFirstChild(XML_Node &outNode) {
  return 0;
  outNode = XML_Node(this->getString(), this->getStart(), this->getEnd());
}

int XML_Node::findChild(XML_Node &outNode, const char *childName) {
  int childNameLength;
  // Check that the tag name is legal
  int status = parseTagName(childName, childNameLength, false);
  bool parsedWholeString = (unsigned) childNameLength == strlen(childName);
  if (status || !parsedWholeString) {
    return -1;
  }

  char *pos;
  pos = strstr(this->getStartPtr(), childName);
  // Not found string
  if (pos == NULL) {
    return -2;
  }

  int start = (int)(pos - this->getStartPtr() - 1); // The index before the name, if name matches
  if (XML_Node::createNode(outNode, this->getString(), start)) {
    return -3;
  }

  // Check that name is exactly the same
  int nodeNameLength;
  outNode.getNamePos(start, nodeNameLength);
  char *nodeNameStart = outNode.getString() + start;
  if (strncmp(nodeNameStart, childName, childNameLength) ||
      strncmp(nodeNameStart, childName, nodeNameLength)) {
    return -4;
  } else {
    return 0;
  }
}

int parseTag(const char *xmlTagStart, int &parseEnd, enum E_XML_TAG_TYPE &tagType) {
  if (xmlTagStart[0] != '<') {
    tagType = XML_TAG_ERROR_ILLEGAL_START_POS;
    parseEnd = 0;
    return -1;
  } else {
    tagType = XML_TAG_ERROR_UNDEFINED; // Default assignment
  }

  parseEnd = 1;
  if (xmlTagStart[parseEnd] == '/') {
    tagType = XML_TAG_CLOSING;
    parseEnd++;
  }

  // Parse the name of the XML tag
  int parseTagNameEnd;
  int status = parseTagName(xmlTagStart + parseEnd, parseTagNameEnd);
  parseEnd += parseTagNameEnd;
  if (status) {
    tagType = XML_TAG_ERROR_ILLEGAL_NAME;
    return -2;
  }

  while (xmlTagStart[parseEnd] == ' ') {
    // Consume all superfluous white space
    while (xmlTagStart[++parseEnd] == ' ');
    int parseTagAttributeEnd;
    status = parseTagAttribute(xmlTagStart + parseEnd, parseTagAttributeEnd);
    parseEnd += parseTagAttributeEnd;
    if (status) {
      tagType = XML_TAG_ERROR_ILLEGAL_ATTRIBUTE;
      return -3;
    }
  }

  // Check the ending
  if (tagType == XML_TAG_CLOSING && xmlTagStart[parseEnd] != '>') {
    tagType = XML_TAG_ERROR_ILLEGAL_ENDING;
    return -4;
  } else if (xmlTagStart[parseEnd] == '/') {
    tagType = XML_TAG_SELF_CLOSING;
    parseEnd++;
  }

  if (xmlTagStart[parseEnd] != '>') {
    tagType = XML_TAG_ERROR_ILLEGAL_ENDING;
    return -5;
  } else if (tagType != XML_TAG_SELF_CLOSING && tagType != XML_TAG_CLOSING) {
    tagType = XML_TAG_OPENING;
  }
  parseEnd++;

  return 0;
}

#define ASCII_TO_LOWERCASE(c)    (((c) >= 'A') && ((c) <= 'Z'))?(c) - ('A' - 'a'):(c)

int categorizeXMLNameCharacter(const char c) {
  // Legal starting XML characters
  if (c == '_' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
    return 1;
  // Legal inner XML characters, but not starting
  } else if (c == '.' || c == '-' || (c >= '0' && c <= '9')) {
    return 2;
  // Illegal XML characters
  } else {
    return 0;
  }
}

int parseTagName(const char *xmlTagNameStart, int &parseEnd, bool checkIsXML /*= true*/) {
  parseEnd = 0;
  if (categorizeXMLNameCharacter(xmlTagNameStart[parseEnd]) != 1) {
    return -1;
  }

  parseEnd = 1;
  while (categorizeXMLNameCharacter(xmlTagNameStart[parseEnd])) {
    parseEnd++;
  }

  // Cannot be called "XML"
  if ((checkIsXML) &&
      (parseEnd == 3) && 
      (ASCII_TO_LOWERCASE(xmlTagNameStart[0]) == 'x') &&
      (ASCII_TO_LOWERCASE(xmlTagNameStart[1]) == 'm' &&
      (ASCII_TO_LOWERCASE(xmlTagNameStart[2]) == 'l'))) {
    parseEnd = 0;
    return -1;
  }
  return 0;
}

int parseTagAttribute(const char *xmlTagAttributeStart, int &parseEnd) {
  parseEnd = 0;
  // The name of the attribute follows the same naming convention as the tag name
  if (parseTagName(xmlTagAttributeStart, parseEnd)) {
    return -1;
  }

  if (xmlTagAttributeStart[parseEnd++] != '=') {
    return -2;
  } else if (xmlTagAttributeStart[parseEnd++] != '\"') {
    return -3;
  }

  int parseAttributeEnd, status;
  status = parseUntilUnescapedCharacter(xmlTagAttributeStart + parseEnd, parseAttributeEnd, '\"');
  parseEnd += parseAttributeEnd;
  if (status) {
    return -4;
  }

  // Move past the match
  parseEnd++;
  return 0;
}

int parseUntilCharacter(const char *xmlStart, int &parseEnd, const char c) {
  char *ptr = strchr(xmlStart, c);
  if (ptr == NULL) {
    parseEnd = strlen(xmlStart);
    return -1;
  } else {
    parseEnd = (int) (ptr - xmlStart);
    return 0;
  }
}

int parseUntilUnescapedCharacter(const char *xmlStart, int &parseEnd, const char c) {
  int parseAttributeEnd;
  int status = parseUntilCharacter(xmlStart, parseAttributeEnd, c);
  parseEnd = parseAttributeEnd;
  while ((xmlStart[parseEnd-1] == '\\') && !status) {
    // Go to next character after match to search forward
    parseEnd++;
    status = parseUntilCharacter(xmlStart + parseEnd, parseAttributeEnd, c);
    parseEnd += parseAttributeEnd;
  }

  if (status) {
    return -1;
  } else {
    return 0;
  }
}