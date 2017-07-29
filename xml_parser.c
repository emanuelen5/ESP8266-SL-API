#include "xml_parser.h"
#include "string.h"

XML_Node::XML_Node() {
  this->start  = 0;
  this->end    = 0;
  this->string = NULL;
}

XML_Node::XML_Node(char *string) {
  this->start  = 0;
  this->end    = strlen(string);
  this->string = string;
}

XML_Node::XML_Node(char *string, int start, int end) {
  this->start  = start;
  this->end    = end;
  this->string = string;
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
  char *pos = this->getStartPtr();
  pos = strstr(pos, childName);
  // Not found
  if (pos == NULL) {
    return -1;
  }

  int start = pos - this->getStartPtr() - 1;
  pos = strstr(pos+1, childName);
  int end = pos - this->getStartPtr() + strlen(childName);

  outNode = XML_Node(this->getString(), start, end);
  return 0;
}

int parseTag(char *xmlTagStart, int &parseEnd, enum E_XML_TAG_TYPE &tagType) {
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

  while (xmlTagStart[parseEnd] == ' ') {
    parseEnd++;
  }

  // Parse the name of the XML tag
  int parseTagNameEnd;
  int status = parseTagName(&xmlTagStart[parseEnd], parseTagNameEnd);
  parseEnd += parseTagNameEnd;
  if (status) {
    tagType = XML_TAG_ERROR_ILLEGAL_NAME;
    return status;
  }

  while (xmlTagStart[parseEnd] == ' ') {
    parseEnd++;
  }

  // Check the ending
  if (tagType == XML_TAG_CLOSING && xmlTagStart[parseEnd] != '>') {
    return -1;
  } else if (xmlTagStart[parseEnd] == '/') {
    tagType = XML_TAG_SELF_CLOSING;
    parseEnd++;
  }

  if (xmlTagStart[parseEnd] != '>') {
    tagType = XML_TAG_ERROR_ILLEGAL_ENDING;
    return -1;
  } else if (tagType != XML_TAG_SELF_CLOSING && tagType != XML_TAG_CLOSING) {
    tagType = XML_TAG_OPENING;
  }
  parseEnd++;

  return 0;
}

#define ASCII_TO_LOWERCASE(c)    (((c) >= 'A') && ((c) <= 'Z'))?(c) - ('A' - 'a'):(c)

int categorizeXMLNameCharacter(char c) {
  // Legal starting XML characters
  if (c == '_' || (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z')) {
    return 1;
  // Legal inner XML characters
  } else if (c == '.' || c == '-' || (c >= '0' && c <= '9')) {
    return 2;
  // Illegal XML characters
  } else {
    return 0;
  }
}

int parseTagName(char *xmlTagNameStart, int &parseEnd) {
  if (categorizeXMLNameCharacter(xmlTagNameStart[0]) != 1) {
    parseEnd = 0;
    return -1;
  }

  parseEnd = 1;
  while (categorizeXMLNameCharacter(xmlTagNameStart[parseEnd])) {
    parseEnd++;
  }

  if (parseEnd == 3) {
    // Cannot be called "XML"
    if ((ASCII_TO_LOWERCASE(xmlTagNameStart[0]) == 'x') &&
        (ASCII_TO_LOWERCASE(xmlTagNameStart[1]) == 'm' &&
        (ASCII_TO_LOWERCASE(xmlTagNameStart[2]) == 'l'))) {
      parseEnd = 0;
      return -1;
    }
  }
  return 0;
}