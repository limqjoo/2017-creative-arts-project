/**
  * npm packages
  */
const path = require('path');
const fs = require('fs');
const express = require('express');
const dgram = require('dgram').createSocket('udp4');
var app = express();
var server = require('http').createServer(app);
var io = require('socket.io')(server);
const rfb = require('rfb2');
var PNG = require('pngjs').PNG;
var gm = require('gm');

var clients = []; //stores clients' info
var fileNum = 0; //for naming output image files
var touchPos; //for storing position of touches before they are sent to oF app
var scale = 0.5; //scales frame from vnc server
var latestFrame; //for storing latest frame from vnc server before updating to webapp
var udpConnected = false;
var sendFrameTimer; //for interval timer

/**********************************************************************
  * WEB SERVER
  */
server.listen(8090);
console.log(`Socket.io listening on 8090`)

app.use(express.static(path.join(__dirname, 'node_modules')));
app.use(express.static(path.join(__dirname, 'public')));

app.get('/file/:name', function (req, res, next) {

  var options = {
    root: __dirname + '/public/',
    dotfiles: 'deny',
    headers: {
        'x-timestamp': Date.now(),
        'x-sent': true
    }
  };

  var fileName = req.params.name;
  res.sendFile(fileName, options, function (err) {
    if (err) {
      next(err);
    } else {
      console.log('Sent:', fileName);
    }
  });
});

/**********************************************************************
 * SOCKET.IO
 * - connects to webapp and communicates the data via web socket
 */
io.on('connection', function (socket) { //initial connection established

  socket.on('init', function (config) { //when the client is connected (i.e. the connection is established) it sends “init” message
    //config contains the data (host, port and password) required for the proxy server to connect to the VNC server

    var r = createRfbConnection(config, socket); //connect to vnc server via rfb, will return error if failed

    if (typeof r === 'string'){ //error message
      socket.emit('error', {error: r});
      return;
    };

    /*socket.on('mouse', function (event) { //mouse event received when only when cursor is dragged
      //r.pointerEvent(evnt.x, evnt.y, evnt.button);
      //instead of updating mouse position to computer, store cursor position
      pos.push({
        x: event.x,
        y: event.y
      });
    });*/

    socket.on('touch', function(event){
      //relay data to oF app
      touchPos = event.data;
      sendTouchPos();
    });

    socket.on('disconnect', function () { //client disconnects
      disconnectClient(socket);
      return;
    });
  });
});

/**
 * Disconnects from VNC server
 * - do some clean up
 * @param {*} socket 
 */
function disconnectClient(socket) {
  clients.forEach(function (client) {
    if (client.socket === socket) {
      client.rfb.end();
      clearInterval(client.interval);
    }
  });
  clients = clients.filter(function (client) {
    //remove interval
    clearInterval(sendFrameTimer);
    console.log('Client disconnected.');
    return client.socket === socket;
  });
}

/**
 * Creates a rfb connection to VNC server
 * @param {*} config 
 * @param {*} socket 
 */
function createRfbConnection(config, socket) {
  var r;
  try {
    r = rfb.createConnection({
      host: config.host,
      port: config.port,
      password: config.password
    });
  } catch (e) {
    console.log(e);
    return e.toString();
  }
  addEventHandlers(r, socket);
  return r;
}

/**
 * Handles updates from VNC server
 * @param {*} r 
 * @param {*} socket 
 */
function addEventHandlers(r, socket) {
  var initialized = false;
  var screenWidth;
  var screenHeight;
  sendFrameTimer = setInterval(sendFrame, 500); //set timer to send frame to client every half a second
  
  function handleConnection(width, height) {
    screenWidth = width;
    screenHeight = height;
    console.log('RFB connection established.');
    socket.emit('init', { //send width and height of screen to client
      width: width,
      height: height
    });
    clients.push({ //add client to list of clients
      socket: socket,
      rfb: r,
      interval: setInterval(function () {
        r.requestUpdate(false, 0, 0, r.width, r.height);
      }, 50)
    });
    initialized = true;
  }

  r.on('error', function (e) {
    console.error('Error while talking with the remote RFB server', e);
    socket.emit('error',{
      error: e
    });
  });

  r.on('rect', function(rect) {
    if (!initialized) {
      handleConnection(rect.width*scale, rect.height*scale);
    }
    if (rect.encoding === rfb.encodings.raw){ // available data: rect.x, rect.y, rect.width, rect.height, rect.data
        // pixmap format is in r.bpp, r.depth, r.redMask, greenMask, blueMask, redShift, greenShift, blueShift
        // rect.data format: <buffer b g r a b g r a ...>
        
        //newImage(rect);
        latestFrame = rect; //save the latest frame data, to be sent to clients every half a second 
    };
  });

  r.on('*', function () {
    console.error(arguments);
  });

  /**
  * Sends frame to webapp
  */
  function sendFrame(){
    if (clients.length > 0){
      socket.emit('frame', {
        x: latestFrame.x,
        y: latestFrame.y,
        width: latestFrame.width*scale,
        height: latestFrame.height*scale,
        data: encodeFrame(latestFrame).toString("base64")
      });
    };
    // if (udpConnected){
    //   dgram.send(encodeFrame(rect).toString("base64"),);
    //   console.log('Message sent to oF app.');
    // };
  };
}


 /**
  * Translates raw frame data from VNC server into PNG buffer
  * @param {*} rect 
  */
