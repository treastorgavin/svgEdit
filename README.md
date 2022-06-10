# svgEdit

## A1
The first assignment showcases the start of the svg parsing library using libxml.

## A2
The second assignment adds further implementation of the parsing library with more functions and corrections on the first assignment (A1).

## A3
The third assignment connects the backend **C** library for parsing **svg** files to the front end web GUI.
The GUI was made using JavaScript and HTML with CSS.
The GUI is simple and was created strictly for demonstrating the functionality of the parsing library.

### TESTING CIS2750

## Fix A1
   - getRects with hen_and_chicks
    - getCircles with vest
    - getPaths with vest, beer, hen
    - getGroups with beer, hen, satisfaction
    - numPaths
        - Emoji_smiliing (m44.6 40.3c-8.1 5.7-17.1 5.6-25.2 0-1-.7-1.8.5-1.2 1.6 2.5 4 7.4 7.7 13.8 7.7s11.3-3.6 13.8-7.7c.6-1.1-.2-2.3-1.2-1.6)
        - Vest crashed
        - Beer crashed
        - Hen crashed
        - Satisfaction crashed
    - numGroups
        - Vest, beer, hen, satisfaction
    - numAttr
        - Vest seg fault
        - Beer crashed
        - Satisfaction crashed
## A2 Testing
- createValid SVG with valid svg
    - Valid with invalid SVGParser settings
        - Negative numbers for x, y, width, height, cx, cy, r
        - Invalid path data
        - Invalid namespace
        - Invalid attributes
- createValid SVG with invalid xml format
    - Invalid SVG format

- Write SVG
    - Check for NULL arguments

- Validate svg
    - Contents of SVG must be valid when converted to XML
    - Check for invalid SVGParser
        - Negative numbers for x, y, width, height, cx, cy, r
        - Invalid path data
        - Invalid namespace
        - Invalid attributes
    - Invalid numbers
    - Check NULL pointers to lists

- Test setAttribute
    - Valid and invalid inputs
        - NULL for everything
        - elemIndex out of bounds for each elemtype
        - Invalid elemtype
    - Rectangles
        - X,y,width,height, otherAttributes
    - Circles
        - Cx, cy, r, otheratter
    - Paths
        - Data, and other attributes
    - Groups
        - Other attributes
    - Incorrect element type
- Check to make sure attribute has been added or changed for each elemtype and at various elemIndexs
- Check for memory leaks when updating existing attributes
- Check for memory leaks when adding a new attribute

- Add components
    - Valid and invalid inputs
    - NULL inputs
    - Incorrect element type
    - Check that the element has been added

A3 testing was done in JS and HTML documentation for visual debugging
