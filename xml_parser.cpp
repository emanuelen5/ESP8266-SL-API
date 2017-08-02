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
  parseTagName(&this->getString()[start], length);
}

int XML_Node::createNode(XML_Node &outNode, char *string) {
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

  // Parse the name of the XML tag
  int parseTagNameEnd;
  int status = parseTagName(&xmlTagStart[parseEnd], parseTagNameEnd);
  parseEnd += parseTagNameEnd;
  if (status) {
    tagType = XML_TAG_ERROR_ILLEGAL_NAME;
    return -2;
  }

  while (xmlTagStart[parseEnd] == ' ') {
    // Consume all superfluous white space
    while (xmlTagStart[++parseEnd] == ' ');
    int parseTagAttributeEnd;
    status = parseTagAttribute(&xmlTagStart[parseEnd], parseTagAttributeEnd);
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

int categorizeXMLNameCharacter(char c) {
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

int parseTagName(char *xmlTagNameStart, int &parseEnd) {
  parseEnd = 0;
  if (categorizeXMLNameCharacter(xmlTagNameStart[parseEnd]) != 1) {
    return -1;
  }

  parseEnd = 1;
  while (categorizeXMLNameCharacter(xmlTagNameStart[parseEnd])) {
    parseEnd++;
  }

  // Cannot be called "XML"
  if ((parseEnd == 3) && 
      (ASCII_TO_LOWERCASE(xmlTagNameStart[0]) == 'x') &&
      (ASCII_TO_LOWERCASE(xmlTagNameStart[1]) == 'm' &&
      (ASCII_TO_LOWERCASE(xmlTagNameStart[2]) == 'l'))) {
    parseEnd = 0;
    return -1;
  }
  return 0;
}

int parseTagAttribute(char *xmlTagAttributeStart, int &parseEnd) {
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

  int parseAttributeEnd;
  int status = 0;
  // Search for a quotation mark. Keep parsing if the quote is escaped and 0-status from parseUntilCharacter().
  while ((xmlTagAttributeStart[parseEnd-1] == '\\' || xmlTagAttributeStart[parseEnd] != '\"') && !status) {
    // Go to next character after match to search forward
    parseEnd++;
    status = parseUntilCharacter(&xmlTagAttributeStart[parseEnd], parseAttributeEnd, '\"');
    parseEnd += parseAttributeEnd;
  }
  if (status) {
    return -4;
  }

  // Move past last quote match
  parseEnd++;
  return 0;
}

int parseUntilCharacter(char *xmlStart, int &parseEnd, char c) {
  parseEnd = 0;
  while (xmlStart[parseEnd] != c && xmlStart[parseEnd] != '\0') {
    parseEnd++;
  }

  // Make sure that the ending was not due to null character
  if (xmlStart[parseEnd-1] != c && xmlStart[parseEnd] == '\0') {
    return -1;
  }

  return 0;
}