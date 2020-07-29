#pragma once

#include "ofMain.h"
#include "screenGrab.h"

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
		void gotMessage(ofMessage msg);

		POINT a, b;
		bool brushDown = false;

		ofShader shaderBlurX;
		ofShader shaderBlurY;
		ofShader shaderPixelate;
		int pixelSize = 5;

		ofTexture frame;
		ofImage imageMask;

		ofFbo maskFbo;
		ofFbo fboOnePass;
		ofFbo fboTwoPass;

		int mode = 0;
};
