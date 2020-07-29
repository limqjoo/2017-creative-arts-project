#include "ofApp.h"

#define RECONNECT_TIME 400

//--------------------------------------------------------------
void ofApp::setup() {
	ofSetVerticalSync(true);
	ofSetFrameRate(60); // 60 fps
	ofEnableAlphaBlending();

	//initialise udp connection
	udpConnection.Create(); //create the socket
	udpConnection.Connect("127.0.0.1", 11999); //set to 127.0.0.1:11999 for sending data
	udpConnection.Bind(11888); //bind to port 11888 for receiving data
	udpConnection.SetNonBlocking(true);

	//browser setup
	browser.setup(GetSystemMetrics(SM_CXSCREEN), GetSystemMetrics(SM_CYSCREEN));
	textBoxes.push_back(googleTextBox);
	textBoxes.push_back(ddgTextBox);
	textBoxes.push_back(askTextBox);

	//load assets
	google.load("images/google-index.png");
	googleHover.load("images/google-search-hover.jpg");
	ddg.load("images/ddg-index.png");
	ddgOverlay.load("images/ddg-overlay.png");
	ddgHover.load("images/ddg-search-hover.png");
	ask.load("images/ask-index.png");
	imageMask.loadImage("images/brush.png");
	myfont.loadFont("arial.ttf", 14);

	//load shaders
	shaderBlurX.load("shaders/GL2/shaderBlurX");
	shaderBlurY.load("shaders/GL2/shaderBlurY");
	shaderPixelate.load("shaders/GL2/shaderPixelate");

	//initialise FBOs & textures
	maskFbo.allocate(ofGetWidth(), ofGetHeight());
	fboOnePass.allocate(ofGetWidth(), ofGetHeight());
	fboTwoPass.allocate(ofGetWidth(), ofGetHeight());
	screen.allocate(ofGetWidth(), ofGetHeight());

	// Clear the FBOs
	// otherwise it will bring some junk with it from the memory
	maskFbo.begin();
	ofClear(0, 255);
	maskFbo.end();
}

//--------------------------------------------------------------
void ofApp::update() {
	if (ofGetFrameNum() % (2400) == 0) { //change mode automatically every 2400 frame
		mode++;
		mode %= 2;
		maskFbo.begin();
		ofClear(0, 0, 0, 255); //clear mask layer every time mode changes
		maskFbo.end();
	}

	if (sinceSearch > 60 * 45) { //more than 45 sec passed since last search
		//reset search variables
		engine = 0; //go back to first search engine
		string query = "";
		p = 0;
		showHint = false;
	}

	char udpMessage[100000];
	udpConnection.Receive(udpMessage, 100000); //listen and record message
	string message = udpMessage;
	if (message != "") { //if received message is not empty
		string reply = "received";
		udpConnection.Send(reply.c_str(), reply.length()); //reply to NodeJs app that it has been received
		processMsg(message); //translate string message into ofPoints for drawing into Mask Fbo
	}

	if (searched) {
		//update browser
		ofxAwesomium::updateCore();
		browser.update();
		if (sinceSearch > 900) { //more than 900 frames has passed since last search
			sinceSearch = 0; //return to search engine UI
			searched = false;
		}
		else if (!browser.getIsLoading()) { //if browser is fully loaded
			sinceSearch++;
		}
	}
	else {
		//determine if user input longer than textbox
		inputWidth = myfont.stringWidth(userInput);
		overflow = (inputWidth >= textBoxes[engine].width); //if string is longer than textbox, display only last part by shifting text to left

		if (userInput.length() == 0) {
			textTyped = false;
		}
		else {
			textTyped = true;
		}

		if (engine == 0) { //google UI
			//detect hover over search button
			if (ofGetMouseX() > googleButton.x && ofGetMouseX() < googleButton.x + 157 && ofGetMouseY() > googleButton.y && ofGetMouseY() < googleButton.y + 46) {
				searchHover = true;
			}
			else {
				searchHover = false;
			}
		}
		else if (engine == 1) { //duckduckgo UI
			//detect hover over search button
			if (ofGetMouseX() > ddgButton.x && ofGetMouseX() < ddgButton.x + ddgHover.getWidth() && ofGetMouseY() > ddgButton.y && ofGetMouseY() < ddgButton.y + ddgHover.getHeight()) {
				searchHover = true;
			}
			else {
				searchHover = false;
			}
		}
		else if (engine == 2) {
			//detect hover over search button
			if (ofGetMouseX() > askButton.x && ofGetMouseX() < askButton.x + askButton.getWidth() && ofGetMouseY() > askButton.y && ofGetMouseY() < askButton.y + askButton.getHeight()) {
				searchHover = true;
			}
			else {
				searchHover = false;
			}
		}
	}
	if (showHint) {
		prompt = prompts[2] + query + "'?";
	}
	else {
		prompt = prompts[p %= 2];
	}
}