function encodeFrame(rect) {
  var png = downscale(rect);

  //return buffer
  return PNG.sync.write(png.pack());
}

/**
 * Saves raw frame data from VNC server as PNG image
 * @param {*} rect 
 */
function newImage(rect){
  var png = downscale(rect);

  png.pack()
    .pipe(fs.createWriteStream(__dirname + '/newfile'+fileNum+'.png'))
    .on('finish', function() {
      console.log('Written!');
    });
  fileNum++;
}

/**
 * Simple downscaling of frame by omission of pixels
 * - also re-orders data values into PNG format
 * @param {*} input 
 */
function downscale(input){
  //create buffer object from original image
  var bgra = Buffer.from(input.data);
  
  //create png with new dimensions
  var output = new PNG({
    width: input.width*scale,
    height: input.height*scale
  });

  var offset = 0;

  //set pixel values for png:
  //swap values of every 'r' & 'b', and skip over 'a' value
  for (var i = 0; i < output.height*output.width*4; i += 4) { //iterate through the new image
      output.data[i] = bgra[offset+2];
      output.data[i+1] = bgra[offset+1];
      output.data[i+2] = bgra[offset];
      output.data[i+3] = 0xff;
      if (offset > 0 && offset%(input.width*4) == 0){ //at every new row in png
        offset += input.width*4; //skip alternate rows
      }
      offset += 8; //skip alternate columns
  }

  return output;
}

/***************************************************************************
 * UDP CONNECTION TO oF APP
 */
//listens for incoming data on port 11999
dgram.bind(11999, "127.0.0.1");
//sends data on port 11888
const sendAdd = "127.0.0.1";
const sendPort = 11888;

dgram.on('error', (err) => {
  console.log(`server error:\n${err.stack}`);
  //udpConnected = false;
  dgram.close();
});

// dgram.on('message', (msg, rinfo) => { //c++ app always sends image data
//   //console.log(`server got: ${msg} from ${rinfo.address}:${rinfo.port}`);
//   var message = String(msg);
//   if (message[0]=="d"){
//       message = message.substring(1);
//       var dimensions = message.split(',');
//       width = dimensions[0];
//       height = dimensions[1];
//   }else{
//     data = message;
//   }
// });

dgram.on('listening', () => {
  console.log(`UDP server listening on ${dgram.address().port}`);
});

/**
 * Confirms that oF app is ready to receive
 */
/*function UDPInit(){
  dgram.send("init", sendPort, sendAdd);
  if (dgram.on('message', function (msg, rinfo){
    if (String(msg) == 'init'){
      udpConnected = true;
      return true;
    }else{
      console.log('oF app not responding. Trying again...');
      return UDPInit();
    };
  }));
};*/

/**
 * Sends string message continuously until confirmation of message received
 * @param {*string} sendingMsg 
 */
function UDPSend(sendingMsg){
  dgram.send(sendingMsg, sendPort, sendAdd);
  return new Promise(function (resolve, reject) {
    var timeout = setTimeout(function () {
      reject();
    }, 450);
    if (dgram.on('message', function (msg, rinfo){
      //console.log(msg.toString('ascii'));
      if (msg.toString('ascii') == "received"){
        //console.log('client received message');
        return true;
      }
    }));
  });
};

/**
 * Sends touch pos and clears array after sending
 * @param {*bool} end 
 */
function sendTouchPos(end = false){ //access client touchPos
  if(touchPos.length > 0){//send pos only when there is something to send
    var sent;
    console.log('sending touchPos');
    sent = UDPSend(touchPos.join());
    if (sent){
      touchPos = []; //clear pos array every time array sent
    }
  }
}