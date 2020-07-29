#pragma once

#include "ofMain.h"
#include "ofxGui.h"
#include "canvas.h"

class ofApp : public ofBaseApp{

	public:
		void setup();
		void setupGui();
		void update();
		void draw();
		void drawGui(ofEventArgs & args);

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
		void gotMessage(ofMessage msg);
		
		bool mPressed = false;
		int mouseX, mouseY;

		Canvas distortion;

		ofPoint a, b;
		ofPixels frame;
		ofPixels output;
		ofTexture texture;
};
