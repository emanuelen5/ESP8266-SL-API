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
  return -1;
}

int XML_Node::findFirstChild(XML_Node &outNode) {
  return -1;
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