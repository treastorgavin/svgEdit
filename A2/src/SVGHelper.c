/**
Gavin Rotsaert-Smith
0997431
**/

#include "SVGParser.h"
#include "SVGHelper.h"

//This parses the SVG and returns an SVG struct
SVG* get_SVG_elements(xmlNode *a_node){
    xmlNode *cur_node = NULL;

    //initializing the SVG struct
    SVG *temp = (SVG*)malloc(sizeof(SVG));

    temp->rectangles = initializeList(&rectangleToString,&deleteRectangle,&compareRectangles);
    temp->circles = initializeList(&circleToString,&deleteCircle,&compareCircles);
    temp->paths = initializeList(&pathToString,&deletePath,&comparePaths);
    temp->groups = initializeList(&groupToString,&deleteGroup,&compareGroups);
    temp->otherAttributes = initializeList(&attributeToString,&deleteAttribute,&compareAttributes);

    memcpy(temp->namespace,"",256);
    strcpy(temp->title,"");
    strcpy(temp->description,"");

    if((char*)a_node->ns == NULL){
        return NULL;
    }

    char* tempNamespace = malloc(sizeof(char)*(strlen((char*)a_node->ns->href))+1);
    strcpy(tempNamespace,(char*)(a_node->ns->href));

    if(strlen(tempNamespace)>256){
        tempNamespace[255] = '\0';
    }
    strcpy(temp->namespace,tempNamespace);

    free(tempNamespace);

    //to compare element name with element we want
    char svgElement[16];
    strcpy(svgElement,"svg");

    char titleElement[16];
    strcpy(titleElement,"title");
    char descElement[16];
    strcpy(descElement,"desc");
    char rectElement[16];
    strcpy(rectElement,"rect");
    char circleElement[16];
    strcpy(circleElement,"circle");
    char pathElement[16];
    strcpy(pathElement,"path");
    char groupElement[16];
    strcpy(groupElement,"g");
    

    xmlAttr * attribute;
    xmlNode* tmpNode = a_node;
    for(attribute = tmpNode->properties; attribute != NULL; attribute = attribute->next){

        //xmlAttr *tmpAttr = tmpNode->properties;

        //while(tmpNode)
        insertBack(temp->otherAttributes,(void*)createAttribute(attribute));

    }

    xmlNode * iterNode = a_node->children;
    while(iterNode != NULL){
        for(cur_node = iterNode; cur_node != NULL; cur_node = cur_node->next){
            if(cur_node->type == XML_ELEMENT_NODE){
                //printf("node type: Element, name: %s\n",cur_node->name);

                char* elementName = malloc(sizeof(char)*(strlen((char*)cur_node->name))+1);
                strcpy(elementName,(char*)(cur_node->name));
                
                //check to see what element we are on
                // if(strcmp(elementName,svgElement) == 0){

                // }
                
                if(strcmp(elementName,titleElement) == 0){
                    xmlNode *tmpNode = cur_node;

                    char *cont = malloc(sizeof(char)*(strlen((char *)(tmpNode->children->content)))+1);
                    strcpy(cont,(char *)(tmpNode->children->content));

                    //printf("%s\n", cont);
                    if(strlen(cont)>256){
                        cont[255] = '\0';
                    }
                    strcpy(temp->title,cont);

                    free(cont);

                }
                
                if(strcmp(elementName,descElement) == 0){
                    xmlNode *tmpNode = cur_node;

                    char *cont = malloc(sizeof(char)*(strlen((char *)(tmpNode->children->content)))+1);
                    strcpy(cont,(char *)(tmpNode->children->content));

                    //printf("%lu\n", strlen(cont));
                    if(strlen(cont)>256){
                        cont[255] = '\0';
                    }
                    //printf("%lu\n", strlen(cont));
                    strcpy(temp->description,cont);

                    free(cont);
                }
                
                if(strcmp(elementName,rectElement) == 0){
                    insertBack(temp->rectangles, (void*)createRectangle(cur_node));
                }

                //CIRCLE
                
                if(strcmp(elementName,circleElement) == 0){
                    insertBack(temp->circles, (void*)createCircle(cur_node));
                }
                
                //PATH
                
                if(strcmp(elementName,pathElement) == 0){
                    insertBack(temp->paths, (void*)createPath(cur_node));
                }
                
                //GROUP
                
                if(strcmp(elementName,groupElement) == 0){
                    insertBack(temp->groups,(void*)createGroup(cur_node));
                }

                free(elementName);
            }
            //recursive call here where the root node is replaced by the child node of that node
        }
        if(iterNode->children != NULL){
            iterNode = iterNode->children;
        }else{
            iterNode = NULL;
        }
        
    }

    return temp;
}

