var c=document.getElementById("myCanvas");
var ctx=c.getContext("2d");

if (gotData){
        var width = 50;
        var height = 50;
        var imgData = ctx.createImageData(width, height); // width x height
        var data = imgData.data;

        // copy img byte-per-byte into our ImageData
        for (var i = 0; i < pixelValues.length; i++) {
            data[i] = pixelValues[i];
        }

        // now we can draw our imagedata onto the canvas
        gl.putImageData(imgData, 0, 0);
    }