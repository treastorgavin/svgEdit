/**
Gavin Rotsaert-Smith
0997431
**/

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "SVGParser.h"
#include "SVGHelper.h"

// SVG parser functions

/**
 * @brief this function does the parsing and allocates an SVG struct. It accepts a filename which must not be modified in any way. If the file has been parsed successfully , a pointer to the newly created SVG struct is returned. If the parsing fails for any reason, the function must return NULL.
 * 
 * Parsing can fail for a number of reasons, but libxml hides them from us. the xmlReadFile function will simply return NULL instead of an XML doc if the file is invalid for any reason.
 * 
 * @param fileName 
 * @return SVG* pointer or NULL
 */
SVG* createSVG(const char* fileName){

    FILE *fp;
    xmlNode *root_element = NULL;
    xmlDoc *doc = NULL; // the document tree we will get after parsing
    // char namespace[256];

    if(fileName == NULL){
        //fprintf(stderr,"Invalid filename.\n");
        return NULL;
    }

    if((fp = fopen(fileName,"rb")) == NULL || strcmp(".svg",fileName+((strlen(fileName)-4)))){
        //fprintf(stderr,"Invalid file type, need .svg\n");
        return NULL;
    }

    doc = xmlReadFile(fileName, NULL, 0); //will return NULL if invalid for any reason

    if(doc == NULL){
        xmlFreeDoc(doc);
        xmlCleanupParser();
        fclose(fp);
        //fprintf(stderr, "Failed to parse %s\n", fileName);
        return NULL;
    }

    //get the root element node
    root_element = xmlDocGetRootElement(doc);

    if(root_element == NULL){
        xmlFreeDoc(doc);
        xmlCleanupParser();
        fclose(fp);
        return NULL;
    }

    // recursive helper to traverse the tree
    SVG* svg = get_SVG_elements(root_element);

    if(svg == NULL){
        xmlFreeDoc(doc);
        xmlCleanupParser();
        fclose(fp);
        return NULL;
    }
    
    xmlFreeDoc(doc);

    xmlCleanupParser();

    fclose(fp);

    //return pointer to newly created SVG struct
    return svg;
}

/**
 * @brief this function returns a humanly readable string representation of the entire vector image object. It will be used mostly by you, for debugging your parser. It must not modify the vector image object in any way. The function must allocate the string dynamically.
 * 
 * @param img 
 * @return char* 
 */
char* SVGToString(const SVG* img){

    if(img == NULL){
        return NULL;
    }

    char* rectStr = toString(img->rectangles);
    char* circStr = toString(img->circles);
    char* pathStr = toString(img->paths);
    char* groupStr = toString(img->groups);
    char* attrStr = toString(img->otherAttributes);

    int len = strlen(img->namespace)+strlen(img->title)+strlen(img->description)+strlen(rectStr)+strlen(circStr)+strlen(pathStr)+strlen(groupStr)+strlen(attrStr)+100;
    char* str = malloc(len*(sizeof(char)));

    sprintf(str,"Namespace: %s\ntitle: %s\ndesc: %s\nRectangle: %s\nCircle: %s\nPaths: %s\nGroups: %s\nAttributes:\n %s\n",img->namespace,img->title,img->description,rectStr,circStr,pathStr,groupStr,attrStr);

    free(rectStr);
    free(circStr);
    free(pathStr);
    free(groupStr);
    free(attrStr);
    
    return str;
}


/**
 * @brief this function deallocates the object, including all of its subcomponents
 * 
 * @param img 
 */
void deleteSVG(SVG* img){

    if(img == NULL){
        return;
    }
    
    freeList(img->rectangles);
    freeList(img->circles);
    freeList(img->paths);
    freeList(img->otherAttributes);
    freeList(img->groups);

    free(img);
}

// HELPER FUNCTIONS

// In addition the above functions, you must also write a number of helper functions. We will need to store the types Group, Rectangle, Circle, Path, and Attribute in lists.

//Attributes
void deleteAttribute( void* data){
    Attribute *toDelete = (Attribute*)data;

    if(data == NULL || toDelete == NULL){
        return;
    }

    free(toDelete->name);
    free(toDelete);
    //free(data);
}

char* attributeToString( void* data){
    Attribute *toS = (Attribute*)data;;

    if(data == NULL){
        return NULL;
    }

    int len = strlen(toS->name) + strlen(toS->value);

    char* str = malloc(len*(sizeof(char))+50);
    sprintf(str,"Attribute name: %s, Attribute value: %s\n", (char*)toS->name,(char*)toS->value);

    return str;
}
int compareAttributes(const void *first, const void *second){
    return 0;
}

