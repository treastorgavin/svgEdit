'use strict'

// C library API
const ffi = require('ffi-napi');

// Express App (Routes)
const express = require("express");
const app     = express();
const path    = require("path");
const fileUpload = require('express-fileupload');

app.use(fileUpload());
app.use(express.static(path.join(__dirname+'/uploads')));

// Minimization
const fs = require('fs');
const JavaScriptObfuscator = require('javascript-obfuscator');
const res = require('express/lib/response');

// Important, pass in port as in `npm run dev 1234`, do not change
const portNum = process.argv[2];

// Send HTML at root, do not change
app.get('/',function(req,res){
  res.sendFile(path.join(__dirname+'/public/index.html'));
});

// Send Style, do not change
app.get('/style.css',function(req,res){
  //Feel free to change the contents of style.css to prettify your Web app
  res.sendFile(path.join(__dirname+'/public/style.css'));
});

// Send obfuscated JS, do not change
app.get('/index.js',function(req,res){
  fs.readFile(path.join(__dirname+'/public/index.js'), 'utf8', function(err, contents) {
    const minimizedContents = JavaScriptObfuscator.obfuscate(contents, {compact: true, controlFlowFlattening: true});
    res.contentType('application/javascript');
    res.send(minimizedContents._obfuscatedCode);
  });
});

// Respond to POST requests that upload files to uploads/ directory
// app.post('/upload', function(req, res) {
//   if(!req.files) {
//     return res.status(400).send('No files were uploaded.');
//   }
 
//   let uploadFile = req.files.uploadFile;
 
//   // Use the mv() method to place the file somewhere on your server
//   uploadFile.mv('uploads/' + uploadFile.name, function(err) {
//     if(err) {
//       return res.status(500).send(err);
//     }

//     res.redirect('/');
//   });
// });

//Respond to GET requests for files in the uploads/ directory
app.get('/uploads/:name', function(req , res){
  fs.stat('uploads/' + req.params.name, function(err, stat) {
    if(err == null) {
      res.sendFile(path.join(__dirname+'/uploads/' + req.params.name));
    } else {
      console.log('Error in file downloading route: '+err);
      res.send('');
    }
  });
});

//******************** Your code goes here ******************** 

//functions from library

let libsvgparser = ffi.Library("./parser/libsvgparser",{
  //funciton names
  'createValidSVGtoJSON': ['string',['string','string']],
  'rectangleToJSONFromFile': ['string',['string','string']],
  'circleToJSONFromFile': ['string',['string','string']],
  'pathToJSONFromFile': ['string',['string','string']],
  'groupToJSONFromFile': ['string',['string','string']],
  'getTitle': ['string',['string','string']],
  'getDesc': ['string',['string','string']],
  'validateFile': ['int',['string','string']],
  'createNewSVG': ['int',['string','string','string','string']],
  'addRectangleToSVGFromServer': ['int',['string','string','float','float','float','float','string','string']],
  'addCircleToSVGFromServer': ['int',['string','string','float','float','float','string','string']],
  'scaleShapeFromServer': ['int',['string','string','string','float']],
  'attributesToJSONFromFile':['string',['string','string','string','int']],
  'changeOrAddAttribute':['int',['string','string','string', 'int', 'string','string']],
  'changeTitle': ['int',['string','string','string']],
  'changeDescription': ['int',['string','string','string']],
  
});

//altered upload file to consider if the file was valid or not
app.post('/validFileUpload', function(req, res){
  //check if a valid file was uploaded
  if(!req.files) {
    return res.status(400).send('No files were uploaded.');
  }
 
  let uploadFile = req.files.uploadFile;
 
  // Use the mv() method to place the file somewhere on your server
  uploadFile.mv('uploads/'+uploadFile.name+'', function(err) {
    if(err) {
      return res.status(500).send(err);
    }

    if(libsvgparser.validateFile('uploads/'+uploadFile.name+'','parser/src/svg.xsd') == 0){
      //remove invalid file
      fs.unlinkSync('uploads/'+uploadFile.name+'');
      console.log('deleted' + uploadFile.name);
    }

    res.redirect('/');
  });

});