Group * createGroup(xmlNode *cur_node){
    // if(group == NULL){
    //     return NULL;
    // }

    Group *group = (Group*)malloc(sizeof(Group));

    group->rectangles = initializeList(&rectangleToString,&deleteRectangle,&compareRectangles);
    group->circles = initializeList(&circleToString,&deleteCircle,&compareCircles);
    group->paths = initializeList(&pathToString,&deletePath,&comparePaths);
    group->groups = initializeList(&groupToString,&deleteGroup,&compareGroups);
    group->otherAttributes = initializeList(&attributeToString,&deleteAttribute,&compareAttributes);

    char rectElement[16];
    strcpy(rectElement,"rect");
    char circleElement[16];
    strcpy(circleElement,"circle");
    char pathElement[16];
    strcpy(pathElement,"path");
    char groupElement[16];
    strcpy(groupElement,"g");

    xmlNode * iterNode = cur_node->children;

    xmlAttr * attribute;
    xmlNode* tmpNode = cur_node;
    for(attribute = tmpNode->properties; attribute != NULL; attribute = attribute->next){

        insertBack(group->otherAttributes,(void*)createAttribute(attribute));

    }

    while(iterNode != NULL){
        for(cur_node = iterNode; cur_node != NULL; cur_node = cur_node->next){
           // printf("node type: Element, name: %s\n",cur_node->name);

            char* elementName = malloc(sizeof(char)*(strlen((char*)(cur_node->name)))+1);
            strcpy(elementName,(char*)(cur_node->name)); 
            
            if(strcmp(elementName,rectElement) == 0){
                insertBack(group->rectangles, (void*)createRectangle(cur_node));
            }

            //CIRCLE
            
            if(strcmp(elementName,circleElement) == 0){
                insertBack(group->circles, (void*)createCircle(cur_node));
            }
            
            //PATH
            
            if(strcmp(elementName,pathElement) == 0){
                insertBack(group->paths, (void*)createPath(cur_node));
            }
            
            //GROUP
            
            if(strcmp(elementName,groupElement) == 0){
                insertBack(group->groups,(void*)createGroup(cur_node));
            }

            free(elementName);
            //recursive call here where the root node is replaced by the child node of that node
        }
        if(iterNode->children != NULL){
            iterNode = iterNode->children;
        }else{
            iterNode = NULL;
        }
        
    }

    return group;
}

//function that makes an attribute struct based on the current node
Attribute * createAttribute(xmlAttr *attribute){

    xmlNode *value = attribute->children;

    char *attrName = malloc(sizeof(char)*(strlen((char *)(attribute->name))+1));
    strcpy(attrName, (char *)(attribute->name));

    char *cont = malloc(sizeof(char)*(strlen((char *)(value->content)))+1);
    strcpy(cont,(char *)(value->content));

    Attribute *attr = (Attribute*)malloc(sizeof(Attribute) + strlen(cont)*sizeof(char)+1);

    // if(attr == NULL){
    //     //fprintf(stderr,"malloc failed\n");
    //     return NULL;
    // }

    attr->name = malloc(sizeof(char)*strlen(attrName)+1);
    strcpy(attr->name,"");
    strcpy(attr->value,"");
    
    //printf("Attribute name: %s, attribute value = %s \n", attrName, cont);

    //printf("attrName: %s\n", attrName);
    strcpy(attr->name, attrName);
    //printf("wtf are you %s\n",cont);
    if(cont != NULL){
        strcpy(attr->value,cont);
    }else{
        strcpy(attr->value,"");
    }
    //printf("attr value: %s\n", (char*)cont);

    free(attrName);
    free(cont);

    return attr;
}

