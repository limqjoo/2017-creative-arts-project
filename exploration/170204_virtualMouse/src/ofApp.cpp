#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetBackgroundAuto(false);
	ofBackground(255);
}

//--------------------------------------------------------------
void ofApp::setupGui() {
	ofSetBackgroundAuto(false);
	ofBackground(255);
}

//--------------------------------------------------------------
void ofApp::update() {

}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(0);
	if (mPressed) {
		ofDrawCircle(ofGetMouseX(), ofGetMouseY(), 2);
	}
}

//--------------------------------------------------------------
void ofApp::drawGui(ofEventArgs & args) {
	ofSetColor(0);
	if (mPressed) {
		ofDrawCircle(mouseX, mouseY, 2);
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	mouseX = x;
	mouseY = y;
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	mPressed = true;
	mouseX = x;
	mouseY = y;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	mPressed = false;
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