//--------------------------------------------------------------
void ofApp::draw() {
	//remove older distortions gradually
	maskFbo.begin();
	ofPushStyle();
	ofSetColor(ofColor(0), 2);
	ofDrawRectangle(0, 0, ofGetWidth(), ofGetHeight());
	ofPopStyle();
	maskFbo.end();

	//draw screen into fbo
	screen.begin();
	ofBackground(255);
	if (searched) {
		//draw browser
		ofPushStyle();
		ofSetColor(255);
		browser.draw(0, 0);
		if (browser.getIsLoading()) {
			ofSetColor(0);
			ofDrawBitmapString("Loading...", 20, 20);
		}
		ofPopStyle();
	}
	else {
		//draw search index
		if (engine == 0) { //google
			ofPushStyle();
			ofSetColor(100);
			if (userInput.length() == 0) {
				myfont.drawString(prompt, googleTextBox.x, googleTextBox.y + 28);
			}
			ofPopStyle();

			ofPushStyle();
			ofSetColor(0);
			//display user input
			if (overflow) { //if string is longer than textbox, display only last part by shifting text to left
				myfont.drawString(userInput, googleTextBox.x - (inputWidth - googleTextBox.width), googleTextBox.y + 28);
			}
			else {
				myfont.drawString(userInput, googleTextBox.x, googleTextBox.y + 28);
			}
			ofPopStyle();

			//draw index page
			google.draw(0, 0, google.getWidth(), google.getHeight());

			if (searchHover) {
				//show hover button image
				googleHover.draw(googleButton, googleHover.getWidth(), googleHover.getHeight());
			}

			ofPushStyle();
			ofSetLineWidth(1);
			ofSetColor(30);
			//display text cursor
			if (ofGetFrameNum() % 60 < 30) { //blinking cursor
				//calculate displacement
				int dX, dY;
				if (overflow) {
					//count from the back
					dX = googleTextBox.width - myfont.stringWidth(userInput.substr(textCursor, userInput.length() - textCursor)) + 1;
				}
				else {
					//count from the front
					dX = myfont.stringWidth(userInput.substr(0, textCursor)) + 1;
				}
				dY = myfont.getLineHeight();
				ofDrawLine(googleTextBox.x + dX, googleTextBox.y + 28 + 3, googleTextBox.x + dX, googleTextBox.y + 28 - dY);
			}
			ofPopStyle();
		}
		else if (engine == 1) { //duckduckgo
			//draw index page
			ddg.draw(0, 0, ddg.getWidth(), ddg.getHeight());

			ofPushStyle();
			ofSetColor(100);
			if (userInput.length() == 0) {//display prompt if nothing typed
				myfont.drawString(prompt, ddgTextBox.x, ddgTextBox.y + 20);
			}
			ofPopStyle();

			ofPushStyle();
			ofSetColor(0);
			//display user input
			if (overflow) { //if string is longer than textbox, display only last part by shifting text to left
				myfont.drawString(userInput, ddgTextBox.x - (inputWidth - ddgTextBox.width), ddgTextBox.y + 20);
			}
			else {
				myfont.drawString(userInput, ddgTextBox.x, ddgTextBox.y + 20);
			}
			ofPopStyle();

			if (textTyped) {
				//draw overlay
				ddgOverlay.draw(0, 0, ddgOverlay.getWidth(), ddgOverlay.getHeight());
				if (searchHover) {
					//show hover button image
					ddgHover.draw(ddgButton, ddgHover.getWidth(), ddgHover.getHeight());
				}
			}

			ofPushStyle();
			ofSetLineWidth(1);
			ofSetColor(30);
			//display text cursor
			if (ofGetFrameNum() % 60 < 30) { //blinking cursor
											 //calculate displacement
				int dX, dY;
				if (overflow) {
					//count from the back
					dX = ddgTextBox.width - myfont.stringWidth(userInput.substr(textCursor, userInput.length() - textCursor)) + 1;
				}
				else {
					//count from the front
					dX = myfont.stringWidth(userInput.substr(0, textCursor)) + 1;
				}
				dY = myfont.getLineHeight();
				ofDrawLine(ddgTextBox.x + dX, ddgTextBox.y + 20 + 3, ddgTextBox.x + dX, ddgTextBox.y + 20 - dY);
			}
			ofPopStyle();
		}
		else if (engine == 2) { //ask
			//draw index page
			ask.draw(0, 0, ask.getWidth(), ask.getHeight());

			ofPushStyle();
			ofSetColor(100);
			if (userInput.length() == 0) {//display prompt if nothing typed
				myfont.drawString(prompt, askTextBox.x, askTextBox.y + 22);
			}
			ofPopStyle();

			ofPushStyle();
			ofSetColor(0);
			//display user input
			if (overflow) { //if string is longer than textbox, display only last part by shifting text to left
				myfont.drawString(userInput, askTextBox.x - (inputWidth - askTextBox.width), askTextBox.y + 22);
			}
			else {
				myfont.drawString(userInput, askTextBox.x, askTextBox.y + 22);
			}
			ofPopStyle();

			ofPushStyle();
			ofSetLineWidth(1);
			ofSetColor(30);
			//display text cursor
			if (ofGetFrameNum() % 60 < 30) { //blinking cursor
											 //calculate displacement
				int dX, dY;
				if (overflow) {
					//count from the back
					dX = askTextBox.width - myfont.stringWidth(userInput.substr(textCursor, userInput.length() - textCursor)) + 1;
				}
				else {
					//count from the front
					dX = myfont.stringWidth(userInput.substr(0, textCursor)) + 1;
				}
				dY = myfont.getLineHeight() + 1;
				ofDrawLine(askTextBox.x + dX, askTextBox.y + 22, askTextBox.x + dX, askTextBox.y + 22 - dY);
			}
			ofPopStyle();
		}
	}
	screen.end();

	// update alpha mask
	if (!touchPos.empty()) { //if there are points to be drawn
		drawDistortion();
	}
	// apply shaders
	if (mode == 0) { //blur
		fboOnePass.begin();
		ofClear(0, 0, 0, 0);
		shaderBlurX.begin();
		shaderBlurX.setUniformTexture("maskTex", maskFbo.getTextureReference(), 1);
		screen.draw(0, 0);
		shaderBlurX.end();
		fboOnePass.end();

		fboTwoPass.begin();
		ofClear(0, 0, 0, 0);
		shaderBlurY.begin();
		shaderBlurY.setUniformTexture("maskTex", maskFbo.getTextureReference(), 1);
		fboOnePass.draw(0, 0);
		shaderBlurY.end();
		fboTwoPass.end();
	}
	else if (mode == 1) { //pixelate
		fboTwoPass.begin();
		ofClear(0, 0, 0, 0);
		shaderPixelate.begin();
		shaderPixelate.setUniform1i("pixelSize", pixelSize);
		shaderPixelate.setUniformTexture("maskTex", maskFbo.getTextureReference(), 1);
		screen.draw(0, 0);
		shaderPixelate.end();
		fboTwoPass.end();
	}
	fboTwoPass.draw(0, 0); //final output
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == OF_KEY_RETURN) {
		//user pressed enter key
		//submit search query
		if (!searched) {
			search();
		}
	}
	else if (key >= 32 && key <= 126) {
		userInput += (char)key;
		textCursor++;
	}
	else if (key == OF_KEY_BACKSPACE && textCursor > 0 && userInput.length() > 0) {
		userInput.pop_back();
		textCursor--;
	}
	else if (key == OF_KEY_LEFT && textCursor > 0) {
		textCursor--;
	}
	else if (key == OF_KEY_RIGHT && textCursor < (int)userInput.length()) {
		textCursor++;
	}
	/*else if (key == OF_KEY_DOWN) {
		mode++;
		mode %= 2;
		maskFbo.begin();
		ofClear(0, 0, 0, 255);
		maskFbo.end();
	}*/
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {


}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	if (button == 2) {
		touchPos.push_back(ofPoint(x, y));
	} 
	if (searched && !browser.getIsLoading()) {
		browser.mouseDragged(x, y, button);
	}
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	/*if (button == 2 && x < 5) {
		fullscreen = !fullscreen;
		ofSetFullscreen(fullscreen);
	}*/
	if (searched && !browser.getIsLoading()) {
		browser.mousePressed(x, y, button);
	}
	else if (searchHover && button == 0) {
		//user clicked search button
		//submit search query
		if (!searched) {
			search();
		}
	}
	cout << x << "," << y << endl;
	/*if (button == 2) {
		touchPos.push_back(ofPoint(x, y));
	}*/
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	if (searched && !browser.getIsLoading()) {
		browser.mouseReleased(x, y, button);
	}
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {
	browser.setup(ofGetWidth(), ofGetHeight());
}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

