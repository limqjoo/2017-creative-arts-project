#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	//ofSetupOpenGL(900,900,OF_WINDOW);			// <-------- setup the GL context
	ofGLWindowSettings settings;
	settings.setGLVersion(3, 2);
	settings.width = 900;
	settings.height = 900;
	ofCreateWindow(settings);
	// this kicks off the running of my app
	// can be OF_WINDOW or OF_FULLSCREEN
	// pass in width and height too:
	ofRunApp(new ofApp());

}
