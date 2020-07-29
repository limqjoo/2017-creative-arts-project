#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofLoadImage(p, "image.jpg");
	pOut.allocate(p.getWidth(), p.getHeight(), 3);
	texture.allocate(p.getWidth(), p.getHeight(), GL_RGB);
	p.pasteInto(pOut, 0, 0);
}

//--------------------------------------------------------------
void ofApp::update() {
	texture.loadData(pOut);
}

//--------------------------------------------------------------
void ofApp::draw() {
	texture.draw(0, 0);
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {
	//pixelate:
	//pixelate(25);

	//smudge (uncomment for mousePressed() also):
	smudgeUpdate();

	//blur (uncomment for mousePressed() also):
	//blurUpdate();
}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {
	//smudge:
	smudgeStart();

	//blur:
	//blurStart();
}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

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

/*
Pixelate:

[ ][ ][ ]
[ ][o][ ]		[ ] - represents (pixelSize^2) no. of pixels
[ ][ ][ ]

ideally pixelSize will be an odd number such that mouseX(x) and mouseY(y) can be in the center of [o]:

(x-1, y-1) (x, y-1) (x+1, y-1)
(x-1, y)	(x, y)	  (x+1, y)		-> if pixelSize = 3;
(x-1, y+1) (x, y+1) (x+1, y+1)

an average of all colours of the pixels within one [ ] will be the new colour set within that [ ].
*/
void ofApp::pixelate(int pixelSize) {
	//round mouse position such that pixelation is only at positions that are multiples of pixelSize
	int mouseX = ceil(ofGetMouseX() / pixelSize)*pixelSize;
	int mouseY = ceil(ofGetMouseY() / pixelSize)*pixelSize;
	//determine if pixelSize is odd, set width of half the pixelSize int 0.5 larger since there cannot be float for x, y position
	int halfPix = 0;
	if (pixelSize % 2 == 0) {
		halfPix = (pixelSize * 3 / 2);
	}
	else {
		halfPix = ceil(pixelSize * 3 / 2);
	}
	for (int X = 0; X < 3; X++) { //x-columns
		for (int Y = 0; Y < 3; Y++) { //y-columns
			//get average colour of all pixels in 
			float R = 0, G = 0, B = 0;
			for (int x = mouseX - halfPix + X * pixelSize; x < mouseX - halfPix + (X + 1) * pixelSize; x++) { //x-pixels
				for (int y = mouseY - halfPix + Y * pixelSize; y < mouseY - halfPix + (Y + 1) * pixelSize; y++) { //y-pixels
					if (y*ofGetWidth() + x >= 0) {
						R += pOut.getColor(x, y).r;
						G += pOut.getColor(x, y).g;
						B += pOut.getColor(x, y).b;
					}
				}
			}
			R /= pow(pixelSize, 2);
			G /= pow(pixelSize, 2);
			B /= pow(pixelSize, 2);
			ofColor avgColour = ofColor(R, G, B);
			for (int x = mouseX - 8 + X * pixelSize; x < mouseX - 8 + (X + 1) * pixelSize; x++) { //x-pixels
				for (int y = mouseY - 8 + Y * pixelSize; y < mouseY - 8 + (Y + 1) * pixelSize; y++) { //y-pixels
					if (x >= 0 && x < ofGetWidth() && y >= 0 && y < ofGetHeight()) { //check for valid access to array
						pOut.setColor(x, y, avgColour);
					}
				}
			}
		}
	}
}

/*
BLUR
*/
void ofApp::blurStart() {
	blurImg.allocate(pOut.getWidth(), pOut.getHeight(), 3); //get the image at the time blur starts
	pOut.pasteInto(blurImg, 0, 0);
}

void ofApp::blurUpdate(int radius) {
	r = radius;
	int mouseX = ofGetMouseX();
	int mouseY = ofGetMouseY();
	for (int y = mouseY - r; y <= mouseY + r; y++) {
		for (int x = mouseX - r; x <= mouseX + r; x++) {
			//if distance between pixel at (x,y) and pixel at (mouseX, mouseY) < r, modify colour of pixel
			if (pow(pow(x - mouseX, 2) + pow(y - mouseY, 2), 0.5) <= r) {
				double newR = 0;
				double newG = 0;
				double newB = 0;
				double k = 0.04;
				//average the pixel's and the surrounding pixels' colour
				for (int i = 0; i < size(blurMatrix); i++) {
					ofColor c = blurImg.getColor(x - 2 + (i % 5), y + ofMap(i, 0, 9, -2, 2));
					newR += c.r*k;
					newG += c.g*k;
					newB += c.b*k;
				}
				if (x >= 0 && x < ofGetWidth() && y >= 0 && y < ofGetHeight()) { //check for valid access to array
					pOut.setColor(x, y, ofColor(ceil(newR), ceil(newG), ceil(newB)));
				}
			}
		}
	}
}

/*
Smudge:
		  . . . . .
	  . . . . . . . . .
	. . . . . . . . . . .
  . . . . . . .	. . . . . .			. - represents one pixel, whereas ; is the center and has co-ordinates (mouseX, mouseY)
  . . . . . . .	. . . . . .
. . . . . . . . . . . . . . .
. . . . . . . . . . . . . . .
. . . . . . . ; . . . . . . .
. . . . . . . . . . . . . . .
. . . . . . . . . . . . . . .
  . . . . . . .	. . . . . .
  . . . . . . . . . . . . .
	. . . . . . . . . . .
		. . . . . . .

original colour of each pixel is added to the pixel of the next mouse position (i.e. pow((mouseX-prevMouseX)+(mouseY-prevMouseY), 1/2))
the higher the smudgeStrength, more of the original colour is retained (i.e. ratio of original colour to the colour of the pixel it is added to is higher)
*/
void ofApp::smudgeStart(int radius, float strength) {
	r = radius;
	int mouseX = ofGetMouseX();
	int mouseY = ofGetMouseY();
	smudgeStrength = strength;
	//get colours of starting area of smudge
	smudgeColours.clear();
	for (int y = mouseY - r; y <= mouseY + r; y++) {
		for (int x = mouseX - r; x <= mouseX + r; x++) {
			//if distance between pixel at (x,y) and pixel at (mouseX, mouseY) < r, save pixel's colour in array
			if (pow(pow(x - mouseX, 2) + pow(y - mouseY, 2), 0.5) <= r) {
				smudgeColours.push_back(pOut.getColor(x, y)); //get colour
			}
		}
	}
}

bool ofApp::smudgeUpdate(float resistance)
{
	if (smudgeStrength >= 1 || smudgeStrength < 0 || resistance > 0.1) {
		return false;
	}
	int mouseX = ofGetMouseX();
	int mouseY = ofGetMouseY();
	int prevMouseX = ofGetPreviousMouseX();
	int prevMouseY = ofGetPreviousMouseY();
	//add colours of starting area of smudge to current cursor area
	int i = 0; //index to access smudgeColours
	for (int y = mouseY - r; y <= mouseY + r; y++) {
		for (int x = mouseX - r; x <= mouseX + r; x++) {
			if (pow(pow(x - mouseX, 2) + pow(y - mouseY, 2), 0.5) <= r) {
				if (x >= 0 && x < ofGetWidth() && y >= 0 && y < ofGetHeight()) { //check for valid access to array
					ofColor newC = pOut.getColor(x, y)*(1 - smudgeStrength) + smudgeColours[i] * smudgeStrength;
					pOut.setColor(x, y, newC); //set new color
					smudgeColours[i] = newC;
				}
				i++;
			}
		}
	}
	smudgeStrength -= resistance;
	return true;
}