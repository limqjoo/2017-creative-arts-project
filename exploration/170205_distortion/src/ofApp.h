#pragma once

#include "ofMain.h"

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

	void pixelate(int pixelSize = 5);
	void blurStart();
	void blurUpdate(int radius = 15);
	void smudgeStart(int radius = 15, float smudgeStrength = 0.5); //0 or 2 arguments needed
	bool smudgeUpdate(float resistance = 0.005);

	int r = 0;

	/*double blurMatrix[25] = { 1 / 256, 4 / 256, 6 / 256, 4 / 256, 1 / 256,
						4 / 256, 16 / 256, 24 / 256, 16 / 256, 4 / 256,
						16 / 256, 24 / 256, 36 / 256, 24 / 256, 16 / 256,
						4 / 256, 16 / 256, 24 / 256, 16 / 256, 4 / 256,
						1 / 256, 4 / 256, 6 / 256, 4 / 256, 1 / 256 };*/
	ofPixels blurImg;
	double blurMatrix[25] = { 0.003776, 0.015095, 0.022643, 0.015095, 0.003776,
		0.015095, 0.060378, 0.090567, 0.060378, 0.015095,
		0.022643, 0.090567, 0.135850, 0.090567, 0.022643,
		0.015095, 0.060378, 0.090567, 0.060378, 0.015095,
		0.003776, 0.015095, 0.022643, 0.015095, 0.003776 };
	//double blurMatrix[25] = {}

	float smudgeStrength = 0;
	vector <ofColor> smudgeColours;

	ofPixels p, pOut;
	ofTexture texture;
};