Rectangle* createRectangle(xmlNode *cur_node){
    xmlNode* tmpNode = cur_node;
    Rectangle *rect = (Rectangle*)malloc(sizeof(Rectangle));

    // if(rect == NULL){
    //     return NULL;
    // }

    //initializing
    rect->x = 0;
    rect->y = 0;
    rect->width = 0;
    rect->height = 0;
    memcpy(rect->units,"",50);
    rect->otherAttributes = initializeList(&attributeToString,&deleteAttribute,&compareAttributes);;

    xmlAttr *attribute;

    for(attribute = tmpNode->properties; attribute != NULL; attribute = attribute->next){
        xmlNode *value = attribute->children;

        char *attrName = malloc(sizeof(char)*(strlen((char *)(attribute->name))+1));
        strcpy(attrName, (char *)(attribute->name));

        char *cont = malloc(sizeof(char)*(strlen((char *)(value->content)))+1);
        strcpy(cont,(char *)(value->content));

        //printf("Attribute name: %s, attribute value = %s \n", attrName, cont);

        //need to check to see if these elements are present (x,y,...)
        if(strcmp(attrName,"x") == 0){
            rect->x = strtof(cont,NULL);
            char* units = getUnits(cont);
            strncpy(rect->units,units,strlen(units));
            free(units);
        }
        else if(strcmp(attrName,"y") == 0){
            rect->y = strtof(cont,NULL);
            
        }
        else if(strcmp(attrName,"width") == 0){
            rect->width = strtof(cont,NULL);
        }
        else if(strcmp(attrName,"height") == 0){
            
            rect->height = strtof(cont,NULL);
        }
        //need to check for otherAttributes
        else if((strcmp(attrName,"units") != 0) || (strcmp(attrName,"height") != 0)||(strcmp(attrName,"width")!=0) || (strcmp(attrName,"y")!=0) || (strcmp(attrName,"x") != 0)){
            xmlAttr *tmpAttr = attribute;

            Attribute *attr = createAttribute(tmpAttr);
            insertBack(rect->otherAttributes,(void*)attr);
            // tmpAttr = tmpAttr->next;
        }

        free(attrName);
        free(cont);
    }


    //add rect to List* reactangles
    return rect;

}

Circle* createCircle(xmlNode *cur_node){
    xmlNode* tmpNode = cur_node;
    Circle *circ = (Circle*)malloc(sizeof(Circle));

    List* circotherAttributesList = initializeList(&attributeToString,&deleteAttribute,&compareAttributes);
    //initializing circle
    circ->cx = 0;
    circ->cy = 0;
    circ->r = 0;
    memcpy(circ->units,"",50);
    circ->otherAttributes = circotherAttributesList;

    xmlAttr *attribute;

    for(attribute = tmpNode->properties; attribute != NULL; attribute = attribute->next){

        xmlNode *value = attribute->children;

        char *attrName = malloc(sizeof(char)*(strlen((char *)(attribute->name))+1));
        strcpy(attrName, (char *)(attribute->name));

        char *cont = malloc(sizeof(char)*(strlen((char *)(value->content)))+1);
        strcpy(cont,(char *)(value->content));

        //need to check to see if these elements are present (x,y,...)
        if(strcmp(attrName,"cx") == 0){
           
            circ->cx = strtof(cont,NULL);
        }
        else if(strcmp(attrName,"cy") == 0){
            
            circ->cy = strtof(cont,NULL);
        }
        else if(strcmp(attrName,"r") == 0){

            circ->r = strtof(cont,NULL);
            char* units = getUnits(cont);
            strncpy(circ->units,units,strlen(units));
            free(units);
            //printf("%f\n",circ->r);
        }
        else if(strcmp(attrName,"units") == 0){
            strcpy(circ->units,(char*)cont);
        }
        //need to check for otherAttributes
        else{
            xmlAttr *tmpAttr = attribute;

            Attribute *attr = createAttribute(tmpAttr);
            insertBack(circ->otherAttributes,(void*)attr);
            // tmpAttr = tmpAttr->next;
        }

        free(attrName);
        free(cont);
    }

    return circ;

}

Path* createPath(xmlNode *cur_node){
    xmlNode* tmpNode = cur_node;

    xmlAttr* attribute = tmpNode->properties;

    xmlNode *value = attribute->children;

    char *cont = malloc(sizeof(char)*(strlen((char*)value->content)+1));
    strcpy(cont,(char *)(value->content));


    char* dat = (char*)xmlGetProp(cur_node,(const xmlChar*)"d");

    Path *path = (Path*)malloc(sizeof(Path)+((strlen(cont)+strlen((char*)dat)+1)*sizeof(char)));
    xmlFree(dat);

    //Path *path = (Path*)malloc(sizeof(Path)+((strlen(cont)+1)*sizeof(char)+1));

    //initialize data
    strcpy(path->data,"");
    
    path->otherAttributes = initializeList(&attributeToString,&deleteAttribute,&compareAttributes);

    for(attribute = tmpNode->properties; attribute != NULL; attribute = attribute->next){
        
        xmlNode *value = attribute->children;

        char *attrName = malloc(sizeof(char)*(strlen((char *)(attribute->name))+1));
        strcpy(attrName, (char *)(attribute->name));

        char *cont = malloc(sizeof(char)*(strlen((char*)value->content)+1));
        strcpy(cont,(char *)(value->content));

        if(cont == NULL){
            //fprintf(stderr,"Error: data cannot be < 0");
            return NULL;
        }
    

        if(strcmp(attrName,"d") == 0){
            strcpy(path->data,cont);
        }
        //need to check for otherAttributes

        else{
            xmlAttr *tmpAttr = attribute;

            Attribute *attr = createAttribute(tmpAttr);
            insertBack(path->otherAttributes,(void*)attr);
            // tmpAttr = tmpAttr->next;
        }

        free(attrName);
        free(cont);
    
    }

    free(cont);
   
    return path;

}

