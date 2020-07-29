#include "ofMain.h"
#include "ofApp1.h"
#include "ofApp2.h"

//========================================================================
int main( ){
	ofGLWindowSettings settings;
	settings.width = 600;
	settings.height = 600;
	settings.setPosition(ofVec2f(300, 0));
	auto window1 = ofCreateWindow(settings);

	settings.width = 600;
	settings.height = 600;
	settings.setPosition(ofVec2f(0, 0));
	auto window2 = ofCreateWindow(settings);

	auto app1 = make_shared<ofApp1>();
	auto app2 = make_shared<ofApp2>();

	vM.settings(app2.get());

	ofRunApp(window1, app1);
	ofRunApp(window2, app2);
	ofRunMainLoop();
}