//create SVG
app.get('/createNewSVG', function(req, res){
  let check = libsvgparser.createNewSVG(req.query.filename,'parser/src/svg.xsd',req.query.description,req.query.title);

  console.log("created = "+check);

  let file;
  let dest;
  if(req.query.filename.endsWith(".svg")){
    file = path.basename(req.query.filename);
    dest = path.resolve("./uploads/",file);
  }else{
    file = path.basename(req.query.filename+".svg");
    dest = path.resolve("./uploads/",file);
  }
  
  
  fs.rename(file,dest, (err)=>{
    if(err) throw err;
    else console.log('moved to uploads');
  });

  if(check){
    res.send({
      thing: 'downloaded'
    });
  }else{
    res.send({
      thing: 'failed'
    });
  }
  
});

//getter for SVG to JSON

app.get('/getSVGJSONs', function(req, res){
  // create an SVG array
  let svg = [];
  let fileName = fs.readdirSync('uploads');

  //validate files for .svg extension
  const validFiles = fileName.filter( file => {
    return file.endsWith(".svg");
  });
  
  // go through each file uploaded
  for(let i = 0; i<validFiles.length; i++){
  
    //creating JSON from file and pushing to the svg array
    svg.push(JSON.parse(libsvgparser.createValidSVGtoJSON('uploads/'+validFiles[i]+'','parser/src/svg.xsd')));
    svg[i].name = validFiles[i];
    //get the size of file in kb
    let size = Math.round((fs.statSync('uploads/'+validFiles[i])["size"])/1000);

    if(size > 0){
      svg[i].size = size;
    }else{
      svg[i].size = 1;
    }
    
  }
  //send the svg array
  res.send(svg);

});

app.get('/changeTitle', function(req, res){
  
  let change = libsvgparser.changeTitle('uploads/'+req.query.filename+'','parser/src/svg.xsd',req.query.title);

  if(change){
    res.send({
      thing: 'changed'
    });
  }else{
    res.send({
      thing: 'failed'
    });
  }
});

app.get('/changeDescription', function(req, res){
  
  let change = libsvgparser.changeDescription('uploads/'+req.query.filename+'','parser/src/svg.xsd',req.query.description);

  if(change){
    res.send({
      thing: 'changed'
    });
  }else{
    res.send({
      thing: 'failed'
    });
  }
});

//display SVG
app.get('/displaySVG', function(req, res){
  // create svg object
  let svg = {};
  svg.description = libsvgparser.getDesc('uploads/'+req.query.filename+'','parser/src/svg.xsd');
  svg.title = libsvgparser.getTitle('uploads/'+req.query.filename+'','parser/src/svg.xsd');
  svg.rects = JSON.parse(libsvgparser.rectangleToJSONFromFile('uploads/'+req.query.filename+'','parser/src/svg.xsd'));
  svg.circles = JSON.parse(libsvgparser.circleToJSONFromFile('uploads/'+req.query.filename+'','parser/src/svg.xsd'));
  svg.paths = JSON.parse(libsvgparser.pathToJSONFromFile('uploads/'+req.query.filename+'','parser/src/svg.xsd'));
  svg.groups = JSON.parse(libsvgparser.groupToJSONFromFile('uploads/'+req.query.filename+'','parser/src/svg.xsd'));

  res.send(svg);

});

