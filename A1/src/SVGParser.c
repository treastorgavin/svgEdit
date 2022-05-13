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
        //fprintf(stderr, "Failed to parse %s\n", fileName);
        return NULL;
    }

    //get the root element node
    root_element = xmlDocGetRootElement(doc);

    if(root_element == NULL){
        return NULL;
    }

    // recursive helper to traverse the tree
    SVG* svg = get_SVG_elements(root_element);

    if(svg == NULL){
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

    List* groups = initializeList(&groupToString,&deleteDummy,&compareGroups);
    // if((img->groups->length) > 0){
    //     groups = iterList(img->groups,"groups");
    // }
    if((img->groups->length) > 0){
        ListIterator iter = createIterator(img->groups);
        void* elem;
        while((elem = nextElement(&iter)) != NULL){
            insertBack(groups,elem);
        }
    }

    return groups;
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


    if(img->paths->length <= 0){
        return 0;
    }else{
        ListIterator iter = createIterator(img->paths);
        void* elem;
        while((elem = nextElement(&iter)) != NULL){
            //compare intArea with area of rectangle
            Path *path = (Path*)(elem);
            
            char* pathData = malloc((strlen((char*)path->data))*(sizeof(char))+1);
            strcpy(pathData,(char*)path->data);

            if(strcmp(pathData,data) == 0){
                numPaths++;
            }
            free(pathData);
        }
    }
    if(img->groups->length > 0){
        numPaths += numPathsGroup(img->groups,data);
    }

    return numPaths;
}
// Function that returns the number of all groups with the specified length - see A1 Module 2 for details
int numGroupsWithLen(const SVG* img, int len){
    if(img == NULL || (len < 0)){
        return 0;
    }

    int numGroups = 0;

    if(img->groups->length <= 0){
        return 0;
    }else{
        ListIterator iter = createIterator(img->groups);
        void* elem;
        while((elem = nextElement(&iter)) != NULL){
            int count = 0;
            Group *group = (Group*)(elem);

            count += group->rectangles->length;
            count += group->circles->length;
            count += group->paths->length;
            if(group->groups->length > 0){
                count += groupCount(group->groups,len);
            }
            if(count == len){
                numGroups++;
            }

        }
    }

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
