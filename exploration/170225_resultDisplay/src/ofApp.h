#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void sendQuery(string Q);
		string translateQuery(string Q);
		void gotMessage(ofMessage msg);
		
		//web query
		string input;
		bool typingInput = true;
		bool queryRetreived = false;
		ofHttpResponse resp;

		//user input history
		//vector<string> textEntries;
		//void addText(string& s);
		//void drawEntries();

		//user input
		string text = "";
		int position = 0;
		int cursorx = 0;
		int cursory = 0;
};
