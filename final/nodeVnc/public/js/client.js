/**
 * Client object
 * @param {object} screen 
 */

function Client(screen) {
  this._screen = screen;
  this._scaleFactor = 1;
  this._socket = io('http://192.168.1.87:8090'); //change address to host computer's address
  this._mouseDown = false;
  this._touchPos = [];
  this._sendTouchPosTimer; //for interval timer
}

/**
 * Send details to connect to VNC server
 * @param {object} config
 */
Client.prototype.connect = function (config) {
  var self = this;
  self._socket.emit('init', {
    host: config.host,
    port: config.port,
    password: config.password
  });
  self._disconnect = false;
  return self._addHandlers();
};

/**
 * Handles what happens after attempting to connect to VNC server
 */
Client.prototype._addHandlers = function () {
  var self = this;
  return new Promise(function (resolve, reject) {
    var timeout = setTimeout(function () {
      reject();
    }, 9000);
    self._socket.on('error', function(data){
      console.log(data.error);
    })
    self._socket.on('init', function (config) {
      console.log('Connected to server!');
      //hide form, show canvas to display received images
      $("#non-index").show().trigger("updatelayout");
      $("#form-wrapper").hide().trigger("updatelayout");
      var canvas = self._screen.getCanvas();
      canvas.width = config.width;
      canvas.height = config.height;
      self.scaleScreen(canvas);
      self._initEventListeners();
      resolve();
      clearTimeout(timeout);
      //this sends touchPos every half a second
      self._sendTouchPosTimer = setInterval(function(){
        if (self._touchPos.length > 0){
          self._socket.emit('touch', {data: self._touchPos});
          self._touchPos = [];
        }
      }, 250); //set timer to send array of coordinates to server every half a second
    });
    self._socket.on('frame', function (frame) {
      self._screen.processFrame(frame);
    });
  });
};

Client.prototype._initEventListeners = function () {
  var self = this;
  self._screen.addTouchHandler(function (x, y, state) {
    x /= self._scaleFactor;
    y /= self._scaleFactor;
    console.log(x+', '+y);
    //multiply by 2 because of downscaling
    self._touchPos.push(Math.round(2*x));
    self._touchPos.push(Math.round(2*y));
    //format of array:
    // [x1, y1, x2, y2, x3, y3, ..., xN, yN]
  });
};

Client.prototype.scaleScreen = function () {
  var self = this;
  var canvas = self._screen.getCanvas();
  var sw = (screen.availWidth * 0.7) / canvas.width;
  var sh = (screen.availHeight * 0.7) / canvas.height;
  var s = Math.min(sw, sh);
  self._scaleFactor = s;
  var transform = 'scale(' + s + ')';
  canvas.style.mozTransform = transform;
  canvas.style.webkitTransform = transform;
  canvas.style.transform = transform;
  //center screen after scaling
  self._screen.center(canvas.width*s, canvas.height*s);
};

Client.prototype.changeMode = function (changedTo) {
    var self = this;
    self._socket.emit('mode', {mode: changedTo});
}

Client.prototype.disconnect = function () {
  var self = this;
  self._socket.disconnect();
  clearInterval(self._sendTouchPosTimer);
  console.log('Disconnected!');
};