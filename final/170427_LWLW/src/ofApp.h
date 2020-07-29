#pragma once

#include "ofxNetwork.h"
#include "ofxAwesomium.h"
#include "ofMain.h"
#include <algorithm>
#include <regex>
#include <string>

using namespace std;

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
	void gotMessage(ofMessage msg);

	string pad(int v);
	void processMsg(string msg);
	void search();
	void drawDistortion();

	//variables & objects
	//UDP
	ofxUDPManager udpConnection;
	bool gotData = false;
	bool paused = true; //pause stream

	//search UI
	ofTrueTypeFont myfont;
	ofImage google, googleHover, ddg, ddgOverlay, ddgHover, ask;
	bool fullscreen = true;
	bool textTyped = false;
	string userInput; //stores what user typed
	int inputWidth;//pixel length of string input
	bool searchHover = false;
	ofPoint googleButton = ofPoint(881, 602);
	ofPoint ddgButton = ofPoint(1259, 594);
	ofRectangle askButton = ofRectangle(1466, 450, 48, 48);
	ofRectangle googleTextBox = ofRectangle(623, 516, 672, 30);
	ofRectangle ddgTextBox = ofRectangle(606, 609, 600, 30);
	ofRectangle askTextBox = ofRectangle(573, 470, 872, 22);
	vector<ofRectangle> textBoxes;
	int textCursor = 0; //blinking line at where the next character is inserted
	bool overflow = false;
	string prompts[3] = { "What would you like to know about today?", "Type something.", "Did you mean '" };
	string prompt;
	int p = 0; //prompt index
	bool showHint = false; //shows hint as prompt

	//search function
	string query;
	bool searched = false;
	int engine = 0;
	ofHttpResponse resp;
	ofxAwesomium browser;
	int sinceSearch = 0; //time since searched

	//shaders
	int mode = 0; //shader mode
	vector<ofPoint> touchPos; //stores touch positions from tablet
	ofShader shaderBlurX;
	ofShader shaderBlurY;
	ofShader shaderPixelate;
	int pixelSize = 10;
	ofImage imageMask; //brush image that will be drawn into maskFbo
	ofFbo maskFbo;
	ofFbo screen;
	ofFbo fboOnePass;
	ofFbo fboTwoPass;
};

