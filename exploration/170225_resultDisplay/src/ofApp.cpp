#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofBackground(0);
}

//--------------------------------------------------------------
void ofApp::update() {
	//string query;
	//getline(cin, query); //get user input
	//for (int i = 0; i < query.length(); i++) { //replace spaces with %20
	//	if (query[i] == ' ') {
	//		query.replace(i, 1,"%20");
	//	}
	//}
	//ofHttpResponse resp = ofLoadURL("https://www.google.co.uk/search?q="+query);
	//cout << resp.data << endl;
}

//--------------------------------------------------------------
void ofApp::draw() {
	ofSetColor(255);
	ofPushMatrix();
	ofTranslate(5, 10);

	//draw user input
	ofDrawBitmapString(text, 10, 10);

	//draw blinking cursor line
	ofPushStyle();
	float timeFrac = 0.5 * sin(3.0f * ofGetElapsedTimef()) + 0.5;

	ofColor col = ofGetStyle().color;

	ofSetColor(col.r * timeFrac, col.g * timeFrac, col.b * timeFrac);
	ofSetLineWidth(3.0f);
	ofLine(position * 8 + 10, 13.7*cursory, position * 8 + 10, 10 + 13.7*cursory);
	ofPopStyle();

	//display webpage html
	if (queryRetreived) {
		int i = 119;
		string results = resp.data.getText();
		while (i < resp.data.size()) {
			results.insert(i, "\n");
			i += 120;
		}
		ofDrawBitmapString(results, 10, 50);
	}
	ofPopMatrix();
}

////--------------------------------------------------------------
//void ofApp::addText(string &s) {
//	textEntries.push_back(s);
//}
////--------------------------------------------------------------
//void ofApp::drawEntries() {
//	for (int i = 0; i < textEntries.size(); ++i) {
//		ofDrawBitmapString(textEntries[i], 50, 100 + 10 * i);
//	}
//}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (typingInput) {
		//add charachter (non unicode sorry!)
		if (key >= 32 && key <= 126) {
			text.insert(text.begin() + position, key);
			position++;
		}
		if (key == OF_KEY_RETURN) {
			input = text;
			text.clear();
			position = 0;
			sendQuery(input);
		}
		if (key == OF_KEY_BACKSPACE) {
			if (position > 0) {
				text.erase(text.begin() + position - 1);
				--position;
			}
		}

		if (key == OF_KEY_DEL) {
			if (text.size() > position) {
				text.erase(text.begin() + position);
			}
		}

		if (key == OF_KEY_LEFT)
			if (position > 0)
				--position;

		if (key == OF_KEY_RIGHT)
			if (position < text.size() + 1)
				++position;
	}
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {
	//check if clicked on input textbox
	typingInput = true;
}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}

void ofApp::sendQuery(string Q) {
	//string query = translateQuery(Q);
	resp = ofLoadURL("https://www.google.co.uk/search?q=" + Q);
	queryRetreived = true;
}

string ofApp::translateQuery(string Q) {
	for (int i = 0; i < Q.length(); i++) { //replace spaces with %20
		if (Q[i] == ' ') {
			Q.replace(i, 1, "%20");
		}
		//',''.''/'':'';'''''"''['']''{''}''<''>''?'!''@''#''$''%''^'...etc.
	}

	return Q;
}

//find: <div data-async-context = "query:hi" id = "ires"> for column of results
//results: <div class="g"...> -> <h3 class="r"...> -> <a href="[url]" ...> -> [title of webpage]