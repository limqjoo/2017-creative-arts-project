#include "vMouse.h"
#include "ofApp2.h"

void vMouse::settings(ofApp2 * App)
{
	app = App;
}

void vMouse::updateMouse(int x, int y)
{
	mouse.x = x;
	mouse.y = y;
}

bool vMouse::mouseDragged()
{
	return &app->mouseDragged;
}

bool vMouse::mousePressed()
{
	return &app->mousePressed;
}

int vMouse::getMouseX()
{
	return mouse.x;
}

int vMouse::getMouseY()
{
	return mouse.y;
}