string ofApp::pad(int v) {
	string value = to_string(v);
	while (value.length() < 3) {
		value = " " + value;
	}
	return value;
}

void ofApp::processMsg(string msg) {
	if (msg == "blur") {
		mode = 0;
		return;
	}
	else if (msg == "pixelate") {
		mode = 1;
		return;
	}

	vector<string> data = ofSplitString(msg, ",");
	if (data.size() % 2 == 0) {
		for (int i = 0; i < data.size(); i += 2) {
			touchPos.push_back(ofPoint(ofToInt(data[i]), ofToInt(data[i + 1])));
		}
	}
}

/*
	Submits query request to browser
*/
void ofApp::search() {
	if (userInput.length() > 0) {
		if (engine == 0 || (engine > 0 && (userInput == query || userInput == "yes" || userInput == "Yes"))){ //same query as previous search
			if (!(userInput == "yes" || userInput == "Yes")) {
				query = userInput;
			}
			userInput.clear();
			//parse string into url query by replacing all spaces
			std::regex space("[[:space:]]");
			string urlQuery = std::regex_replace(query, space, "%20");
			//get html file of web page
			if (engine == 0) {
				browser.loadURL("https://www.google.co.uk/search?q=" + urlQuery);
			}
			else if (engine == 1) {
				browser.loadURL("https://duckduckgo.com/?q=" + urlQuery);
			}
			else if (engine == 2) {
				browser.loadURL("https://uk.ask.com/web?q=" + urlQuery);
			}
			if (engine < 2) {
				showHint = true;
			}
			else {
				showHint = false;
			}
			engine++;
			engine %= 3;
			sinceSearch = 0;
			if (engine == 2) {
				p = 0;
			}
			searched = true;
			return;
		}
	}
	userInput.clear();
	p++;
	p %= 2;
}

