#include "ofMain.h"
#include "ofApp.h"

//========================================================================
int main( ){
	SetProcessDPIAware();

	ofGLWindowSettings settings;
	settings.setGLVersion(2, 1);
	settings.width = GetSystemMetrics(SM_CXSCREEN);
	settings.height = GetSystemMetrics(SM_CYSCREEN);
	//settings.windowMode = OF_FULLSCREEN;
	ofCreateWindow(settings);
	
	ofRunApp(new ofApp());
}