//Groups
void deleteGroup(void* data){

    if(data == NULL){
        return;
    }

    Group *toDel = (Group*)data;

    freeList(toDel->rectangles);
    freeList(toDel->circles);
    freeList(toDel->paths);
    freeList(toDel->otherAttributes);
    freeList(toDel->groups);

    free(toDel);

}
char* groupToString( void* data){
    Group *toS = (Group*)data;//malloc(sizeof(Group*));

    if(data == NULL || toS == NULL){
        //fprintf(stderr,"Error NULL, in toString.\n");
        return NULL;
    }

    toS = (Group*)data;

    char* rectStr = toString(toS->rectangles);
    char* circStr = toString(toS->circles);
    char* pathStr = toString(toS->paths);
    char* groupStr = toString(toS->groups);
    char* attrStr = toString(toS->otherAttributes);

    int len = strlen(rectStr)+strlen(circStr)+strlen(pathStr)+strlen(groupStr)+strlen(attrStr)+100;
    char* str = malloc(len*(sizeof(char)));

    sprintf(str,"Rectangle: %s\nCircle: %s\nPaths: %s\nGroups: %s\nAttributes: %s\n",rectStr,circStr,pathStr,groupStr,attrStr);

    free(rectStr);
    free(circStr);
    free(pathStr);
    free(groupStr);
    free(attrStr);
    
    return str;
}
int compareGroups(const void *first, const void *second){
    return 0;
}

//Rectangle
void deleteRectangle(void* data){
    Rectangle *toDel = (Rectangle*)data;

    if(data == NULL || toDel == NULL){
        return;
    }

    if(toDel->otherAttributes != NULL){
        freeList(toDel->otherAttributes);
    }

    free(toDel);

}
char* rectangleToString(void* data){

    Rectangle* toS = (Rectangle*)data;

    if(data == NULL || toS == NULL){
        //fprintf(stderr,"Error NULL, in rectangleToString.\n");
        return NULL;
    }

    char* attrStr = toString(toS->otherAttributes);
    char* str = malloc(strlen(attrStr)*(sizeof(char))+100);

    sprintf(str,"x: %f, y: %f, width: %f, height: %f, units: %s, Attributes: %s\n", toS->x,toS->y,toS->width,toS->height,toS->units,attrStr);

    free(attrStr);

    // free(toS);
    
    return str;

}
int compareRectangles(const void *first, const void *second){
    return 0;
}

//Circle
void deleteCircle(void* data){
    Circle *toDel;

    if(data == NULL){
        return;
    }

    toDel = (Circle*)data;
    freeList(toDel->otherAttributes);
    
    free(toDel);

}
char* circleToString(void* data){
    Circle* toS = (Circle*)data;

    if(data == NULL || toS == NULL){
        //fprintf(stderr,"Error NULL, in rectangleToString.\n");
        return NULL;
    }

    char* attrStr = toString(toS->otherAttributes);

    int len = sizeof(float)*(toS->cx) + sizeof(float)*(toS->cx) + sizeof(float)*(toS->cx) + strlen(toS->units);

    char* str = malloc((len+(strlen(attrStr)))*(sizeof(char))+50);

    sprintf(str,"cx: %f, cy: %f, r: %f, units: %s, Attributes: %s\n", toS->cx,toS->cy,toS->r,toS->units,attrStr);

    free(attrStr);
    
    return str;
}

int compareCircles(const void *first, const void *second){
    return 0;
}

//Path
void deletePath(void* data){
    Path *toDel;
    
    if(data == NULL){
        return;
    }

    toDel = (Path*)data;
    freeList(toDel->otherAttributes);

    free(toDel);
    //free(data);
}
char* pathToString(void* data){

    Path *toS = (Path*)data;

    if(data == NULL || toS == NULL){
        //fprintf(stderr,"Error NULL, in toString.\n");
        return NULL;
    }

    char* attrStr = toString(toS->otherAttributes);

    char* str = malloc((strlen(attrStr)+strlen((char*)toS->data))*(sizeof(char))+50);
    strcpy(str,(char*)toS->data);

    sprintf(str,"data:%s,Attributes: %s\n", toS->data,attrStr);

    free(attrStr);
    
    return str;
}

int comparePaths(const void *first, const void *second){
    return 0;
}

// Function that returns a list of all rectangles in the struct.  
List* getRects(const SVG* img){
    if(img == NULL){
        return NULL;
    }

    //initialize rects list with dummyDeleter that does nothing
    //this is so that when we free this list it doesnt free the elements in the list just the pointers to the elements in the list
    
    if((img->groups->length) > 0){
        List* rects = iterListRect(img->groups);
        if((img->rectangles->length) > 0){
            ListIterator iter = createIterator(img->rectangles);
            void* elem;
            while((elem = nextElement(&iter)) != NULL){
                insertBack(rects,elem);
            }
        
        }
        return rects;
    }else{
        List* rects = initializeList(&rectangleToString,&deleteDummy,&compareRectangles);
        if((img->rectangles->length) > 0){
            ListIterator iter = createIterator(img->rectangles);
            void* elem;
            while((elem = nextElement(&iter)) != NULL){
                insertBack(rects,elem);
            }
        
        }
        return rects;
    }

}

