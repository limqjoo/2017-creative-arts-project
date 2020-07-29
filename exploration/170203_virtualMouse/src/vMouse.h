#pragma once
#ifndef VMOUSE_H
#define VMOUSE_H

#include "ofPoint.h"
#include "ofApp2.h"

class vMouse {
public:
	vMouse() {};
	~vMouse() {};

	void settings(ofApp2 * App);
	void updateMouse(int x, int y);
	bool mouseDragged();
	bool mousePressed();
	int getMouseX();
	int getMouseY();

private:
	ofPoint mouse = ofPoint(0, 0);
	ofBaseApp* app;
};

#endif // !VMOUSE_H