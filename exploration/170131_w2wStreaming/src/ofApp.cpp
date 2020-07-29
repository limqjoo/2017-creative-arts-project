#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofEnableAlphaBlending();
	ofSetupScreen();
	// Sets the current process as dots per inch (dpi) aware, so as to get real resolution
	//SetProcessDPIAware();

	// Set parameters for screenshot
	a.x = 0;
	a.y = 0;
	//b.x = GetSystemMetrics(SM_CXSCREEN); //get screen resolution
	//b.y = GetSystemMetrics(SM_CYSCREEN);
	b.x = 900;
	b.y = 900;

	texture.allocate(b.x, b.y, GL_RGBA);

	//screenGrab(a, b);
}

//--------------------------------------------------------------
void ofApp::update(){
	ofPixels frame = screenGrab(a, b);

	texture.loadData(frame);
}

//--------------------------------------------------------------
void ofApp::draw(){
	texture.draw(0, 0, b.x, b.y);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}
