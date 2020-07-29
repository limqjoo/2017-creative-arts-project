#pragma once
#include "ofMain.h"
#include "screenGrab.h"

class Canvas {
public:
	Canvas() {};
	~Canvas() {};
	void setup(ofPoint A, ofPoint B);
	void getFrame();
	void start(ofPixels img);
	ofPixels update(ofPixels img);
	void draw();
	void switchMode();
	bool switchMode(int newMode);

private:
	void pixelate(int pixelSize = 5);
	void blurStart();
	void blurUpdate(int radius = 15);
	void smudgeStart(int radius = 15, float smudgeStrength = 0.5); //0 or 2 arguments needed
	bool smudgeUpdate(float resistance = 0.005);

	ofPoint A, B;
	ofPixels currentFrame;
	ofPixels output;
	int mode = 0;
	int numModes = 3;
	int r = 0;

	ofPixels blurImg;
	double blurMatrix[25] = { 0.003776, 0.015095, 0.022643, 0.015095, 0.003776,
		0.015095, 0.060378, 0.090567, 0.060378, 0.015095,
		0.022643, 0.090567, 0.135850, 0.090567, 0.022643,
		0.015095, 0.060378, 0.090567, 0.060378, 0.015095,
		0.003776, 0.015095, 0.022643, 0.015095, 0.003776 };

	float smudgeStrength = 0;
	vector <ofColor> smudgeColours;
};
