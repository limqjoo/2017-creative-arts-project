#include "ofApp1.h"

//--------------------------------------------------------------
void ofApp1::setup(){
	//load image
}

//--------------------------------------------------------------
void ofApp1::update(){

}

//--------------------------------------------------------------
void ofApp1::draw(){
	ofFill();
	if (vM.mouseDragged()) {
		//draw cursor
		ofSetColor(255);
		ofDrawCircle(vM.getMouseX(), vM.getMouseY(), 2);
	}
	if (vM.mousePressed()) {
		//draw cursor
		ofSetColor(0);
		ofDrawCircle(vM.getMouseX(), vM.getMouseY(), 2);
	}
}

//--------------------------------------------------------------
void ofApp1::keyPressed(int key){

}

//--------------------------------------------------------------
void ofApp1::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp1::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp1::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp1::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp1::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp1::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp1::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp1::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp1::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp1::dragEvent(ofDragInfo dragInfo){ 

}