char *getUnits(char* string){
    char* units = malloc(sizeof(char)*10);

    strcpy(units,"");

    //printf("%s\n",string);
    
    for(int i = 0; i < strlen(string)-1; i++){
        if((string[i] >= 'A' && string[i] <= 'Z') || (string[i] >= 'a' && string[i] <= 'z')){
            strcat(units,&string[i]);
        }
    }

    return units;
}


int groupCount(List* list,int len){
    int numGroups = 0;
    ListIterator itr = createIterator(list);
    void* data;
    while((data = nextElement(&itr)) != NULL){
        //compare intArea with area of rectangle
        Group *group = (Group*)(data);

        numGroups += group->rectangles->length;
        numGroups += group->circles->length;
        numGroups += group->paths->length;
        if(group->groups->length > 0){
            numGroups += groupCount(group->groups,len);
        }
    }

    if(numGroups != len){
        return 0;
    }

    return numGroups;
}

int groupAttrCount(List *list){
    int numAttributes = 0;
    ListIterator itr = createIterator(list);
    void* data;
    while((data = nextElement(&itr)) != NULL){
        //compare intArea with area of rectangle
        Group *group = (Group*)(data);
        if(group->otherAttributes->length > 0){
            numAttributes += group->otherAttributes->length;
        }
        if(group->rectangles->length > 0){
            ListIterator iter = createIterator(group->rectangles);
            void* elem;
            while((elem = nextElement(&iter)) != NULL){
                Rectangle *rect = (Rectangle*)(elem);
                numAttributes += rect->otherAttributes->length;
            }
        }
        if(group->circles->length > 0){
            ListIterator iter = createIterator(group->circles);
            void* elem;
            while((elem = nextElement(&iter)) != NULL){
                Circle *circ = (Circle*)(elem);
                numAttributes += circ->otherAttributes->length;
            }
        }
        if(group->paths->length > 0){
            ListIterator iter = createIterator(group->paths);
            void* elem;
            while((elem = nextElement(&iter)) != NULL){
                Path *path = (Path*)(elem);
                numAttributes += path->otherAttributes->length;
            }
        }
        if(group->groups->length > 0){
            ListIterator iter = createIterator(group->groups);
            void* elem;
            while((elem = nextElement(&iter)) != NULL){
                Group *group = (Group*)(elem);
                numAttributes += group->otherAttributes->length;
                if(group->groups->length > 0){
                    numAttributes += groupAttrCount(group->groups);
                }
            }
        }
    }

    return numAttributes;
}

void deleteDummy(void *data){};

List* iterListRect(List *list){
    if(list == NULL){
        return NULL;
    }
    List* shapeList = initializeList(&rectangleToString,&deleteDummy,&compareRectangles);
    ListIterator iter = createIterator(list);
    void* elem;
    while((elem = nextElement(&iter)) != NULL){
        Group* group = (Group*)elem;
        if((group->rectangles->length) > 0){
            ListIterator iter = createIterator(group->rectangles);
            void* elem;
            while((elem = nextElement(&iter)) != NULL){
                //Rectangle* rect = (Rectangle*)elem;
                insertBack(shapeList,elem);
            }
        }
        if(group->groups->length > 0){
            //recursive call here pass in groups->groups
            appendLists(shapeList, iterListRect(group->groups));
            //append the shapelist from the recursive call to the previous shapList
        }
    }
    return shapeList;
}

