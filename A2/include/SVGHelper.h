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

//ASSIGNMENT 1 HELPERS
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

//ASSIGNMENT 2 HELPERS

//converts SVG struct to an xmlDoc struct i.e. a libxml tree
xmlDoc* svgtoTree(const SVG* img);
void rectToTree(List *list,xmlNodePtr root);
void circToTree(List *list,xmlNodePtr root);
void pathToTree(List *list,xmlNodePtr root);
void groupToTree(List *list,xmlNodePtr root);
void attributeToTree(List *list, xmlNodePtr elemNode);

bool updateAttribute(List* list,Attribute* attr);
bool validateRect(List *rectangles);
bool validateCirc(List *cricles);
bool validatePath(List *paths);
bool validateGroups(List *groups);
bool validateAttr(List *attributes);

SVG* createEmptySVG();

#endif
