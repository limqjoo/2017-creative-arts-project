const http = require('http');
const fs = require('fs');

const dgram = require('dgram');
const server = dgram.createSocket('udp4');

var WebSocket = require('ws');
const wss = new WebSocket.Server({host: "127.0.0.1", port: 8080});
const ws = new WebSocket("ws://127.0.0.1:8080");
var width, height, data;
var wsConnected = false;
var udpConnected = false;
var received_msg;
 
//serve web pages
http.createServer(function(request, response) {

	if(request.url === "/index"){
		sendFileContent(response, "index.html", "text/html");
	}
	else if(request.url === "/"){
		response.writeHead(200, {'Content-Type': 'text/html'});
		response.write('<b>Hey there!</b><br /><br />This is the default response. Requested URL is: ' + request.url);
	}
	else if(/^\/[a-zA-Z0-9\/]*.js$/.test(request.url.toString())){
		sendFileContent(response, request.url.toString().substring(1), "text/javascript");
	}
	else if(/^\/[a-zA-Z0-9\/]*.css$/.test(request.url.toString())){
		sendFileContent(response, request.url.toString().substring(1), "text/css");
	}
	else{
		console.log("Requested URL is: " + request.url);
		response.end();
	}
}).listen(443);

function sendFileContent(response, fileName, contentType){
	fs.readFile(fileName, function(err, data){
		if(err){
			response.writeHead(404);
			response.write("Not Found!");
		}
		else{
			response.writeHead(200, {'Content-Type': contentType});
			response.write(data);
		}
		response.end();
	});
}

//websocket - server <-> html5
wss.on('connection', function(ws) {
    ws.send(String(width)+","+String(height));
    wsConnected = true;

    ws.on('message', function incoming(msg, flags) {
        //console.log(msg);
        received_msg = msg;
        if (received_msg == 'getData' && udpConnected){
            ws.send(data);
        }
    });
});

//dgram udp connection - c++ <-> server
server.on('error', (err) => {
  console.log(`server error:\n${err.stack}`);
  server.close();
});

server.on('message', (msg, rinfo) => { //c++ app always sends image data
  //console.log(`server got: ${msg} from ${rinfo.address}:${rinfo.port}`);
  var message = String(msg);
  if (message[0]=="d"){
      message = message.substring(1);
      var dimensions = message.split(',');
      width = dimensions[0];
      height = dimensions[1];
  }else{
    data = message;
  }
});

server.on('listening', () => {
  var address = server.address();
  console.log(`server listening ${address.address}:${address.port}`);
  udpConnected = true;
});

server.bind(11999, "127.0.0.1");