List* iterListCircles(List *list){
    if(list == NULL){
        return NULL;
    }
    List* shapeList = initializeList(&circleToString,&deleteDummy,&compareCircles);
    ListIterator iter = createIterator(list);
    void* elem;
    while((elem = nextElement(&iter)) != NULL){
        Group* group = (Group*)elem;
        if((group->circles->length) > 0){
            ListIterator iter = createIterator(group->circles);
            void* elem;
            while((elem = nextElement(&iter)) != NULL){
                //inserting circle pointer into shapelist
                insertBack(shapeList,elem);
            }

        }
        if((group->groups->length) > 0){
            //recursive call here pass in groups->groups
            appendLists(shapeList, iterListCircles(group->groups));
            //append the shapelist from the recursive call to the previous shapList
        }
    }
    return shapeList;
}

List* iterListPaths(List *list){
    if(list == NULL){
        return NULL;
    }
    List* shapeList = initializeList(&pathToString,&deleteDummy,&comparePaths);
    ListIterator iter = createIterator(list);
    void* elem;
    while((elem = nextElement(&iter)) != NULL){
        Group* group = (Group*)elem;
        if((group->paths->length) > 0){
            ListIterator iter = createIterator(group->paths);
            void* elem;
            while((elem = nextElement(&iter)) != NULL){
                //Path* path = (Path*)elem;
                insertBack(shapeList,elem);
            }

        }
        if(group->groups->length > 0){
            //recursive call here pass in groups->groups
            appendLists(shapeList, iterListPaths(group->groups));
            //append the shapelist from the recursive call to the previous shapList
        }
    }
    return shapeList;
}

List* iterListGroups(List *list){
    if(list == NULL){
        return NULL;
    }
    List* shapeList = initializeList(&groupToString,&deleteDummy,&compareGroups);
    ListIterator iter = createIterator(list);
    void* elem;
    while((elem = nextElement(&iter)) != NULL){
        Group* group = (Group*)elem;
        
        ListIterator iter = createIterator(group->groups);
        void* elem;
        while((elem = nextElement(&iter)) != NULL){
            insertBack(shapeList,elem);
        }

        if(group->groups->length > 0){
            //recursive call here pass in groups->groups
            appendLists(shapeList, iterListGroups(group->groups));
            //append the shapelist from the recursive call to the previous shapList
        }
    }
    return shapeList;
}

int numCircleGroups(List *list, float area){

    int numCircs = 0;
    ListIterator itr = createIterator(list);
    void* data;
    while((data = nextElement(&itr)) != NULL){
        //compare intArea with area of rectangle
        Group *group = (Group*)(data);
        if(group->circles->length > 0){
            ListIterator itr = createIterator(group->circles);
            void* data;
            while((data = nextElement(&itr)) != NULL){
                Circle *circ = (Circle*)(data);
                float radius = circ->r;

                int circleArea = ceil(3.14159265358979323846*radius*radius);
                if(circleArea == ceil(area)){
                    numCircs++;
                }

            }
        }
        if(group->groups->length > 0){
            numCircs += numCircleGroups(group->groups,area);
        }
        
    }
    return numCircs;
}

int numRectGroups(List* list,float area){
    int numRects = 0;
    ListIterator itr = createIterator(list);
    void* data;
    while((data = nextElement(&itr)) != NULL){
        //compare intArea with area of rectangle
        Group *group = (Group*)(data);
        if(group->rectangles->length > 0){
            ListIterator itr = createIterator(group->rectangles);
            void* data;
            while((data = nextElement(&itr)) != NULL){
                Rectangle *rect = (Rectangle*)(data);
                float height = rect->height;
                float width = rect->width;

                int rectArea = ceil(height*width);
                if(rectArea == ceil(area)){
                    numRects++;
                }

            }
        }
        if(group->groups->length > 0){
            numRects += numRectGroups(group->groups,area);
        }
        
    }
    return numRects;
}

