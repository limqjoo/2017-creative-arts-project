#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofEnableAlphaBlending();
	ofSetupScreen();
	// Sets the current process as dots per inch (dpi) aware, so as to get real resolution
	ofHideCursor();

	// Set parameters for screenshot
	a.x = 0;
	a.y = 0;
	b.x = GetSystemMetrics(SM_CXSCREEN)/2; //get screen resolution
	b.y = GetSystemMetrics(SM_CYSCREEN)/2;
	//b.x = 900;
	//b.y = 900;

	shaderBlurX.load("shaderBlurX");
	shaderBlurY.load("shaderBlurY");
	shaderPixelate.load("shaderPixelate");

	imageMask.loadImage("brush.png");
	cursor.loadImage("cursor.png");

	maskFbo.allocate(ofGetWidth(), ofGetHeight());
	fboOnePass.allocate(ofGetWidth(), ofGetHeight());
	fboTwoPass.allocate(ofGetWidth(), ofGetHeight());

	// Clear the FBO's
	// otherwise it will bring some junk with it from the memory
	maskFbo.begin();
	ofClear(0, 0, 0, 255);
	maskFbo.end();

	frame.allocate(b.x, b.y, GL_RGBA);
}

//--------------------------------------------------------------
void ofApp::update() {
	frame.loadData(screenGrab(a, b)); //load each new frame as ofTexture
}

//--------------------------------------------------------------
void ofApp::draw() {
	//ofSetColor(255);
	// this is our alpha mask which we draw into.
	if (brushDown) {
		maskFbo.begin();
		if (mode == 0) { //blur
			int brushImageSize = 50;
			int brushImageX = mouseX - brushImageSize * 0.5;
			int brushImageY = mouseY - brushImageSize * 0.5;
			imageMask.draw(brushImageX, brushImageY, brushImageSize, brushImageSize);
		}
		else if (mode == 1) { //pixelate
			int halfPix;
			int drawPosX = floor(ofGetMouseX() / pixelSize)*pixelSize;
			int drawPosY = floor(ofGetMouseY() / pixelSize)*pixelSize;
			if (pixelSize % 2 == 0) {
				halfPix = (pixelSize * 3 / 2);
			}
			else {
				halfPix = ceil(pixelSize * 3 / 2);
			}
			for (int X = 0; X < 3; X++) { //x-columns
				for (int Y = 0; Y < 3; Y++) { //y-columns
					ofSetColor(10, 0, 0); //assign 'pixel' area through colours
					ofDrawRectangle(drawPosX + pixelSize*(X - 1), drawPosY + pixelSize*(Y - 1), pixelSize, pixelSize);
				}
			}
		}
		maskFbo.end();
	}
	if (mode == 0) {
		fboOnePass.begin();
		ofClear(0, 0, 0, 0);

		shaderBlurX.begin();
		shaderBlurX.setUniformTexture("maskTex", maskFbo.getTextureReference(), 1);
		frame.draw(0, 0);
		shaderBlurX.end();

		fboOnePass.end();

		fboTwoPass.begin();
		ofClear(0, 0, 0, 0);

		shaderBlurY.begin();
		shaderBlurY.setUniformTexture("maskTex", maskFbo.getTextureReference(), 1);
		fboOnePass.draw(0, 0);
		shaderBlurY.end();

		fboTwoPass.end();
	}
	else if (mode == 1) {
		fboTwoPass.begin();
		ofClear(0, 0, 0, 0);

		shaderPixelate.begin();
		shaderPixelate.setUniform1i("pixelSize", pixelSize);
		shaderPixelate.setUniformTexture("maskTex", maskFbo.getTextureReference(), 1);
		frame.draw(0, 0);
		shaderPixelate.end();

		fboTwoPass.end();
	}
	ofPushMatrix();
	fboTwoPass.draw(0, 0);
	fboTwoPass.draw(0, ofGetHeight()/2);
	ofPopMatrix();
	//fboTwoPass.draw(ofGetWidth() / 2, 0);
	//draw cursor
	POINT p;
	if (GetCursorPos(&p))
	{
		if (p.x < ofGetWidth() && p.y < ofGetHeight()/2) {
			cursor.draw(p.x, p.y, 16, 23);
			cursor.draw(p.x, p.y+ofGetHeight()/2, 16, 23);
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == 'a') {
		mode++;
		mode %= 2;
		maskFbo.begin();
		ofClear(0, 0, 0, 255);
		maskFbo.end();
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
		brushDown = true;
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	brushDown = false;
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
