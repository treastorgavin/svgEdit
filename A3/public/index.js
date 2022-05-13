// Put all onload AJAX calls here, and event listeners
jQuery(document).ready(function() {
    // class example use for reference

    // // On page-load AJAX Example
    // jQuery.ajax({
    //     type: 'get',            //Request type
    //     dataType: 'json',       //Data type - we will use JSON for almost everything 
    //     url: '/endpoint1',   //The server endpoint we are connecting to
    //     data: {
    //         data1: "Value 1",
    //         data2:1234.56
    //     },
    //     success: function (data) {
    //         /*  Do something with returned object
    //             Note that what we get is an object, not a string, 
    //             so we do not need to parse it on the server.
    //             JavaScript really does handle JSONs seamlessly
    //         */
    //         jQuery('#blah').html("On page load, received string '"+data.somethingElse+"' from server");
    //         //We write the object to the console to show that the request was successful
    //         console.log(data); 

    //     },
    //     fail: function(error) {
    //         // Non-200 return, do something with error
    //         $('#blah').html("On page load, received error from server");
    //         console.log(error); 
    //     }
    // });

    // // Event listener form example , we can use this instead explicitly listening for events
    // // No redirects if possible
    // $('#someform').submit(function(e){
    //     $('#blah').html("Form has data: "+$('#entryBox').val());
    //     e.preventDefault();
    //     //Pass data to the Ajax call, so it gets passed to the server
    //     $.ajax({
    //         //Create an object for connecting to another waypoint
    //     });
    // });

    //A3 functions
    // file log
    $.ajax({
        type: 'get',
        dataType: 'json', //set the endpoint to the SVG JSON getter function in app.js this will return an array (svg) of JSON strings for files in the upload folder
        url: '/getSVGJSONs',
        data: {
        },
        success: function (data){
            // do something with the json SVG remember this is an object not a string

            //add files to file log

            for(let i = 0; i < data.length; i++){
                let file = data[i].name;
                console.log(data);
                //make a table body
                $('#fileLogTable').append('<tbody>');
                //add a new row
                $('#fileLogTable').append('<tr>');
                //add header cells for titles and svg img
                $('#fileLogTable').append('<th><a href="'+file+'" download><img style="width:200px;height:auto;" src="uploads/'+file+'"></a></th>');
                $('#fileLogTable').append('<th><a href="'+file+'" download>'+file+'</a></th>');
                //adding subsections of the table
                // size, numRects, numCircs, numPaths, numGroups
                $('#fileLogTable').append('<th>'+data[i].size+'kb</th>');
                $('#fileLogTable').append('<th>'+data[i].numRect+'</th>');
                $('#fileLogTable').append('<th>'+data[i].numCirc+'</th>');
                $('#fileLogTable').append('<th>'+data[i].numPaths+'</th>');
                $('#fileLogTable').append('<th>'+data[i].numGroups+'</th>');
                //end row
                $('#fileLogTable').append('</tr>');
                //end body of table
                $('#fileLogTable').append('</tbody>');
                
                //add to drop down menu
                $('#svgDropDownMenu').append('<option id='+i+'>'+file+'</option>');
                $('#svgShapeDropDownMenu').append('<option id='+i+'>'+file+'</option>');
                $('#scaleShapeDropDownMenu').append('<option id='+i+'>'+file+'</option>');
            }

            if(data.length === 0){
                $('#fileLogTable').append('<p>No files</p>');
            }

        },
        fail: function(error){
            $('#blah').html("On page load, received error from server");
            console.log(error);
        }
    });

    $('#showAttributesButton').on('click',function(){
        let select = document.getElementById("svgDropDownMenu");
        let setval = select.options[select.selectedIndex].text;
        let chosen = setval;
        $.ajax({
            type: 'get',
            dataType: 'json',
            url: '/displaySVGwithAttributes',
            data: {
                filename: chosen,
            },
            success: function(data){
                console.log(chosen);
                console.log(data);

                $("#SVGViewerPanel").empty();
                
                $("#SVGViewerPanel").append('<tbody>');
                //make new table
                $("#SVGViewerPanel").append('<table class="table">');
                //display svg
                $("#SVGViewerPanel").append('<tr>');
                $("#SVGViewerPanel").append('<img style="width:800px;height:auto;" src="./uploads/'+chosen+'" href="'+chosen+'"/>');
                $("#SVGViewerPanel").append('</tr>');
                //inner table for title/desc
                $("#SVGViewerPanel").append("<thead>");
                //new row for title and desc
                $("#SVGViewerPanel").append('<tr>');
                $("#SVGViewerPanel").append('<th scope="col" style="text-align:center">Title</th>');
                $("#SVGViewerPanel").append('<th scope="col" style="text-align:center">Description</th>');
                $("#SVGViewerPanel").append('</tr>');
                $("#SVGViewerPanel").append('<tr>');
                $("#SVGViewerPanel").append('<td>'+data.title+'</td>');
                $("#SVGViewerPanel").append('<td>'+data.description+'</td>');
                $("#SVGViewerPanel").append('</tr>');

                $("#SVGViewerPanel").append('<tr>');
                $("#SVGViewerPanel").append('<th scope="col" style="text-align:center">Component</th>');
                $("#SVGViewerPanel").append('<th scope="col" style="text-align:center">Summary</th>');
                $("#SVGViewerPanel").append('<th scope="col" style="text-align:center">Other Attributes</th>');
                $("#SVGViewerPanel").append('</tr>');
                $("#SVGViewerPanel").append('</thead>');

                $("#SVGViewerPanel").append('<tbody>');
                for(let i = 0; i < data.rects.length; i++){    
                    $("#SVGViewerPanel").append('<tr scope="row">');
                    $("#SVGViewerPanel").append('<td style="text-align:left">Rectangle ' + i + '</td>');
                    $("#SVGViewerPanel").append('<td style="text-align:center">x = ' + data.rects[i].x + ' y = ' + data.rects[i].y + ' width = ' + data.rects[i].w + 'height = '+ data.rects[i].h +' units = ' + data.rects[i].units + '</td>');
                    for(let j = 0; j < data.rects[i].attributes.length; j++){
                        $("#SVGViewerPanel").append('<td id="attribute" style="text-align:right">'+'name ='+data.rects[i].attributes[j].name+ ' value = '+ data.rects[i].attributes[j].value+'</td>');
                    }
                    $("#SVGViewerPanel").append('</tr>');
                }
                

                //circles
                for(let i = 0; i < data.circles.length; i++){
                    $("#SVGViewerPanel").append('<tr scope="row">');
                    $("#SVGViewerPanel").append('<td style="text-align:left">Circle ' + i + '</td>');
                    $("#SVGViewerPanel").append('<td style="text-align:center">cx = ' + data.circles[i].cx + ' cy = ' + data.circles[i].cy + ' r = ' + data.circles[i].r +' units = ' + data.circles[i].units + '</td>');
                    for(let j = 0; j < data.circles[i].attributes.length; j++){
                        $("#SVGViewerPanel").append('<td id="attribute" style="text-align:right">'+'name ='+data.circles[i].attributes[j].name+ ' value = '+ data.circles[i].attributes[j].value+'</td>');
                    }
                    $("#SVGViewerPanel").append('</tr>');
                }
                //paths
                for(let i = 0; i < data.paths.length; i++){
                    $("#SVGViewerPanel").append('<tr scope="row">');
                    $("#SVGViewerPanel").append('<td style="text-align:left">Path ' + i + '</td>');
                    $("#SVGViewerPanel").append('<td style="text-align:center">data = ' + data.paths[i].d+'</td>');
                    for(let j = 0; j < data.paths[i].attributes.length; j++){
                        $("#SVGViewerPanel").append('<td id="attribute" style="text-align:right">'+'name ='+data.paths[i].attributes[j].name+ ' value = '+ data.paths[i].attributes[j].value+'</td>');
                    }
                    $("#SVGViewerPanel").append('</tr>');
                }
                //groups
                for(let i = 0; i < data.groups.length; i++){
                    $("#SVGViewerPanel").append('<tr>');
                    $("#SVGViewerPanel").append('<td style="text-align:left">Group ' + i + '</td>');
                    $("#SVGViewerPanel").append('<td style="text-align:center">children = ' + data.groups[i].children +'</td>');
                    for(let j = 0; j < data.groups[i].attributes.length; j++){
                        $("#SVGViewerPanel").append('<td id="attribute" style="text-align:right">'+'name ='+data.groups[i].attributes[j].name+ ' value = '+ data.groups[i].attributes[j].value+'</td>');
                    }
                    $("#SVGViewerPanel").append('</tr>');
                }
                $("#SVGViewerPanel").append('</tbody>');
           
            },
            fail: function(error){
                $('#blah').html("On page load, received error from server");
                console.log(error);
            }
        });

    });

    $('#editAttributesButton').on('click',function(){
        $('#editAttributes').toggle();
    });

    //change attributes
    $('#changeAttr').submit(function(e){

        e.preventDefault();
        let select = document.getElementById("svgDropDownMenu");
        let setval = select.options[select.selectedIndex].text;
        let chosen = setval;

        console.log("i made it here");

        $.ajax({
            type: "get",
            url: '/changeAttribute',
            dataType: 'json',
            data:{
                shapeType: $('#shapeType').val(),
                shapeNum: $('#shapeNum').val(),
                attributeName: $('#attrName').val(),
                attributeValue: $('#attrValue').val(),
                filename: chosen,
            },

            success: function(data){

                if(data.thing == 'changed'){
                    updateFileLog();
                    console.log("attribute changed");
                    location.reload();
                }else{
                    alert("failed to change attribute, check spelling and shape number.");
                }
            },
        });
    });

    //alter the file viewer based on the dropdown menu
    $('#svgDropDownMenu').on("change",(function(){
        let select = document.getElementById("svgDropDownMenu");
        let setval = select.options[select.selectedIndex].text;
        let chosen = setval;
        console.log('selected file = '+ chosen);
        $.ajax({
            type: 'get',
            dataType: 'json',
            url: '/displaySVG',
            data: {
                filename: chosen,
            },
            success: function(data){
                console.log(chosen);
                console.log(data);
                updateViewPanel(chosen,data);
            },
            error: function(error){
                $('#blah').html("On page load, received error from server");
                console.log(error);
            }
        });
    }));

    $('#changeTitleForm').submit(function(e){
        e.preventDefault();
        let select = document.getElementById("svgDropDownMenu");
        let setval = select.options[select.selectedIndex].text;
        let chosen = setval;
        $.ajax({
            type:'get',
            dataType:'json',
            url:'/changeTitle',
            data:{
                title: $('#titleTextBox').val(),
                filename: chosen,
            },
            success: function(data){

                if(data.thing == 'changed'){
                    console.log("changed title");
                    //update the file log view
                    updateFileLog();
                }else{
                    alert("Failed to change title");
                }

            }
        });
        //showSVG(chosen);

        //location.reload();
    });
    $('#changeDescForm').submit(function(e){
        e.preventDefault();
        let select = document.getElementById("svgDropDownMenu");
        let setval = select.options[select.selectedIndex].text;
        let chosen = setval;
        $.ajax({
            type:'get',
            dataType:'json',
            url:'/changeDescription',
            data:{
                description: $('#descTextBox').val(),
                filename: chosen,
            },
            success: function(data){

                if(data.thing == 'changed'){
                    console.log("changed description");
                    //update the file log view
                    updateFileLog();
                    
                }else{
                    alert("Failed to change description");
                }
            }
        });
        //showSVG(chosen);
    });

    //event listener for valid uploads
    $('#uploadForm').submit(function(e){

        e.preventDefault();

        let form = document.getElementById('uploadForm');
        let formData = new FormData(form);

        $.ajax({
            type: "POST",
            url: '/validFileUpload',
            data: formData,
            contentType: false,
            processData: false,
            cache: false,

            success: function(msg){
                console.log("uploaded");
                updateFileLog();
                location.reload();
            },
            error: function(){
                alert("file cannot upload");
            }
        });
    });

    //download a file by creating a new SVG

    $('#createForm').submit(function(e){
        e.preventDefault();
        $.ajax({
            type: 'get',
            dataType: 'json',
            url: '/createNewSVG',
            data:{
                filename: $('#fileName').val(),
                description: $('#description').val(),
                title: $('#title').val(),
            },
            success: function(data){
                if(data.thing == 'downloaded'){
                    console.log("created new SVG");
                    
                    updateFileLog();
                    location.reload();
                }else{
                    alert("Failed to create SVG");
                }
            },
        });

    });

    //ADD SHAPES
    
    //get the rectangle values from the inputs upon submit button
    $('#addRectangle').submit(function(e){
        e.preventDefault();
        
        let select = document.getElementById("svgShapeDropDownMenu");
        let setval = select.options[select.selectedIndex].text;
        let chosen = setval;
        $.ajax({
            type:'get',
            dataType:'json',
            url:'/addRectangle',
            data:{
                x: $('#xBox').val(),
                y: $('#yBox').val(),
                width: $('#widthBox').val(),
                height: $('#heightBox').val(),
                units: $('#runitsBox').val(),
                fill: $('#rectangleFillBox').val(),
                filename: chosen,
            },
            success: function(data){

                if(data.thing == 'added'){
                    console.log("added Rectangle");
                    //update the file log view
                    updateFileLog();
                    location.reload();
                }else{
                    alert("Failed to add rectangle");
                }

            },

        });

        
    });

    //add cicle
    $('#addCircle').submit(function(e){
        e.preventDefault();
        let select = document.getElementById("svgShapeDropDownMenu");
        let setval = select.options[select.selectedIndex].text;
        let chosen = setval;
        $.ajax({
            type:'get',
            dataType:'json',
            url:'/addCircle',
            data:{
                cx: $('#cxBox').val(),
                cy: $('#cyBox').val(),
                r: $('#rBox').val(),
                units: $('#cunitsBox').val(),
                fill: $('#circleFillBox').val(),
                filename: chosen,
            },
            success: function(data){
                if(data.thing == 'added'){
                    //update the file log view
                    
                    console.log("added Circle");
                    updateFileLog();
                    location.reload();
                }else{
                    alert("Failed to add Circle");
                }
            },
        });

    });

    //SCALE SHAPES
    $('#scale').submit(function(e){
        e.preventDefault();
        //console.log("debug");
        let select = document.getElementById("scaleShapeDropDownMenu");
        let setval = select.options[select.selectedIndex].text;
        let chosen = setval;

        let shapeType = document.getElementById("shapeDropDown");
        let shapeVal = shapeType.options[shapeType.selectedIndex].text;
        $.ajax({
            type:'get',
            dataType:'json',
            url:'/scaleShape',
            data:{ // get the shapeType, and scale factor and filename
                shape: shapeVal,
                scaleFactor: $('#scaleBox').val(),
                filename: chosen,
            },
            success: function(data){
                
                if(data.thing == 'scaled'){
                    //update the file log view
                    console.log("scaled shapes");
                    updateFileLog();
                    location.reload();
                    
                }else{
                    alert("Failed to scale shape.");
                }

            }
        });
        
    });

    //update file log view
    function updateFileLog(){
        $.ajax({
            type: 'get',
            dataType: 'json', //set the endpoint to the SVG JSON getter function in app.js this will return an array (svg) of JSON strings for files in the upload folder
            url: '/getSVGJSONs',
            data: {
            },
            success: function (data){
                $("#fileLogTable").empty();
                for(let i = 0; i < data.length; i++){
                    let file = data[i].name;
                    console.log(data);
                    //make a table body
                    $('#fileLogTable').append('<tbody>');
                    //add a new row
                    $('#fileLogTable').append('<tr>');
                    //add header cells for titles and svg img
                    $('#fileLogTable').append('<th><a href="'+file+'" download><img style="width:200px;height:auto;" src="uploads/'+file+'"></a></th>');
                    $('#fileLogTable').append('<th><a href="'+file+'" download>'+file+'</a></th>');
                    //adding subsections of the table
                    // size, numRects, numCircs, numPaths, numGroups
                    $('#fileLogTable').append('<th>'+data[i].size+'kb</th>');
                    $('#fileLogTable').append('<th>'+data[i].numRect+'</th>');
                    $('#fileLogTable').append('<th>'+data[i].numCirc+'</th>');
                    $('#fileLogTable').append('<th>'+data[i].numPaths+'</th>');
                    $('#fileLogTable').append('<th>'+data[i].numGroups+'</th>');
                    //end row
                    $('#fileLogTable').append('</tr>');
                    //end body of table
                    $('#fileLogTable').append('</tbody>');

                    $('#svgDropDownMenu').append('<option id='+i+'>'+file+'</option>');
                    $('#svgShapeDropDownMenu').append('<option id='+i+'>'+file+'</option>');
                    $('#scaleShapeDropDownMenu').append('<option id='+i+'>'+file+'</option>');
                }
            }
        });
        return;
    }

    function moveFileToUploads(fileName){

        e.preventDefault();

        let form = "CIS2750W22_Project/"+fileName+".svg";
        let formData = new FormData(form);

        $.ajax({
            type: "POST",
            url: '/validFileUpload',
            data: formData,
            contentType: false,
            processData: false,
            cache: false,

            success: function(msg){
                console.log("uploaded");
                updateFileLog();
                location.reload();
            },
            error: function(){
                alert("file cannot upload");
            }
        });
    }

    function updateViewPanel(fileName,data){
        $("#SVGViewerPanel").empty();
        $("#SVGViewerPanel").append('<tbody>');
        //make new table
        $("#SVGViewerPanel").append('<table>');
        //display svg
        $("#SVGViewerPanel").append('<tr>');
        $("#SVGViewerPanel").append('<img style="width:800px;height:auto;" src="./uploads/'+fileName+'" href="'+fileName+'"/>');
        $("#SVGViewerPanel").append('</tr>');
        //inner table for title/desc
        $("#SVGViewerPanel").append("</table>");
        //new row for title and desc
        $("#SVGViewerPanel").append('<tr>');
        $("#SVGViewerPanel").append('<th style="text-align:center">Title</th>');
        $("#SVGViewerPanel").append('<th style="text-align:center">Description</th>');
        $("#SVGViewerPanel").append('</tr>');
        $("#SVGViewerPanel").append('<tr>');
        $("#SVGViewerPanel").append('<td>'+data.title+'</td>');
        $("#SVGViewerPanel").append('<td>'+data.description+'</td>');
        $("#SVGViewerPanel").append('</tr>');
        $("#SVGViewerPanel").append('</table>');

        $("#SVGViewerPanel").append('<table>');
        $("#SVGViewerPanel").append('<tr>');
        $("#SVGViewerPanel").append('<th style="text-align:center">Component</th>');
        $("#SVGViewerPanel").append('<th style="text-align:center">Summary</th>');
        $("#SVGViewerPanel").append('<th style="text-align:center">Other Attributes</th>');
        $("#SVGViewerPanel").append('</tr>');
        $("#SVGViewerPanel").append('</table>');

        //fill the table with components
        //rectangels
        for(let i = 0; i < data.rects.length; i++){
            $("#SVGViewerPanel").append('<tr>');
            $("#SVGViewerPanel").append('<td style="text-align:left">Rectangle ' + i + '</td>');
            $("#SVGViewerPanel").append('<td style="text-align:center">x = ' + data.rects[i].x + ' y = ' + data.rects[i].y + ' width = ' + data.rects[i].w + 'height = '+ data.rects[i].h +' units = ' + data.rects[i].units + '</td>');
            $("#SVGViewerPanel").append('<td id="attribute" style="text-align:right">'+data.rects[i].numAttr+'</td>');
            $("#SVGViewerPanel").append('</tr>');
        }

        //circles
        for(let i = 0; i < data.circles.length; i++){
            $("#SVGViewerPanel").append('<tr>');
            $("#SVGViewerPanel").append('<td style="text-align:left">Circle ' + i + '</td>');
            $("#SVGViewerPanel").append('<td style="text-align:center">cx = ' + data.circles[i].cx + ' cy = ' + data.circles[i].cy + ' r = ' + data.circles[i].r + ' units = ' + data.circles[i].units + '</td>');
            $("#SVGViewerPanel").append('<td id="attribute" style="text-align:right">'+data.circles[i].numAttr+'</td>');
            $("#SVGViewerPanel").append('</tr>');
        }
        //paths
        for(let i = 0; i < data.paths.length; i++){
            $("#SVGViewerPanel").append('<tr>');
            $("#SVGViewerPanel").append('<td style="text-align:left">Path ' + i + '</td>');
            $("#SVGViewerPanel").append('<td style="text-align:center">data = ' + data.paths[i].d + '</td>');
            $("#SVGViewerPanel").append('<td id="attribute" style="text-align:right">'+data.paths[i].numAttr+'</td>');
            $("#SVGViewerPanel").append('</tr>');
        }
        //groups
        for(let i = 0; i < data.groups.length; i++){
            $("#SVGViewerPanel").append('<tr>');
            $("#SVGViewerPanel").append('<td style="text-align:left">Group ' + i + '</td>');
            $("#SVGViewerPanel").append('<td style="text-align:center">children = ' + data.groups[i].children + '</td>');
            $("#SVGViewerPanel").append('<td id="attribute" style="text-align:right">'+data.groups[i].numAttr+'</td>');
            $("#SVGViewerPanel").append('</tr>');
        }
        $("#SVGViewerPanel").append('</tbody>');
    }

    function showSVG(chosen){
        $.ajax({
            type: 'get',
            dataType: 'json',
            url: '/displaySVG',
            data: {
                filename: chosen,
            },
            success: function(data){
                updateViewPanel(chosen,data);
            },
            error: function(error){
                $('#blah').html("On page load, received error from server");
                console.log(error);
            }
        });
    }

    // function updateViewPanelWithAttributes(fileName,data){

    // }
});