int numPathsGroup(List* list, const char* data){
    int numPaths = 0;
    ListIterator itr = createIterator(list);
    void* elem;
    while((elem = nextElement(&itr)) != NULL){
        //compare intArea with area of rectangle
        Group *group = (Group*)(elem);
        if(group->paths->length > 0){
            ListIterator itr = createIterator(group->paths);
            void* elem;
            while((elem = nextElement(&itr)) != NULL){
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
        if(group->groups->length > 0){
            numPaths += numPathsGroup(group->groups,data);
        }
        
    }
    return numPaths;
}


void appendLists(List *list, List* listToAdd){
    if(list == NULL || listToAdd == NULL){
        return;
    }

    ListIterator itr = createIterator(listToAdd);
    void* elem;
    while((elem = nextElement(&itr)) != NULL){
        insertBack(list,elem);
    }

    freeList(listToAdd);
    return;
}

//converts SVG struct to an xmlDoc struct i.e. a libxml tree
xmlDoc* svgtoTree(const SVG* img){
    
    xmlDoc* doc = NULL;
    xmlNodePtr root_node = NULL;

    //create new doc and set root node
    doc = xmlNewDoc(BAD_CAST "1.0");
    
    //should this be the svg node?? the add contents to root_node(svg)?

    root_node = xmlNewNode(NULL, BAD_CAST "svg");
    xmlDocSetRootElement(doc,root_node);
    
    char *value = malloc(sizeof(char)*(strlen(img->namespace)+1));
    strcpy(value,(char*)img->namespace);
    xmlNsPtr ns = xmlNewNs(root_node, BAD_CAST value, NULL);

    xmlSetNs(root_node,ns);

    //for adding namespace need to creat a new namespace
    // xmlNewNs
    // then to set it use xmlSetNs
    
    // xmlNewNs(root_node, (const xmlChar*) value, NULL);
    // xmlSetNs(root_node, );

    // xmlNewProp(root_node,(const xmlChar*)"namespace",(const xmlChar*)value);
    //char *value = malloc(strlen((img->title)+1)*sizeof(char));
    if(strcmp(img->title,"") != 0){
        value = (char*)realloc(value,(strlen(img->title)+1)*sizeof(char));
        strcpy(value,(char*)img->title);
        xmlNewChild(root_node,NULL,BAD_CAST "title",BAD_CAST value);
    }
    // xmlNodePtr title = xmlNewNode(NULL,(const xmlChar*)"title");
    // value = (char*)realloc(value,(strlen(img->title)+1)*sizeof(char));
    // strcpy(value,(char*)img->title);
    // xmlSetNs(title,NULL);
    // xmlNodePtr titleText = xmlNewText(BAD_CAST value);
    // xmlAddChild(title,titleText);
    // xmlAddChild(root_node,title);

    //DESCRIPTION
    if(strcmp(img->description,"") != 0){
        value = (char*)realloc(value,(strlen(img->description)+1)*sizeof(char));
        strcpy(value,(char*)img->description);
        xmlNewChild(root_node,NULL,BAD_CAST "desc",BAD_CAST value);
    }
    // xmlNodePtr desc = xmlNewNode(NULL,(const xmlChar*)"desc");
    // value = (char*)realloc(value,(strlen(img->description)+1)*sizeof(char));
    // strcpy(value,(char*)img->description);
    // xmlSetNs(desc,NULL);
    // xmlNodePtr descText = xmlNewText(BAD_CAST value);
    // xmlAddChild(desc,descText);
    // xmlAddChild(root_node,desc);

    free(value);

    //create a new node which is attached as a child node of root_node
    /** order of nodes
    *Rects <rect>
    *Circles <circle>
    *Paths <path>
    *groups <g>
    */
    //use xmlAddProp

    /* 
     * xmlNewProp() creates attributes, which is "attached" to an node.
     * It returns xmlAttrPtr, which isn't used here.
     */
    //xmlNewProp(node,(const xmlChar*)"height",(const xmlChar*)value);
    // parse the height from char to float to get rid of quotes from char
    /*
     * Here goes another way to create nodes. xmlNewNode() and xmlNewText
     * creates a node and a text node separately. They are "attached"
     * by xmlAddChild() 
     */
    // node = xmlNewNode(NULL, BAD_CAST "node4");
    // node1 = xmlNewText(BAD_CAST
    //                "other way to create content (which is also a node)");
    // //add the text to the node
    // xmlAddChild(node, node1);
    // //add that node with text to the root_node (root_node is svg node)
    // xmlAddChild(root_node, node);

    if(img->otherAttributes->length > 0){
        char *buff = malloc(256);
        ListIterator iter = createIterator(img->otherAttributes);
        void* elem;
        // xmlNewNode(NULL,(const xmlChar*)"node");
        while((elem = nextElement(&iter)) != NULL){
            Attribute *attr = (Attribute*)(elem);
            xmlNewProp(root_node, (const xmlChar*) attr->name, (const xmlChar*) attr->value);
        }
        free(buff);
    }
    if(img->rectangles->length > 0){
        rectToTree(img->rectangles,root_node);
    }
    if(img->circles->length > 0){
        circToTree(img->circles,root_node);
    }
    if(img->paths->length > 0){
        pathToTree(img->paths,root_node);
    }
    if(img->groups->length > 0){
        groupToTree(img->groups,root_node);
    }
    return doc;
}

void rectToTree(List *list,xmlNodePtr root_node){
    ListIterator iter = createIterator(list);
    void* elem;
    while((elem = nextElement(&iter)) != NULL){
        xmlNodePtr rectNode = NULL;
        rectNode = xmlNewChild(root_node,NULL,BAD_CAST "rect",NULL);

        Rectangle *rect = (Rectangle*)(elem);

        // rectNode = xmlNewChild(root_node, NULL, (const xmlChar*) "rect", NULL);
        
        char* units = malloc((strlen(rect->units)+1)*sizeof(char));
        strcpy(units,(char*)rect->units);
        char* val = malloc(524+((strlen(rect->units))*sizeof(char)));
        //parse the height from char to float to get rid of quotes from char
        sprintf(val,"%f%s",rect->x,units);

        xmlNewProp(rectNode, (const xmlChar*) "x", (const xmlChar*) val);

        strcpy(val,"");
        
        sprintf(val,"%f%s",rect->y,units);
        xmlNewProp(rectNode, (const xmlChar*) "y", (const xmlChar*) val);

        strcpy(val,"");

        sprintf(val,"%f%s",rect->width,units);
        xmlNewProp(rectNode, (const xmlChar*) "width", (const xmlChar*) val);

        strcpy(val,"");

        sprintf(val,"%f%s",rect->height,units);
        xmlNewProp(rectNode, (const xmlChar*) "height", (const xmlChar*) val);
        
        if(rect->otherAttributes->length > 0){
            attributeToTree(rect->otherAttributes, rectNode);
        }

        free(units);
        free(val);

    }
    return;
}
void circToTree(List *list,xmlNodePtr root_node){
    ListIterator iter = createIterator(list);
    void* elem;
    while((elem = nextElement(&iter)) != NULL){
        xmlNodePtr circNode = NULL;
        circNode = xmlNewChild(root_node,NULL,BAD_CAST "circle",NULL);

        Circle *circle = (Circle*)(elem);
        
        char* units = malloc((strlen(circle->units)+1)*sizeof(char));
        strcpy(units,(char*)circle->units);
        char* val = malloc(524+(strlen(circle->units)+1)*sizeof(char));
        //parse the height from char to float to get rid of quotes from char
        sprintf(val,"%f%s",circle->cx,units);
        xmlNewProp(circNode, (const xmlChar*) "cx", (const xmlChar*) val);

        strcpy(val,"");
        
        sprintf(val,"%f%s",circle->cy,units);
        xmlNewProp(circNode, (const xmlChar*) "cy", (const xmlChar*) val);

        strcpy(val,"");

        sprintf(val,"%f%s",circle->r,units);
        xmlNewProp(circNode, (const xmlChar*) "r", (const xmlChar*) val);
        
        if(circle->otherAttributes->length > 0){
            attributeToTree(circle->otherAttributes, circNode);
        }

        free(units);
        free(val);

    }
    return;
}
void pathToTree(List *list,xmlNodePtr root_node){
    ListIterator iter = createIterator(list);
    void* elem;
    while((elem = nextElement(&iter)) != NULL){
        xmlNodePtr pathNode = NULL;
        pathNode = xmlNewChild(root_node,NULL,BAD_CAST "path",NULL);

        Path *path = (Path*)(elem);

        char* val = malloc((strlen(path->data)+1)*sizeof(char));
        strcpy(val,(char*)path->data);
        xmlNewProp(pathNode, (const xmlChar*)"d", (const xmlChar*) val);
        
        if(path->otherAttributes->length > 0){
            attributeToTree(path->otherAttributes, pathNode);
        }

        free(val);

        //xmlAddChild(root_node,pathNode);

    }
    return;
}

void groupToTree(List *list,xmlNodePtr root_node){
    ListIterator iter = createIterator(list);
    void* elem;
    while((elem = nextElement(&iter)) != NULL){
        xmlNodePtr groupNode = NULL;
        groupNode = xmlNewChild(root_node,NULL,BAD_CAST "g",NULL);

        Group *group = (Group*)(elem);

        if(group->otherAttributes->length > 0){
            char *buff = malloc(256);
            ListIterator iter = createIterator(group->otherAttributes);
            void* elem;
            while((elem = nextElement(&iter)) != NULL){
                Attribute *attr = (Attribute*)(elem);
                // buff = realloc(buff,sizeof(char)*strlen((char*)attr->name)+1);
                // strcpy(buff,(char*)attr->name);
                xmlNewProp(groupNode, (const xmlChar*) attr->name, (const xmlChar*) attr->value);
            }
            free(buff);
        }
        if(group->rectangles->length > 0){
            rectToTree(group->rectangles,groupNode);
        }
        if(group->circles->length > 0){
            circToTree(group->circles,groupNode);
        }
        if(group->paths->length > 0){
            pathToTree(group->paths,groupNode);
        }
        if(group->groups->length > 0){
            groupToTree(group->groups,groupNode);
        }

        //xmlAddChild(root_node,groupNode);

    }
}

void attributeToTree(List *list, xmlNodePtr elemNode){
    ListIterator iter = createIterator(list);
    void* elem;
    while((elem = nextElement(&iter)) != NULL){
        Attribute *attr = (Attribute*)(elem);
        xmlNewProp(elemNode, (const xmlChar*) attr->name, (const xmlChar*) attr->value);
    }
}

bool updateAttribute(List *list,Attribute* attr){

    Node* node = list->head;
    while(node != NULL){
        if(strcmp(((Attribute*)node->data)->name, attr->name) == 0) {
            
            node->data = realloc(node->data, sizeof(Attribute) + (strlen(attr->value) + 1) * sizeof(char));
            strcpy(((Attribute*)node->data)->value, attr->value);
            return true;
        }
        node = node->next;
    }
    return false;
}

bool validateRect(List* rectangles){
    if(rectangles == NULL){
        return false;
    }
    ListIterator iter = createIterator(rectangles);
    void* elem;
    while((elem = nextElement(&iter)) != NULL){
        Rectangle* rect = (Rectangle*)elem;
        if(rect->x < 0){
            return false;
        }
        if(rect->y < 0){
            return false;
        }
        if(rect->width < 0){
            return false;
        }
        if(rect->height < 0){
            return false;
        }
        if(!validateAttr(rect->otherAttributes)){
            return false;
        }

    }
    return true;
}
bool validateCirc(List* circles){
    if(circles == NULL){
        return false;
    }
    ListIterator iter = createIterator(circles);
    void* elem;
    while((elem = nextElement(&iter)) != NULL){
        Circle* circle = (Circle*)elem;
        if(circle->cx < 0){
            return false;
        }
        if(circle->cy < 0){
            return false;
        }
        if(circle->r < 0){
            return false;
        }
        if(!validateAttr(circle->otherAttributes)){
            return false;
        }
    }
    return true;
}
bool validatePath(List* paths){
    if(paths == NULL){
        return false;
    }
    ListIterator iter = createIterator(paths);
    void* elem;
    while((elem = nextElement(&iter)) != NULL){
        Path* path = (Path*)elem;
        if(path->data == NULL){
            return false;
        }
        if(!validateAttr(path->otherAttributes)){
            return false;
        }
    }
    return true;
}
bool validateGroups(List* groups){
    if(groups == NULL){
        return false;
    }
    ListIterator iter = createIterator(groups);
    void* elem;
    while((elem = nextElement(&iter)) != NULL){
        Group* group = (Group*)elem;
        if(!validateAttr(group->otherAttributes)){
            return false;
        }
        if(!validateRect(group->rectangles)){
            return false;
        }
        if(!validateCirc(group->circles)){
            return false;
        }
        if(!validatePath(group->paths)){
            return false;
        }
        if(!validateGroups(group->groups)){
            return false;
        }
    }
    return true;
}

bool validateAttr(List *attributes){
    if(attributes == NULL){
        return false;
    }
    ListIterator iter = createIterator(attributes);
    void* elem;
    while((elem = nextElement(&iter)) != NULL){
        Attribute* attr = (Attribute*)elem;
        if(attr->name == NULL){
            return false;
        }
    }
    return true;
}

SVG* createEmptySVG(){
    
    SVG* svg = malloc(sizeof(SVG));

    strcpy(svg->namespace, "http://www.w3.org/2000/svg");

    strcpy(svg->description, "");
    strcpy(svg->title, "");

    svg->rectangles = initializeList(&rectangleToString, &deleteRectangle, &comparePaths);
    svg->circles = initializeList(&rectangleToString, &deleteCircle, &comparePaths);
    svg->paths = initializeList(&rectangleToString, &deletePath, &comparePaths);
    svg->groups = initializeList(&groupToString, &deleteGroup, &comparePaths);
    svg->otherAttributes = initializeList(&rectangleToString, &deleteAttribute, &comparePaths);

    return svg;
}
