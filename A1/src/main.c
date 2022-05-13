// for testing

//gcc -g -I/usr/include/libxml2/ main.c SVGParser.c SVGHelper.c LinkedListAPI.c -lxml2 -lm -ggdb3 -o oFile

//gcc -I/usr/include/libxml2 -I/Users/gavin/Desktop/CIS\*2750/A1/include main.c SVGParser.c SVGHelper.c LinkedListAPI.c -lxml2 -ggdb3 -o output

//gcc -I/usr/include/libxml2 -I/home/undergrad/0/grotsaer/CIS2750/A1/src/include main.c SVGParser.c SVGHelper.c LinkedListAPI.c -lxml2 -ggdb3 -o output


//valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes ./oFile ../svgs/quad01.svg

//valgrind ./oFile ../svgs/quad01.svg

//export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:.

//chmod 755 test1pre.

//./test1pre

#include "SVGParser.h"
#include "SVGHelper.h"

int main(int argc, char **argv){

    LIBXML_TEST_VERSION

    if( argc == 2 ) {
        printf("The argument supplied is %s\n", argv[1]);
    }
    else if( argc > 2 ) {
        printf("Too many arguments supplied.\n");
    }
    else {
        printf("One argument expected.\n");
    }


    SVG* svg = createSVG(argv[1]);
    List* rects = getRects(svg);
    List* circles = getCircles(svg);
    printf("circles length= %d\n",circles->length);
    List* paths = getPaths(svg);
    List* groups = getGroups(svg);
    
    char* string = NULL;

    char* rectsS = toString(rects);
    char* circlesS = toString(circles);
    char* pathsS = toString(paths);
    char* groupsS = toString(groups);

    printf("rects: %s, circles: %s, paths: %s, groups: %s\n",rectsS,circlesS,pathsS,groupsS);

    free(rectsS);
    free(circlesS);
    free(pathsS);
    free(groupsS);

    freeList(rects);
    freeList(circles);
    freeList(paths);
    freeList(groups);

    string = SVGToString(svg);
    printf("%s",string);
    free(string);

    int numrects = numRectsWithArea(svg,20);
    int numcircs = numCirclesWithArea(svg,10);
    int numpaths = numPathsWithdata(svg,"m44.6 42.3c-8.1 5.7-17.1 5.6-25.2 0-1-.7-1.8.5-1.2 1.6 2.5 4 7.4 7.7 13.8 7.7s11.3-3.6 13.8-7.7c.6-1.1-.2-2.3-1.2-1.6");
    int numgroups = numGroupsWithLen(svg, 2);
    int numAtt = numAttr(svg);
    printf("numAttr: %d\n",numAtt);

    printf("numRects: %d, numcircs: %d, numpaths: %d, numgroups: %d\n",numrects,numcircs,numpaths,numgroups);

    deleteSVG(svg);

    return 0;
}