// for testing

//gcc -g -I/usr/include/libxml2/ main.c SVGParser.c SVGHelper.c LinkedListAPI.c -lxml2 -lm -ggdb3 -o oFile

//gcc -I/usr/include/libxml2 -I/Users/gavin/Desktop/CIS\*2750/Assignment\ 2\ -\ due\ February\ 18\,\ 9pm-20220203/CIS2750W22_Project/include main.c SVGParser.c SVGHelper.c LinkedListAPI.c -lxml2 -ggdb3 -o output

//gcc -I/usr/include/libxml2 -I/home/undergrad/0/grotsaer/CIS2750/A1/src/include main.c SVGParser.c SVGHelper.c LinkedListAPI.c -lxml2 -ggdb3 -o output


//valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./oFile ../svgs/rects.svg svg.xsd

//valgrind ./oFile ../svgs/quad01.svgz

//export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.

//chmod 755 test1pre.

//./test1pre

//./output ../svgs/rect.svg ../svgs/svg.xsd

#include "SVGParser.h"
#include "SVGHelper.h"

int main(int argc, char **argv){

    LIBXML_TEST_VERSION

    // if( argc == 2 ) {
    //     printf("The argument supplied is %s\n", argv[1]);
    // }
    // else if( argc > 3 ) {
    //     printf("Too many arguments supplied.\n");
    // }
    // else {
    //     printf("One argument expected.\n");
    // }
    
    SVG* svg = createValidSVG(argv[1], argv[2]);

    //SVG *svg = createSVG(argv[1]);

    Attribute *newAttribute = (Attribute*)malloc(sizeof(Attribute) + 5000);
    newAttribute->name = malloc(sizeof(char)*100);
    strcpy(newAttribute->name,"d");
    strcpy(newAttribute->value,"m1188.2 152.5c-11 0.15-21.6 5.68-29.4 14.53-11 3.57-20.8 12.28-26.4 23.84-1 1.47-1.8 2.98-2.6 4.57-0.1 0.01-0.1 0.02-0.1 0.03-3.4-0.18-6.9 0.19-10.2 1.03-4.1-4.02-8.9-7.16-14-9.1-19.1-8.1-40.7 3.33-50.3 22.97-2.6 4.11-4.7 8.69-6.1 13.63-14.9 0.35-29.2 10.56-36.5 25.56-3.8 5.78-6.4 12.51-7.5 19.84-4 19.49 3.8 40.53 17.3 52.44-0.3 1.33-0.6 2.69-0.8 4.06-5.6 27.08 11.5 57.1 35.3 61.54 2.8 2.39 6 4.37 9.5 5.93 0.6 1.14 1.3 2.27 2.1 3.35 0.1 0.31 0.3 0.62 0.4 0.93 5.1 15.36 22 25.44 38 23.1 4.2-0.42 8.3-1.72 12.1-3.69 0.6 0.06 1.1 0.13 1.7 0.16 11.8 1.35 23-3.97 31.3-12.94 15.2 0.22 29-10.46 36.4-25.41 1.3-1.97 2.5-4.05 3.5-6.22 2.1 0.53 4.2 0.89 6.3 1 14.9 1.71 28.8-7.16 37.2-20.62 3.9-3.92 7.3-8.67 9.9-13.94 5-7.63 8.1-16.81 8.6-26.44 0.6 0.06 1.1 0.13 1.7 0.16 7.7 0.88 15.1-1.06 21.6-5 15.2 0.01 28.9-10.72 36.2-25.62 5.9-9.26 9.3-20.76 8.5-32.54l-0.1-0.87c-0.8-20.42-13.1-39.69-30-46.03-10.6-4.49-21.9-2.99-31.6 2.56-0.2 0.01-0.5 0.02-0.7 0.03-4.6-5.05-10.1-8.98-16.1-11.25-4.3-1.8-8.7-2.63-13-2.62-3.6-16.58-14.6-31.03-28.8-36.35-4.4-1.86-8.9-2.68-13.4-2.62z");

    if(svg != NULL){
        printf("prevlength: %d\n",svg->paths->length);
    }

    List* paths = getPaths(svg);
    char* pathsJSON = pathListToJSON(paths);
    printf("\n%s\n",pathsJSON);
    free(pathsJSON);
    freeList(paths);

    if(svg != NULL){
        char* attrJSON = attrListToJSON(svg->otherAttributes);
        printf("\n%s\n",attrJSON);
        free(attrJSON);
    }else{
        char* attrJSON = attrListToJSON(NULL);
        printf("\n%s\n",attrJSON);
        free(attrJSON);
    }

    if(setAttribute(svg, PATH, 2, newAttribute)){
        printf("setAttribute worked!\n");
        printf("newlength: %d\n",svg->paths->length);
    }else{
        printf("setAttribute did not work\n");
        deleteAttribute((void*)newAttribute);
    }

    List* paths2 = getPaths(svg);
    char* pathsJSON2 = pathListToJSON(paths2);
    printf("\n%s\n",pathsJSON2);
    free(pathsJSON2);
    freeList(paths2);
    
    // if(svg != NULL){
    //     char* attrJSON3 = attrListToJSON(svg->otherAttributes);
    //     printf("\n%s\n",attrJSON3);
    //     free(attrJSON3);
    //     printf("Adding viewbox\n");
    // }else{
    //     char* attrJSON3 = attrListToJSON(NULL);
    //     printf("\n%s\n",attrJSON3);
    //     free(attrJSON3);
    //     printf("Adding viewbox\n");
    // }

    // Attribute *newAttribute2 = (Attribute*)malloc(sizeof(Attribute) + 500);
    // newAttribute2->name = malloc(sizeof(char)*100);
    // strcpy(newAttribute2->name,"viewBox");
    // strcpy(newAttribute2->value,"0 0 64 64");

    // if(setAttribute(svg, SVG_IMG, 2, newAttribute2)){
    //     printf("setAttribute worked!\n");
    //     printf("newlength: %d\n",svg->otherAttributes->length);
    // }else{
    //     printf("setAttribute did not work\n");
    //     deleteAttribute((void*)newAttribute2);
    // }

    // if(svg != NULL){
    //     char* attrJSON2 = attrListToJSON(svg->otherAttributes);
    //     printf("\n%s\n",attrJSON2);
    //     free(attrJSON2);
    // }else{
    //     char* attrJSON2 = attrListToJSON(NULL);
    //     printf("\n%s\n",attrJSON2);
    //     free(attrJSON2);
    // }

    // Rectangle *rectangle = (Rectangle*)malloc(sizeof(Rectangle) + 500);
    // rectangle->x = 5;
    // rectangle->y = 8;
    // rectangle->width = 3;
    // rectangle->height = 4;
    // strcpy(rectangle->units,"");
    // rectangle->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
    // addComponent(svg, RECT, rectangle);
    // //deleteRectangle(rectangle);

    // Circle *circ = (Circle*)malloc(sizeof(Circle) + 500);
    // circ->cx = 5;
    // circ->cy = 8;
    // circ->r = 3;
    // strcpy(circ->units,"");
    // circ->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
    // addComponent(svg, CIRC, circ);
    // //deleteCircle(circ);

    // Path *path = (Path*)malloc(sizeof(Path) + 500);
    // strcpy(path->data,"M30,1h40l29,29v40l-29,29h-40l-29-29v-40z");
    // path->otherAttributes = initializeList(&attributeToString, &deleteAttribute, &compareAttributes);
    // addComponent(svg, PATH, path);
    // //deletePath(path);

    if(validateSVG(svg, argv[2])){
        printf("validateSVG worked!\n");
    }else{
        printf("validate SVG did not work\n");
    }

    if(writeSVG(svg, "outputSVG.svg")){
        printf("write worked!\n");
    }else{
        printf("write SVG did not work\n");
    }
    
    // List* rects = getRects(svg);
    // char* rectJSON = rectListToJSON(rects);
    // printf("\n%s\n",rectJSON);
    // free(rectJSON);
    
    // List* circles = getCircles(svg);
    // char* circleJSON = circListToJSON(circles);
    // printf("\n%s\n",circleJSON);
    // free(circleJSON);

    // List* groups = getGroups(svg);
    // char* groupsJSON = groupListToJSON(groups);
    // printf("\n%s\n",groupsJSON);
    // free(groupsJSON);

    // char* SVGJSON = SVGtoJSON(svg);
    // printf("\n%s\n",SVGJSON);
    // free(SVGJSON);
    
    // char* string = NULL;

    // char* rectsS = toString(rects);
    // char* circlesS = toString(circles);
    // char* pathsS = toString(paths);
    // char* groupsS = toString(groups);

    // printf("rects: %s, circles: %s, paths: %s, groups: %s\n",rectsS,circlesS,pathsS,groupsS);

    // free(rectsS);
    // free(circlesS);
    // free(pathsS);
    // free(groupsS);

    // freeList(rects);
    // freeList(circles);
    // freeList(paths);
    // freeList(groups);

    // string = SVGToString(svg);
    // printf("%s",string);
    // free(string);

    // int numrects = numRectsWithArea(svg,20);
    // int numcircs = numCirclesWithArea(svg,10);
    // int numpaths = numPathsWithdata(svg,"Stuff and junk.95");
    // int numgroups = numGroupsWithLen(svg,3);
    // int numAtt = numAttr(svg);
    // printf("numAttr: %d\n",numAtt);

    // printf("numRects: %d, numcircs: %d, numpaths: %d, numgroups: %d\n",numrects,numcircs,numpaths,numgroups);

    deleteSVG(svg);

    return 0;
}