// Function that returns a list of all circles in the struct.  
List* getCircles(const SVG* img){
    if(img == NULL){
        return NULL;
    }

    if((img->groups->length) > 0){
        List* circles = iterListCircles(img->groups);
        if((img->circles->length) > 0){
            ListIterator iter = createIterator(img->circles);
            void* elem;
            while((elem = nextElement(&iter)) != NULL){
                insertBack(circles,elem);
            }
        
        }
        return circles;
    }else{
        List* circles = initializeList(&circleToString,&deleteDummy,&compareCircles);
        if((img->circles->length) > 0){
            ListIterator iter = createIterator(img->circles);
            void* elem;
            while((elem = nextElement(&iter)) != NULL){
                insertBack(circles,elem);
            }
        
        }
        return circles;
    }

}
// Function that returns a list of all groups in the struct.
List* getGroups(const SVG* img){
    if(img == NULL){
        return NULL;
    }

    if((img->groups->length) > 0){
        List* groups = iterListGroups(img->groups);
        ListIterator iter = createIterator(img->groups);
        void* elem;
        while((elem = nextElement(&iter)) != NULL){
            insertBack(groups,elem);
        }
        
        return groups;
    }else{
        List* groups = initializeList(&groupToString,&deleteDummy,&compareGroups);
        return groups;
    }
 
}
// Function that returns a list of all paths in the struct.
List* getPaths(const SVG* img){
    if(img == NULL){
        return NULL;
    }

    if((img->groups->length) > 0){
        List* paths = iterListPaths(img->groups);
        if((img->paths->length) > 0){
            ListIterator iter = createIterator(img->paths);
            void* elem;
            while((elem = nextElement(&iter)) != NULL){
                insertBack(paths,elem);
            }
        
        }
        return paths;
    }else{
        List* paths = initializeList(&pathToString,&deleteDummy,&comparePaths);
        if((img->paths->length) > 0){
            ListIterator iter = createIterator(img->paths);
            void* elem;
            while((elem = nextElement(&iter)) != NULL){
                insertBack(paths,elem);
            }
        
        }
        return paths;
    }
}


// Function that returns the number of all rectangles with the specified area
int numRectsWithArea(const SVG* img, float area){
    if(img == NULL || (area < 0)){
        return 0;
    }

    int numRects = 0;

    if(img->rectangles->length <= 0){
        return 0;
    }else{
        ListIterator itr = createIterator(img->rectangles);
        void* data;
        while((data = nextElement(&itr)) != NULL){
            //compare intArea with area of rectangle
            Rectangle *rect = (Rectangle*)(data);
            float height = rect->height;
            float width = rect->width;

            int rectArea = ceil(height*width);
            if(rectArea == ceil(area)){
                numRects++;
            }

        }
    }
    if(img->groups->length > 0){
        numRects += numRectGroups(img->groups,area);
    }
    
    return numRects;
}
// Function that returns the number of all circles with the specified area
int numCirclesWithArea(const SVG* img, float area){
    if(img == NULL || (area < 0)){
        return 0;
    }

    int numCircs = 0;

    if(img->circles->length <= 0){
        return 0;
    }else{
        ListIterator itr = createIterator(img->circles);
        void* data;
        while((data = nextElement(&itr)) != NULL){
            //compare intArea with area of rectangle
            Circle *circ = (Circle*)(data);
            float radius = circ->r;

            int circleArea = ceil(3.14159265358979323846*radius*radius);
            if(circleArea == ceil(area)){
                numCircs++;
            }
        }
    }
    if(img->groups->length > 0){
        numCircs += numCircleGroups(img->groups,area);
    }
    
    return numCircs;
}
// Function that returns the number of all paths with the specified data - i.e. Path.data field
int numPathsWithdata(const SVG* img, const char* data){
    if(img == NULL || data == NULL){
        return 0;
    }

    int numPaths = 0;

    List* paths = getPaths(img);
    ListIterator iter = createIterator(paths);
    void* elem;
    while((elem = nextElement(&iter)) != NULL){
        //compare intArea with area of rectangle
        Path *path = (Path*)(elem);
        
        char* pathData = malloc((strlen((char*)path->data)+1)*(sizeof(char)));
        strcpy(pathData,(char*)path->data);

        if(strcmp(pathData,(char*)data) == 0){
            numPaths++;
        }
        free(pathData);
    }
    freeList(paths);

    return numPaths;
}
// Function that returns the number of all groups with the specified length - see A1 Module 2 for details
int numGroupsWithLen(const SVG* img, int len){
    if(img == NULL || (len < 0)){
        return 0;
    }

    int numGroups = 0;

    List* groups = getGroups(img);

    ListIterator iter = createIterator(groups);
    void* elem;
    while((elem = nextElement(&iter)) != NULL){
        int count = 0;
        Group *group = (Group*)(elem);

        count += group->rectangles->length;
        count += group->circles->length;
        count += group->paths->length;
        count += group->groups->length;
        if(count == len){
            numGroups++;
        }

    }
    freeList(groups);

    return numGroups;
    
}

