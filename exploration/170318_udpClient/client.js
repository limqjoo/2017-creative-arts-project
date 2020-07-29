var pixelValues;
var gotData = false;

if ("WebSocket" in window){
    var ws = new WebSocket('ws://127.0.0.1:8080');
    
    ws.onopen = function(){
        console.log('Connection open!');
        ws.send('getData');
    };

    // Handle any errors that occur.
    ws.onerror = function(error) {
        console.log('WebSocket Error: ' + error);
    };

    ws.onmessage = function(event) {
        pixelValues = splitIntoArray(event.data);
        //console.log(pixelValues);
        gotData = true;
        ws.send('getData');
    }

    ws.onclose = function(){ 
                // websocket is closed.
                //alert("Connection is closed..."); 
    };
}else{
    // The browser doesn't support WebSocket
    alert("WebSocket NOT supported by your Browser!");
}

//splits string of pixel values into array
function splitIntoArray(str){
    var int16 = new Int16Array(str.length/3);
    for (var i = 0; i < str.length/3; i++){
        int16[i] = Number(str.substring(i*3,i*3+3).trim());
    }
    return int16;
}