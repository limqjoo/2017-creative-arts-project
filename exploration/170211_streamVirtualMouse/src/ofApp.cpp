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
	b.x = 910;
	b.y = 910;

	texture.allocate(b.x, b.y, GL_RGB);
	output.allocate(b.x, b.y, GL_RGB);
	ofLoadImage(output, "image_sq.jpg");
	distortion.setup(a, b);
}

//--------------------------------------------------------------
void ofApp::setupGui() {
	
}

//--------------------------------------------------------------
void ofApp::update(){
	
	texture.loadData(output);
}

//--------------------------------------------------------------
void ofApp::draw(){
	texture.draw(0, 0, b.x, b.y);
}

//--------------------------------------------------------------
void ofApp::drawGui(ofEventArgs & args) {
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
void ofApp::mouseDragged(int x, int y, int button) {
	mouseX = x;
	mouseY = y;
	(distortion.update(frame)).pasteInto(output, 0, 0);
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	mPressed = true;
	mouseX = x;
	mouseY = y;
	distortion.start(frame);
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	mPressed = false;
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