/*  Function that returns the total number of Attribute structs in the SVG struct - i.e. the number of Attributes
    contained in all otherAttributes lists in the structs making up the SVG struct
    *@pre SVG struct  exists, is not null, and has not been freed.  
    *@post SVG has not been modified in any way
    *@return the total length of all attribute structs in the SVG
    *@param obj - a pointer to an SVG struct
*/
int numAttr(const SVG* img){
    if(img == NULL){
        return 0;
    }

    int numAttributes = 0;

    if(img->otherAttributes->length > 0){
        numAttributes += img->otherAttributes->length;
    }
    if(img->rectangles->length > 0){
        ListIterator iter = createIterator(img->rectangles);
        void* elem;
        while((elem = nextElement(&iter)) != NULL){
            Rectangle *rect = (Rectangle*)(elem);
            numAttributes += rect->otherAttributes->length;
        }
    }
    if(img->circles->length > 0){
        ListIterator iter = createIterator(img->circles);
        void* elem;
        while((elem = nextElement(&iter)) != NULL){
            Circle *circ = (Circle*)(elem);
            numAttributes += circ->otherAttributes->length;
        }
    }
    if(img->paths->length > 0){
        ListIterator iter = createIterator(img->paths);
        void* elem;
        while((elem = nextElement(&iter)) != NULL){
            Path *path = (Path*)(elem);
            numAttributes += path->otherAttributes->length;
        }
    }
    if(img->groups->length > 0){

        numAttributes += groupAttrCount(img->groups);
    }
    
    return numAttributes;
}
//hello


