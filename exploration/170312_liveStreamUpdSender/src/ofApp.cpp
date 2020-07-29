#include "ofApp.h"

#define RECONNECT_TIME 400

//--------------------------------------------------------------
void ofApp::setup() {
	// we don't want to be running too fast
	ofSetVerticalSync(true);
	ofSetFrameRate(10);

	//create the socket and set to send to 127.0.0.1:11999
	udpConnection.Create();
	udpConnection.Connect("127.0.0.1", 11999);
	udpConnection.SetNonBlocking(true);

	ofEnableAlphaBlending();
	ofSetupScreen();
	// Sets the current process as dots per inch (dpi) aware, so as to get real resolution
	SetProcessDPIAware();

	//Set parameters for screenshot
	a.x = 0;
	a.y = 0;
	//b.x = GetSystemMetrics(SM_CXSCREEN) / 2; //get screen resolution
	//b.y = GetSystemMetrics(SM_CYSCREEN) / 2;
	b.x = 640;
	b.y = 480;

	frame.allocate(b.x, b.y, GL_RGBA);

	numX = frame.getWidth() / 80;
	numY = frame.getHeight() / 80;

	//tell webapp size of frame
	string dimensions = "d" + to_string(b.x - a.x) + "," + to_string(b.y - a.y);
	udpConnection.Send(dimensions.c_str(), dimensions.length());
}

//--------------------------------------------------------------
void ofApp::update() {
	if (!paused) {
		if (ofGetFrameNum() % (numX*numY) == 0) {
			screen = screenGrab(a, b); //get new screengrab every numX*numY frames
			//frame.loadData(screenGrab(a, b)); //load each new frame as ofTexture
			gotData = true;
		}
		if (gotData) {
			string message = "";
			//send parts of frame in 80x80 packets
			int w = ofGetFrameNum() % numX;
			int h = int(floor(ofGetFrameNum() / numX)) % numY;
			
			message = to_string(w) + "," + to_string(h) + ","; //send index for putting together the parts in the client side
			for (int y = 0; y < 80; y++) {
				for (int x = 0; x < 80; x++) {
					int X = w * 80 + x;
					int Y = h * 80 + y;
					message += pad(screen.getColor(X, Y).r) + pad(screen.getColor(X, Y).g) + pad(screen.getColor(X, Y).b);
				}
			}
			udpConnection.Send(message.c_str(), message.length());
			message = "";
		}
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	//frame.draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == 'p') {
		paused = !paused;
		cout << "paused: " << paused << endl;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {


}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	//tell webapp size of frame
	string dimensions = "d" + to_string(b.x - a.x) + "," + to_string(b.y - a.y);
	udpConnection.Send(dimensions.c_str(), dimensions.length());
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

string ofApp::pad(int v) {
	string value = to_string(v);
	while (value.length() < 3) {
		value = " " + value;
	}
	return value;
}
