/**
Gavin Rotsaert-Smith
0997431
**/

#ifndef SVGHELPER_H
#define SVGHELPER_H

#include <stdio.h>
#include <math.h>
#include <libxml/parser.h>
#include <libxml/tree.h>
#include <libxml/encoding.h>
#include <libxml/xmlwriter.h>
#include <libxml/xmlschemastypes.h>
#include "LinkedListAPI.h"
#include "SVGParser.h"

SVG* get_SVG_elements(xmlNode *a_node);
Attribute * createAttribute(xmlAttr *attribute);
Group * createGroup(xmlNode *a_node);
Rectangle* createRectangle(xmlNode *a_node);
Circle* createCircle(xmlNode *cur_node);
Path* createPath(xmlNode *cur_node);
char *getUnits(char* string);
void deleteDummy(void* data);
int groupCount(List* list, int len);
int groupAttrCount(List *list);
int numCircleGroups(List *list, float area);
int numRectGroups(List* list, float area);
int numPathsGroup(List* list, const char* data);

List* iterListRect(List *list);
List* iterListCircles(List *list);
List* iterListPaths(List* list);
List* iterListGroups(List *list);

void appendLists(List *list, List* listToAdd);

#endif