/** Function to create an SVG struct based on the contents of an SVG file.
 * This function must validate the XML tree generated by libxml against a SVG schema file
 * before attempting to traverse the tree and create an SVG struct
 *@pre File name cannot be an empty string or NULL.
       File represented by this name must exist and must be readable.
       Schema file name is not NULL/empty, and represents a valid schema file
 *@post Either:
        A valid SVG struct has been created and its address was returned
		or 
		An error occurred, or SVG file was invalid, and NULL was returned
 *@return the pinter to the new struct or NULL
 *@param fileName - a string containing the name of the SVG file
**/
SVG* createValidSVG(const char* fileName, const char* schemaFile){

    FILE *fp;

    if(fileName == NULL || schemaFile == NULL){
        //fprintf(stderr,"Invalid filename.\n");
        return NULL;
    }

    if((fp = fopen(fileName,"rb")) == NULL || strcmp(".svg",fileName+((strlen(fileName)-4)))){
        //fprintf(stderr,"Invalid file type, need .svg\n");
        return NULL;
    }

    fclose(fp);

    SVG* svg = NULL;
    xmlNode *root_element = NULL;
    xmlDoc *doc = NULL; // the document tree we will get after parsing

    int r = 0;

    xmlLineNumbersDefault(1);

    xmlSchemaPtr schema = NULL;
    xmlSchemaParserCtxtPtr ctxt;

    ctxt = xmlSchemaNewParserCtxt(schemaFile);
    xmlSchemaSetParserErrors(ctxt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
    schema = xmlSchemaParse(ctxt);
    xmlSchemaFreeParserCtxt(ctxt);

    doc = xmlReadFile(fileName, NULL, 0); //will return NULL if invalid for any reason
  
    if(doc == NULL){
        //fprintf(stderr, "Failed to parse %s\n", fileName);
        svg = NULL;
    }else{

        xmlSchemaValidCtxtPtr ctxt;

        ctxt = xmlSchemaNewValidCtxt(schema);
        xmlSchemaSetValidErrors(ctxt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
        r = xmlSchemaValidateDoc(ctxt, doc);
        xmlSchemaFreeValidCtxt(ctxt);
    }

    if(schema != NULL){
        xmlSchemaFree(schema);
    }

    if(r == 0){
        //valid

        //get the root element node
        root_element = xmlDocGetRootElement(doc);

        if(root_element != NULL){
            svg = get_SVG_elements(root_element);
        }

        // recursive helper to traverse the tree
        

    }
    
    xmlFreeDoc(doc);

    xmlCleanupParser();

    xmlMemoryDump();

    if(svg == NULL){
        return NULL;
    }

    return svg;
}

/** Function to writing an SVG struct into a file in SVG format.
 *@pre
    SVG struct exists, is valid, and and is not NULL.
    fileName is not NULL, has the correct extension
 *@post SVG struct has not been modified in any way, and a file representing the
    SVG contents in SVG format has been created
 *@return a boolean value indicating success or failure of the write
 *@param
    doc - a pointer to a SVG struct
 	fileName - the name of the output file
 **/
bool writeSVG(const SVG* img, const char* fileName){
    
    if(img == NULL || fileName == NULL){
        return false;
    }

    xmlDoc* doc = svgtoTree(img);
    
    //xmlSaveFormatFileEnc("-", doc, "UTF-8", 1);
    int t = xmlSaveFormatFileEnc(fileName, doc, "UTF-8", 1);

    // xmlSaveFormatFileEnc will return -1 if failure
    if(t == -1){
        xmlFreeDoc(doc);
        xmlCleanupParser();
        xmlMemoryDump();
        return false;
    }

    xmlFreeDoc(doc);

    xmlCleanupParser();
    
    return true;
}

/** Function to validating an existing an SVG struct against a SVG schema file
 *@pre 
    SVG struct exists and is not NULL
    schema file name is not NULL/empty, and represents a valid schema file
 *@post SVG has not been modified in any way
 *@return the boolean aud indicating whether the SVG is valid
 *@param obj - a pointer to a SVGdoc struct
 *@param obj - the name iof a schema file
 **/
bool validateSVG(const SVG* img, const char* schemaFile){
    
    if(img == NULL || schemaFile == NULL){
        return NULL;
    }
    
    if(!validateAttr(img->otherAttributes)){
        return false;
    }
    if(!validateRect(img->rectangles)){
        return false;
    }
    if(!validateCirc(img->circles)){
        return false;
    }
    if(!validatePath(img->paths)){
        return false;
    }
    if(!validateGroups(img->groups)){
        return false;
    }

    bool validation = false;

    FILE *fp;
    
    if((fp = fopen(schemaFile,"rb")) == NULL){
        return NULL;
    }
    
    int r = 0;

    xmlLineNumbersDefault(1);

    xmlSchemaPtr schema = NULL;
    xmlSchemaParserCtxtPtr ctxt;

    ctxt = xmlSchemaNewParserCtxt(schemaFile);
    xmlSchemaSetParserErrors(ctxt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
    schema = xmlSchemaParse(ctxt);
    xmlSchemaFreeParserCtxt(ctxt);
    
    xmlDoc* doc = svgtoTree(img);
    
    if(doc == NULL){
        return false;
    }else{

        xmlSchemaValidCtxtPtr ctxt;
        ctxt = xmlSchemaNewValidCtxt(schema);
        xmlSchemaSetValidErrors(ctxt, (xmlSchemaValidityErrorFunc) fprintf, (xmlSchemaValidityWarningFunc) fprintf, stderr);
        r = xmlSchemaValidateDoc(ctxt, doc);
        xmlSchemaFreeValidCtxt(ctxt);

    }
    //xmlFreeDoc(doc);

    if(schema != NULL){
        xmlSchemaFree(schema);
    }

    
    xmlFreeDoc(doc);
    
    xmlCleanupParser();

    xmlMemoryDump();
    
    fclose(fp);

    if(r == 0){
        validation = true;
    }

    return validation;
    
}

/**
* img = pointer to SVG struct
* elemType = value indicating what struct: SVG_IMG, CIRC, RECT, PATH, GROUP
* elemIndex = index of element to modify in the element type list, ignored if we are modifying SVG_IMG
* newAttribute = the new attribute, will have a name and value (name = cx, value = 129.21)
* if an element we are setting exists then replace it, if it does not then append it to the current list
* if it worked return true
* if it returns false do not free newAttribute
*/
bool setAttribute(SVG* img, elementType elemType, int elemIndex, Attribute* newAttribute){
    if(img == NULL || newAttribute == NULL){
        return false;
    }
    // if(newAttribute->name == NULL){
    //     return false;
    // }
    bool updated = false;
    if(elemType == SVG_IMG){
        //add to svg->otherAttributes
        //Node* node = img->otherAttributes->head;
        updated = updateAttribute(img->otherAttributes,newAttribute);
        if(!updated){
            //add attribute to back
            insertBack(img->otherAttributes,newAttribute);
        }else{
            deleteAttribute(newAttribute);
        }
        
        return true;

    }else if(elemType == RECT){
        //take into account element index (which rectangle) and see if it exists
        
        if(img->rectangles->length <= elemIndex || elemIndex < 0){
            return false;
        }

        ListIterator iter = createIterator(img->rectangles);
        void* elem;
        
        //check to see if the element is set (x,y,width,height)
        int index = 0;
        //traverse the list until element index
        //if not then add it to otherAttributes of rectangle
        while((elem = nextElement(&iter)) != NULL){
            Rectangle *rect = (Rectangle*)(elem);

            if(index == elemIndex){
                if(strcmp("x",(char*)newAttribute->name) == 0){
                    //replace existing value
                    float tmp = atof(newAttribute->value);
                    rect->x = tmp;
                    updated = true;
                }else if(strcmp("y",newAttribute->name) == 0){
                    //replace existing value
                    float tmp = atof(newAttribute->value);
                    rect->y = tmp;
                    updated = true;
                    
                }
                else if(strcmp("width",newAttribute->name) == 0){
                    //replace existing value
                    float tmp = atof(newAttribute->value);
                    rect->width = tmp;
                    updated = true;
                    
                }
                else if(strcmp("height",newAttribute->name) == 0){
                    //replace existing value
                    float tmp = atof(newAttribute->value);
                    rect->height = tmp;
                    updated = true;
                }
                else{
                    //Node* node = rect->otherAttributes->head;
                    updated = updateAttribute(rect->otherAttributes,newAttribute);
                    if(!updated){
                        insertBack(rect->otherAttributes,(void*)newAttribute);
                    }
                }
            }
            index++;
        }
        if(updated){
            deleteAttribute((void*)newAttribute);
        }

        return true;

    }else if(elemType == CIRC){
        //take into account element index (which rectangle) and see if it exists
        if(img->circles->length <= elemIndex || elemIndex < 0){
            return false;
        }

        ListIterator iter = createIterator(img->circles);
        void* elem;
        
        //check to see if the element is set (x,y,width,height)
        int index = 0;
        //traverse the list until element index
        //if not then add it to otherAttributes of rectangle
        while((elem = nextElement(&iter)) != NULL){
            Circle *circle = (Circle*)(elem);

            if(index == elemIndex){
                if(strcmp("cx",newAttribute->name) == 0){
                    //replace existing value
                    float tmp = atof(newAttribute->value);
                    circle->cx = tmp;
                    updated = true;
                    
                }else if(strcmp("cy",newAttribute->name) == 0){
                    //replace existing value
                    float tmp = atof(newAttribute->value);
                    circle->cy = tmp;
                    updated = true;
                    
                }
                else if(strcmp("r",newAttribute->name) == 0){
                    //replace existing value
                    float tmp = atof(newAttribute->value);
                    circle->r = tmp;
                    updated = true;
                    
                }else{
                    //Node* node = circle->otherAttributes->head;
                    updated = updateAttribute(circle->otherAttributes,newAttribute);
                    if(!updated){
                        insertBack(circle->otherAttributes,(void*)newAttribute);
                    }

                }
            }
            index++;
        }
        if(updated){
            deleteAttribute((void*)newAttribute);
        }

        return true;
    }else if(elemType == PATH){
        //take into account element index (which rectangle) and see if it exists

        if(img->paths->length <= elemIndex || elemIndex < 0){
            return false;
        }

        ListIterator iter = createIterator(img->paths);
        void* elem;
        
        //check to see if the element is set (x,y,width,height)
        int index = 0;
        //traverse the list until element index
        //if not then add it to otherAttributes of rectangle
        Node* node = img->paths->head;
        while((elem = nextElement(&iter)) != NULL){
            Path *path = (Path*)(elem);
            if(index == elemIndex){
                if(strcmp("d",newAttribute->name) == 0){
                    //replace existing value
                    node->data = realloc(node->data,sizeof(Path)+((strlen((char*)newAttribute->value)+1)*sizeof(char)));
                    strcpy(((Path*)node->data)->data,(char*)newAttribute->value);
                    updated = true;
                    
                }else{
                    //Node* node = path->otherAttributes->head;
                    updated = updateAttribute(path->otherAttributes,newAttribute);
                    if(!updated){
                        insertBack(path->otherAttributes,(void*)newAttribute);
                        
                    }
                }
            }
            node = node->next;
            index++;
        }
        if(updated){
            deleteAttribute((void*)newAttribute);
        }
        
        return true;
    }else if(elemType == GROUP){
        //take into account element index (which rectangle) and see if it exists
        if(img->groups->length <= elemIndex || elemIndex < 0){
            return false;
        }

        ListIterator iter = createIterator(img->groups);
        void* elem;
        
        //check to see if the element is set (x,y,width,height)
        int index = 0;
        //traverse the list until element index
        //if not then add it to otherAttributes of rectangle
        while((elem = nextElement(&iter)) != NULL){
            Group *group = (Group*)(elem);

            if(index == elemIndex){
                //Node* node = group->otherAttributes->head;
                updated = updateAttribute(group->otherAttributes,newAttribute);
                if(!updated){
                    insertBack(group->otherAttributes,(void*)newAttribute);
                    
                }
            }
            index++;
        }
        if(updated){
            deleteAttribute((void*)newAttribute);
        }
        return true;
    }
    return false;

}

void addComponent(SVG* img, elementType type, void* newElement){
    
    if(img == NULL || newElement == NULL || type == SVG_IMG || type == GROUP){
        return;
    }
    if(type == RECT){
        insertBack(img->rectangles,newElement);
    }else if(type == CIRC){
        insertBack(img->circles,newElement);
    }else if(type == PATH){
        insertBack(img->paths,newElement);
    }
    return;
}

/** Function to converting an Attribute into a JSON string
*@pre Attribute is not NULL
*@post Attribute has not been modified in any way
*@return A string in JSON format
*@param event - a pointer to an Attribute struct
**/
char* attrToJSON(const Attribute *a){
    
    if(a == NULL){
        char *ret = malloc(3);
        sprintf(ret,"{}");
        return ret;
    }

    char* str = malloc(sizeof(char)*(strlen((char*)a->name)+strlen((char*)a->value)+50));
    sprintf(str,"{\"name\":\"%s\",\"value\":\"%s\"}",a->name,a->value);

    return str;
}

/** Function to converting a Circle into a JSON string
*@pre Circle is not NULL
*@post Circle has not been modified in any way
*@return A string in JSON format
*@param event - a pointer to a Circle struct
**/
char* circleToJSON(const Circle *c){
    if(c == NULL){
        char *ret = malloc(3);
        sprintf(ret,"{}");
        return ret;
    }

    int numA = c->otherAttributes->length;

    char* str = malloc(sizeof(char)*(strlen((char*)c->units)+256));

    sprintf(str,"{\"cx\":%.2f,\"cy\":%.2f,\"r\":%.2f,\"numAttr\":%d,\"units\":\"%s\"}",c->cx,c->cy,c->r,numA,c->units);

    return str;
}

/** Function to converting a Rectangle into a JSON string
*@pre Rectangle is not NULL
*@post Rectangle has not been modified in any way
*@return A string in JSON format
*@param event - a pointer to a Rectangle struct
**/
char* rectToJSON(const Rectangle *r){
    if(r == NULL){
        char *ret = malloc(3);
        sprintf(ret,"{}");
        return ret;
    }

    int numA = r->otherAttributes->length;

    char* str = malloc(sizeof(char)*(strlen((char*)r->units)+256));

    sprintf(str,"{\"x\":%.2f,\"y\":%.2f,\"w\":%.2f,\"h\":%.2f,\"numAttr\":%d,\"units\":\"%s\"}",r->x,r->y,r->width,r->height,numA,r->units);

    return str;
}

/** Function to converting a Path into a JSON string
*@pre Path is not NULL
*@post Path has not been modified in any way
*@return A string in JSON format
*@param event - a pointer to a Path struct
**/
char* pathToJSON(const Path *p){

    if(p == NULL){
        char *ret = malloc(3);
        sprintf(ret,"{}");
        return ret;
    }

    int numA = p->otherAttributes->length;

    int dataLen = strlen((char*)p->data);

    char* str = malloc(sizeof(char)*(strlen((char*)p->data)+256));

    if(dataLen > 64){
        char* copy = malloc(sizeof(char)*(strlen((char*)p->data)+1));
        strcpy(copy,(char*)p->data);
        copy[64] = '\0';
        sprintf(str,"{\"d\":\"%s\",\"numAttr\":%d}",copy,numA);
        free(copy);
    }else{
        sprintf(str,"{\"d\":\"%s\",\"numAttr\":%d}",p->data,numA);
    }

    return str;
    
}

/** Function to converting a Group into a JSON string
*@pre Group is not NULL
*@post Group has not been modified in any way
*@return A string in JSON format
*@param event - a pointer to a Group struct
**/
char* groupToJSON(const Group *g){
    if(g == NULL){
        char *ret = malloc(3);
        sprintf(ret,"{}");
        return ret;
    }

    int numA = g->otherAttributes->length;

    int numChildren = g->rectangles->length;
    numChildren += g->circles->length;
    numChildren += g->paths->length;
    numChildren += g->groups->length;

    char* str = malloc(254);

    sprintf(str,"{\"children\":%d,\"numAttr\":%d}",numChildren,numA);

    return str;
}

/** Function to converting an SVG into a JSON string
*@pre SVG is not NULL
*@post SVG has not been modified in any way
*@return A string in JSON format
*@param event - a pointer to an SVG struct
**/
char* SVGtoJSON(const SVG* img){
    if(img == NULL){
        char *ret = malloc(3);
        sprintf(ret,"{}");
        return ret;
    }

    List* rects = getRects(img);
    List* circles = getCircles(img);
    List* paths = getPaths(img);
    List* groups = getGroups(img);

    char* str = malloc(256);

    sprintf(str,"{\"numRect\":%d,\"numCirc\":%d,\"numPaths\":%d,\"numGroups\":%d}",rects->length,circles->length,paths->length,groups->length);

    freeList(rects);
    freeList(circles);
    freeList(paths);
    freeList(groups);

    return str;
}
/** Function to converting a list of Attribute structs into a JSON string
*@pre Attribute list is not NULL
*@post Attribute list has not been modified in any way
*@return A string in JSON format
*@param event - a pointer to a List struct
**/
char* attrListToJSON(const List *list){
    if(list == NULL || (list->length == 0)){
        char *ret = malloc(3);
        sprintf(ret,"[]");
        return ret;
    }

    char* aJSON = malloc(100);
    strcpy(aJSON,"");

    ListIterator iter = createIterator((List*)list);
    void* elem;
    while((elem = nextElement(&iter)) != NULL){
        Attribute *attr = (Attribute*)(elem);
        char* attrStr = attrToJSON(attr);
        char* buff = malloc(sizeof(char)*(strlen(attrStr)+2));
        strcpy(buff,attrStr);
        strcat(buff,",");
        aJSON = (char*)realloc(aJSON,(sizeof(char)*(strlen(aJSON)+strlen(buff)+1)));
        strcat(aJSON,buff);
        free(buff);
        free(attrStr);
    }

    aJSON[strlen(aJSON)-1] = '\0';

    char* str = malloc(sizeof(char)*strlen(aJSON)+3);

    sprintf(str,"[%s]",aJSON);
    free(aJSON);

    return str;
    
}

/** Function to converting a list of Circle structs into a JSON string
*@pre Circle list is not NULL
*@post Circle list has not been modified in any way
*@return A string in JSON format
*@param event - a pointer to a List struct
**/
char* circListToJSON(const List *list){
    
    if(list == NULL || (list->length == 0)){
        char *ret = malloc(3);
        sprintf(ret,"[]");
        return ret;
    }

    char* cJSON = malloc(100);
    strcpy(cJSON,"");

    ListIterator iter = createIterator((List*)list);
    void* elem;
    while((elem = nextElement(&iter)) != NULL){
        Circle *circ = (Circle*)(elem);
        char* circStr = circleToJSON(circ);
        char* buff = malloc(sizeof(char)*(strlen(circStr)+2));
        strcpy(buff,circStr);
        strcat(buff,",");
        cJSON = (char*)realloc(cJSON,(sizeof(char)*(strlen(cJSON)+strlen(buff)+1)));
        strcat(cJSON,buff);
        free(buff);
        free(circStr);
    }

    cJSON[strlen(cJSON)-1] = '\0';

    char* str = malloc(sizeof(char)*strlen(cJSON)+3);

    sprintf(str,"[%s]",cJSON);
    free(cJSON);

    return str;
}

/** Function to converting a list of Rectangle structs into a JSON string
*@pre Rectangle list is not NULL
*@post Rectangle list has not been modified in any way
*@return A string in JSON format
*@param event - a pointer to a List struct
**/
char* rectListToJSON(const List *list){
        
    if(list == NULL || (list->length == 0)){
        char *ret = malloc(3);
        sprintf(ret,"[]");
        return ret;
    }

    char* rJSON = malloc(100);
    strcpy(rJSON,"");

    ListIterator iter = createIterator((List*)list);
    void* elem;
    while((elem = nextElement(&iter)) != NULL){
        Rectangle *rect = (Rectangle*)(elem);
        char* rectStr = rectToJSON(rect);
        char* buff = malloc(sizeof(char)*(strlen(rectStr)+2));
        strcpy(buff,rectStr);
        strcat(buff,",");
        rJSON = (char*)realloc(rJSON,(sizeof(char)*(strlen(rJSON)+strlen(buff)+1)));
        strcat(rJSON,buff);
        free(buff);
        free(rectStr);
    }

    rJSON[strlen(rJSON)-1] = '\0';

    char* str = malloc(sizeof(char)*strlen(rJSON)+3);

    sprintf(str,"[%s]",rJSON);
    free(rJSON);

    return str;
}

/** Function to converting a list of Path structs into a JSON string
*@pre Path list is not NULL
*@post Path list has not been modified in any way
*@return A string in JSON format
*@param event - a pointer to a List struct
**/
char* pathListToJSON(const List *list){
            
    if(list == NULL || (list->length == 0)){
        char *ret = malloc(3);
        sprintf(ret,"[]");
        return ret;
    }

    char* pJSON = malloc(100);
    strcpy(pJSON,"");

    ListIterator iter = createIterator((List*)list);
    void* elem;
    while((elem = nextElement(&iter)) != NULL){
        Path *path = (Path*)(elem);
        char* pathStr = pathToJSON(path);
        char* buff = malloc(sizeof(char)*(strlen(pathStr)+2));
        strcpy(buff,pathStr);
        strcat(buff,",");
        pJSON = (char*)realloc(pJSON,(sizeof(char)*(strlen(pJSON)+strlen(buff)+1)));
        strcat(pJSON,buff);
        free(buff);
        free(pathStr);
    }

    pJSON[strlen(pJSON)-1] = '\0';

    char* str = malloc(sizeof(char)*strlen(pJSON)+3);

    sprintf(str,"[%s]",pJSON);
    free(pJSON);

    return str;
}

/** Function to converting a list of Group structs into a JSON string
*@pre Group list is not NULL
*@post Group list has not been modified in any way
*@return A string in JSON format
*@param event - a pointer to a List struct
**/
char* groupListToJSON(const List *list){
                
    if(list == NULL || (list->length == 0)){
        char *ret = malloc(3);
        sprintf(ret,"[]");
        return ret;
    }

    char* groupJSON = malloc(100);
    strcpy(groupJSON,"");

    ListIterator iter = createIterator((List*)list);
    void* elem;
    while((elem = nextElement(&iter)) != NULL){
        Group *group = (Group*)(elem);
        char* groupStr = groupToJSON(group);
        char* buff = malloc(sizeof(char)*(strlen(groupStr)+2));
        strcpy(buff,groupStr);
        strcat(buff,",");
        groupJSON = (char*)realloc(groupJSON,(sizeof(char)*(strlen(groupJSON)+strlen(buff)+1)));
        strcat(groupJSON,buff);
        free(buff);
        free(groupStr);
    }

    groupJSON[strlen(groupJSON)-1] = '\0';

    char* str = malloc(sizeof(char)*strlen(groupJSON)+3);

    sprintf(str,"[%s]",groupJSON);
    free(groupJSON);

    return str;
}


/* ******************************* Bonus A2 functions - optional for A2 *************************** */

SVG* JSONtoSVG(const char* svgString){
    return NULL;
}

Rectangle* JSONtoRect(const char* svgString){
    return NULL;
}

Circle* JSONtoCircle(const char* svgString){
    return NULL;
}