//display with attributes
app.get('/displaySVGwithAttributes',function(req, res){
  let svg = {};

  let numElements = JSON.parse(libsvgparser.createValidSVGtoJSON('uploads/'+req.query.filename+'','parser/src/svg.xsd'));

  svg.description = libsvgparser.getDesc('uploads/'+req.query.filename+'','parser/src/svg.xsd');
  svg.title = libsvgparser.getTitle('uploads/'+req.query.filename+'','parser/src/svg.xsd');
  
  //getting attributes for all rectangles
  svg.rects = JSON.parse(libsvgparser.rectangleToJSONFromFile('uploads/'+req.query.filename+'','parser/src/svg.xsd'));
  for(let i = 0; i<numElements.numRect; i++){
    svg.rects[i].attributes = JSON.parse(libsvgparser.attributesToJSONFromFile('uploads/'+req.query.filename+'','parser/src/svg.xsd','RECT',i));
  }
  

  //getting circle attribtes
  svg.circles = JSON.parse(libsvgparser.circleToJSONFromFile('uploads/'+req.query.filename+'','parser/src/svg.xsd'));
  
  for(let i = 0; i<numElements.numCirc; i++){
    svg.circles[i].attributes = JSON.parse(libsvgparser.attributesToJSONFromFile('uploads/'+req.query.filename+'','parser/src/svg.xsd','CIRC',i));
  }

  //getting path attributes
  svg.paths = JSON.parse(libsvgparser.pathToJSONFromFile('uploads/'+req.query.filename+'','parser/src/svg.xsd'));
  
  for(let i = 0; i<numElements.numPaths; i++){
    svg.paths[i].attributes = JSON.parse(libsvgparser.attributesToJSONFromFile('uploads/'+req.query.filename+'','parser/src/svg.xsd','PATH',i));
  }
  

  //getting group attributes
  svg.groups = JSON.parse(libsvgparser.groupToJSONFromFile('uploads/'+req.query.filename+'','parser/src/svg.xsd'));
  for(let i = 0; i<numElements.numGroups; i++){
    svg.groups[i].attributes = JSON.parse(libsvgparser.attributesToJSONFromFile('uploads/'+req.query.filename+'','parser/src/svg.xsd','GROUP',i));
  }

  res.send(svg);
})

app.get('/changeAttribute', function(req, res){
  let change = libsvgparser.changeOrAddAttribute('uploads/'+req.query.filename,'parser/src/svg.xsd',req.query.shapeType,req.query.shapeNum,req.query.attributeName,req.query.attributeValue);

  console.log("changed: " + change);

  if(change == 1){
    res.send({
      thing: 'changed'});
  }else{
    res.send({
      thing: 'not'});
  }

});
//download SVG
// app.get('/downloadSVG', function(req, res){

//   // get the name of SVG, title and desc
//   let check = libsvgparser.downloadSVG('uploads/'+req.query.filename,'parser/src/svg.xsd',req.query.title,req.query.description);

//   if(check === 1){
//     res.send('downloaded');
//   }else{
//     res.status(400).send('File cannot be downloaded.');
//   }
  
// });

//ADD SHAPES
app.get('/addRectangle', function(req, res){
  //write to the svg file
  // request the values for the rectangle
  // filename is from the drop down menu
  //console.log("debug");
  let check = libsvgparser.addRectangleToSVGFromServer('uploads/'+req.query.filename,'parser/src/svg.xsd',req.query.x,req.query.y,req.query.width,req.query.height,req.query.units,req.query.fill);
  //console.log("debug check: "+ check);
  if(check){
    res.send({
      thing: 'added'
    });
  }else{
    res.send({
      thing: 'failed'
    });
  }
  
});

app.get('/addCircle', function(req, res){
  //write to the svg file
  // request the values for the rectangle
  // filename is from the drop down menu
  console.log(req.query.units);
  let check = libsvgparser.addCircleToSVGFromServer('uploads/'+req.query.filename,'parser/src/svg.xsd',req.query.cx,req.query.cy,req.query.r,req.query.units,req.query.fill);

  if(check){
    res.send({
      thing: 'added'
    });
  }else{
    res.send({
      thing: 'failed'
    });
  }
  
});

//SCALE SHAPES
app.get('/scaleShape',function(req, res){
  // takes the file, schema, shapeType, and scale factor
  let check = libsvgparser.scaleShapeFromServer('uploads/'+req.query.filename,'parser/src/svg.xsd',req.query.shape,req.query.scaleFactor);
  //console.log("check: "+check);

  if(check ==1){
    res.send({
      thing: 'scaled'
    });
  }else{
    res.send({
      thing: 'failed'
    });
  }
});

app.listen(portNum);
console.log('Running app at localhost: ' + portNum);
