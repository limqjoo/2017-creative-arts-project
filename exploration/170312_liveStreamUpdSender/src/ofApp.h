#pragma once

#include "ofxNetwork.h"
#include "ofMain.h"
#include "../../../screenGrab.h"

class ofApp : public ofBaseApp {

public:
	void setup();
	void update();
	void draw();

	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void mouseEntered(int x, int y);
	void mouseExited(int x, int y);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	string pad(int v);
	void gotMessage(ofMessage msg);

	ofxUDPManager udpConnection;

	ofTrueTypeFont  mono;
	ofTrueTypeFont  monosm;
	vector<ofPoint> stroke;

	POINT a, b;
	ofTexture frame;
	int numX, numY;

	ofPixels screen;
	bool gotData = false;
	bool paused = true; //pause stream
};

