#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	SetProcessDPIAware();
	int width = GetSystemMetrics(SM_CXSCREEN); //get screen resolution
	int height = GetSystemMetrics(SM_CYSCREEN);

	ofGLWindowSettings settings;
	settings.setGLVersion(3, 0);
	settings.width = width/2;
	settings.height = height;
	settings.setPosition(ofVec2f(width/2, 0));
	ofCreateWindow(settings);

	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
