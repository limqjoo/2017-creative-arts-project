(function (w) {
  'use strict';

  /**
   * Screen object
   * @param {*} canvas 
   */
  function Screen(canvas) {
    this._canvas = canvas;
    this._touchId;
    this._touched = false;
    this._touchPos = [];
    this._screenW;
    this._screenH;
    this._horiMargin;
    this._vertMargin;
  }

  /**
    * Process input frame
    * shaders do their stuff
    * and then display output
    */
  Screen.prototype.processFrame = function (rect) {
    var self = this;
    var img = new Image();
    img.width = rect.width;
    img.height = rect.height;
    img.src = 'data:image/png;base64,' + rect.data;
    //img to texture
    img.onload = function () {
      //webgl stuff here
      render(img, self._canvas);
    };
  };

  /**
    * Adds touch event listeners
    * - only allow one touch at a time to register
    * - only register touches within canvas element
    */
  Screen.prototype.addTouchHandler = function (cb) {
    var self = this;
    if (!this._touched){ // if touch has been registered, the following touches will not matter when the first touch has not been cancelled
      this._canvas.addEventListener('touchstart', this._ontouchstart = function(e){
        //only register when touch is within canvas element
        if (e.targetTouches[0].pageX-self._horiMargin > 0 && e.targetTouches[0].pageX-self._horiMargin < self._screenW && e.targetTouches[0].pageY-self._vertMargin > 0 && e.targetTouches[0].pageY-self._vertMargin < self._screenH){
          this._touchId = e.targetTouches[0].identifier;
          cb.call(null, e.targetTouches[0].pageX-self._horiMargin, e.targetTouches[0].pageY-self._vertMargin, 'start');
          this._touched = true;
          e.preventDefault();
        }
      });
    };
    this._canvas.addEventListener('touchmove', this._ontouchmove = function(e){
      for (var i = 0; i < e.targetTouches.length; i++){
        if (e.targetTouches[i].identifier == this._touchId){
          if (e.targetTouches[0].pageX-self._horiMargin > 0 && e.targetTouches[0].pageX-self._horiMargin < self._screenW && e.targetTouches[0].pageY-self._vertMargin > 0 && e.targetTouches[0].pageY-self._vertMargin < self._screenH){
            cb.call(null, e.targetTouches[i].pageX-self._horiMargin, e.targetTouches[i].pageY-self._vertMargin, 'move');
            e.preventDefault();
          }
        };
      };
    });
    this._canvas.addEventListener('touchend', this._ontouchend = function(e){
      for (var i = 0; i < e.targetTouches.length; i++){
        if (e.targetTouches[i].identifier == this._touchId){
          if (e.targetTouches[0].pageX-self._horiMargin > 0 && e.targetTouches[0].pageX-self._horiMargin < self._screenW && e.targetTouches[0].pageY-self._vertMargin > 0 && e.targetTouches[0].pageY-self._vertMargin < self._screenH){
            cb.call(null, e.changedTouches[i].pageX-self._horiMargin, e.changedTouches[i].pageY-self._vertMargin, 'end');
            this._touched = false;
            e.preventDefault();
          }
        };
      };
    });
  };

  /**
    * Removes all event listeners
    */
  Screen.prototype.removeHandlers = function () {
    // this._canvas.removeEventListener('mouseup', this._onmouseup);
    // this._canvas.removeEventListener('mousedown', this._onmousedown);
    // this._canvas.removeEventListener('mousemove', this._onmousemove);
    this._canvas.removeEventListener('touchstart', this._ontouchstart);
    this._canvas.removeEventListener('touchmove', this._ontouchmove);
    this._canvas.removeEventListener('touchend', this._ontouchend);
  };

  Screen.prototype.getCanvas = function () {
    return this._canvas;
  };

  Screen.prototype.getTouchPos = function () {
    var pos = this._touchPos;
    this._touchPos = []; //clear array
    return pos;
  };

  /**
   * centers canvas
   * takes transformed canvas width and height
   * to be called once screen object is initialised and whenever window is resized
   */
  Screen.prototype.center = function(screenW, screenH){
    var self = this;
    //get dimensions of viewport
    var width = $(window).width();
    var height = $(window).height();
    self._screenW = screenW;
    self._screenH = screenH;

    //calculate margins
    self._horiMargin = parseFloat((width-screenW)/2);
    self._vertMargin = parseFloat((height-screenH)/2);

    //update canvas position
    $('#screen').css('margin', self._vertMargin+'px '+self._horiMargin+'px');
  }

  w.Screen = Screen;
}(window));

// Screen.prototype.addMouseHandler = function (cb) {
  //   this._canvas.addEventListener('mousedown', this._onmousedown = function(e) {
  //     cb.call(null, e.pageX, e.pageY, 1);
  //     e.preventDefault();
  //   }, false);
  //   this._canvas.addEventListener('mouseup', this._onmouseup = function(e) {
  //     state = 0;
  //     cb.call(null, e.pageX, e.pageY, 0);
  //     e.preventDefault();
  //   }, false);
  //   this._canvas.addEventListener('mousemove', this._onmousemove = function(e) {
  //     cb.call(null, e.pageX, e.pageY, 2);
  //     e.preventDefault();
  //   });
  // };