/*
	draws into mask fbo
	to be called only in draw()
*/
void ofApp::drawDistortion() {
	maskFbo.begin();
	for (int i = 0; i < touchPos.size(); i++) {
		if (mode == 0) { //blur
			int brushImageSize = 50;
			int brushImageX = touchPos[i].x - brushImageSize * 0.5;
			int brushImageY = touchPos[i].y - brushImageSize * 0.5;
			imageMask.draw(brushImageX, brushImageY, brushImageSize, brushImageSize);
		}
		else if (mode == 1) { //pixelate
			int halfPix;
			int drawPosX = floor(touchPos[i].x / pixelSize)*pixelSize;
			int drawPosY = floor(touchPos[i].y / pixelSize)*pixelSize;
			if (pixelSize % 2 == 0) {
				halfPix = (pixelSize * 3 / 2);
			}
			else {
				halfPix = ceil(pixelSize * 3 / 2);
			}
			for (int X = 0; X < 3; X++) { //x-columns
				for (int Y = 0; Y < 3; Y++) { //y-columns
					ofSetColor(255); //assign 'pixel' area through colours
					ofDrawRectangle(drawPosX + pixelSize*(X - 1), drawPosY + pixelSize*(Y - 1), pixelSize, pixelSize);
				}
			}
		}
	}
	//delete inside textbox
	ofPushStyle();
	ofSetColor(0);
	ofDrawRectangle(textBoxes[engine].x, textBoxes[engine].y, textBoxes[engine].width, textBoxes[engine].height);
	ofPopStyle();
	maskFbo.end();
	touchPos.clear();
}