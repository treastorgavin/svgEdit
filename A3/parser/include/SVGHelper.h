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

// ASSIGNMENT 3 FUNCTIONS
char* createValidSVGtoJSON(char* fileName, char* schemaFile);
char* SVGtoJSONFromFile(char* fileName, char* schemaFile);
char* rectangleToJSONFromFile(char* fileName, char* schemaFile);
char* circleToJSONFromFile(char* fileName, char* schemaFile);
char* pathToJSONFromFile(char* fileName, char* schemaFile);
char* groupToJSONFromFile(char* fileName, char* schemaFile);
char* getTitle(char* fileName, char* schemaFile);
char* getDesc(char* fileName, char* schemaFile);
int validateFile(char* fileName, char* schemaFile);
bool createNewSVG(char* fileName, char* schemaFile, char* desc, char* title);
int addRectangleToSVGFromServer(char* fileName, char* schemaFile,float x, float y, float width, float height, char* units, char* fillAttr);
int addCircleToSVGFromServer(char* fileName, char* schemaFile,float cx, float cy, float r, char* units, char* fillAttr);
int scaleShapeFromServer(char* fileName, char* schemaFile, char* elemType, float scaleFactor);
char* attributesToJSONFromFile(char* fileName, char* schemaFile, char* shape, int index);
bool changeOrAddAttribute(char* fileName, char* schemaFile,char* shapeType, int shapeNum, char* attributeName, char* attributeValue);
bool changeTitle(char* fileName, char* schemaFile, char* title);
bool changeDescription(char* fileName, char* schemaFile, char* description);

#endif