/**
 * WEBGL stuff
 * @param {*} image 
 * @param {*} canvas 
 */
function render(image, canvas) {
  // Get A WebGL context
  /** @type {HTMLCanvasElement} */
  var gl = canvas.getContext("webgl");
  if (!gl) {
    return;
  }

  // setup GLSL program
  var program = webglUtils.createProgramFromScripts(gl, ["2d-vertex-shader", "2d-fragment-shader"]);

  // look up where the vertex data needs to go.
  var positionLocation = gl.getAttribLocation(program, "a_position");
  var texcoordLocation = gl.getAttribLocation(program, "a_texCoord");

  // Create a buffer to put three 2d clip space points in
  var positionBuffer = gl.createBuffer();

  // Bind it to ARRAY_BUFFER (think of it as ARRAY_BUFFER = positionBuffer)
  gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);
  // Set a rectangle the same size as the image.
  setRectangle(gl, 0, 0, image.width, image.height);

  // provide texture coordinates for the rectangle.
  var texcoordBuffer = gl.createBuffer();
  gl.bindBuffer(gl.ARRAY_BUFFER, texcoordBuffer);
  gl.bufferData(gl.ARRAY_BUFFER, new Float32Array([
      0.0,  0.0,
      1.0,  0.0,
      0.0,  1.0,
      0.0,  1.0,
      1.0,  0.0,
      1.0,  1.0,
  ]), gl.STATIC_DRAW);

  // Create a texture.
  var texture = gl.createTexture();
  gl.bindTexture(gl.TEXTURE_2D, texture);

  // Set the parameters so we can render any size image.
  gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_S, gl.CLAMP_TO_EDGE);
  gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_WRAP_T, gl.CLAMP_TO_EDGE);
  gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MIN_FILTER, gl.NEAREST);
  gl.texParameteri(gl.TEXTURE_2D, gl.TEXTURE_MAG_FILTER, gl.NEAREST);

  // Upload the image into the texture.
  gl.texImage2D(gl.TEXTURE_2D, 0, gl.RGBA, gl.RGBA, gl.UNSIGNED_BYTE, image);

  // lookup uniforms
  var resolutionLocation = gl.getUniformLocation(program, "u_resolution");

  webglUtils.resizeCanvasToDisplaySize(gl.canvas);

  // Tell WebGL how to convert from clip space to pixels
  gl.viewport(0, 0, gl.canvas.width, gl.canvas.height);

  // Clear the canvas
  gl.clearColor(0, 0, 0, 0);
  gl.clear(gl.COLOR_BUFFER_BIT);

  // Tell it to use our program (pair of shaders)
  gl.useProgram(program);

  // Turn on the position attribute
  gl.enableVertexAttribArray(positionLocation);

  // Bind the position buffer.
  gl.bindBuffer(gl.ARRAY_BUFFER, positionBuffer);

  // Tell the position attribute how to get data out of positionBuffer (ARRAY_BUFFER)
  var size = 2;          // 2 components per iteration
  var type = gl.FLOAT;   // the data is 32bit floats
  var normalize = false; // don't normalize the data
  var stride = 0;        // 0 = move forward size * sizeof(type) each iteration to get the next position
  var offset = 0;        // start at the beginning of the buffer
  gl.vertexAttribPointer(
      positionLocation, size, type, normalize, stride, offset)

  // Turn on the teccord attribute
  gl.enableVertexAttribArray(texcoordLocation);

  // Bind the position buffer.
  gl.bindBuffer(gl.ARRAY_BUFFER, texcoordBuffer);

  // Tell the position attribute how to get data out of positionBuffer (ARRAY_BUFFER)
  var size = 2;          // 2 components per iteration
  var type = gl.FLOAT;   // the data is 32bit floats
  var normalize = false; // don't normalize the data
  var stride = 0;        // 0 = move forward size * sizeof(type) each iteration to get the next position
  var offset = 0;        // start at the beginning of the buffer
  gl.vertexAttribPointer(
      texcoordLocation, size, type, normalize, stride, offset)

  // set the resolution
  gl.uniform2f(resolutionLocation, gl.canvas.width, gl.canvas.height);

  // Draw the rectangle.
  var primitiveType = gl.TRIANGLES;
  var offset = 0;
  var count = 6;
  gl.drawArrays(primitiveType, offset, count);
}

function setRectangle(gl, x, y, width, height) {
  var x1 = x;
  var x2 = x + width;
  var y1 = y;
  var y2 = y + height;
  gl.bufferData(gl.ARRAY_BUFFER, new Float32Array([
     x1, y1,
     x2, y1,
     x1, y2,
     x1, y2,
     x2, y1,
     x2, y2,
  ]), gl.STATIC_DRAW);
}