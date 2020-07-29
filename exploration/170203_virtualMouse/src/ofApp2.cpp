#include "ofApp2.h"

//--------------------------------------------------------------
void ofApp2::setup() {

}

//--------------------------------------------------------------
void ofApp2::update() {
}

//--------------------------------------------------------------
void ofApp2::draw() {
	//draw cursor
	ofFill();
	ofSetColor(255, 0, 0);
	ofDrawCircle(vM.getMouseX(), vM.getMouseY(), 2);
}

//--------------------------------------------------------------
void ofApp2::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp2::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp2::mouseMoved(int x, int y) {
	vM.updateMouse(x, y);
}

//--------------------------------------------------------------
void ofApp2::mouseDragged(int x, int y, int button) {
	
}

//--------------------------------------------------------------
void ofApp2::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp2::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp2::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp2::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp2::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp2::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp2::dragEvent(ofDragInfo dragInfo) {

}
