#include "canvas.h"

void Canvas::setup(ofPoint a, ofPoint b) { //called once in setup
	output.allocate(b.x, b.y, 3);
	A = a;
	B = b;
}

void Canvas::getFrame() {
	currentFrame = screenGrab(A, B);
	currentFrame.pasteInto(output, 0, 0);
}

void Canvas::start(ofPixels img) //called once when mouse pressed
{
	//getFrame();
	img.pasteInto(output, 0, 0);
	if (mode == 1) {
		blurStart();
	}
	else if (mode == 2) {
		smudgeStart();
	}
}

ofPixels Canvas::update(ofPixels img) //called while mouse dragged
{
	img.pasteInto(output, 0, 0);
	if (mode == 0) {
		pixelate();
	}
	else if (mode == 1) {
		blurUpdate();
	}
	else if (mode == 2) {
		smudgeUpdate();
	}
	return output;
}

void Canvas::draw()
{

}

void Canvas::switchMode()
{
	mode++;
	mode %= numModes;
}

bool Canvas::switchMode(int newMode)
{
	if (newMode < numModes) {
		mode = newMode;
		return true;
	}
	return false;
}

void Canvas::pixelate(int pixelSize) {
	int mouseX = ceil(ofGetMouseX() / pixelSize)*pixelSize;
	int mouseY = ceil(ofGetMouseY() / pixelSize)*pixelSize;
	int halfPix = 0;
	if (pixelSize % 2 == 0) {
		halfPix = (pixelSize * 3 / 2);
	}
	else {
		halfPix = ceil(pixelSize * 3 / 2);
	}
	for (int X = 0; X < 3; X++) { //x-columns
		for (int Y = 0; Y < 3; Y++) { //y-columns
			float R = 0, G = 0, B = 0;
			for (int x = mouseX - halfPix + X * pixelSize; x < mouseX - halfPix + (X + 1) * pixelSize; x++) { //x-pixels
				for (int y = mouseY - halfPix + Y * pixelSize; y < mouseY - halfPix + (Y + 1) * pixelSize; y++) { //y-pixels
					if (y*ofGetWidth() + x >= 0) {
						R += output.getColor(x, y).r;
						G += output.getColor(x, y).g;
						B += output.getColor(x, y).b;
					}
				}
			}
			//int v = (int)rand() % 15 - 7; //add some variant to shade
			R /= pow(pixelSize, 2);
			G /= pow(pixelSize, 2);
			B /= pow(pixelSize, 2);
			ofColor avgColour = ofColor(R, G, B);
			for (int x = mouseX - 8 + X * pixelSize; x < mouseX - 8 + (X + 1) * pixelSize; x++) { //x-pixels
				for (int y = mouseY - 8 + Y * pixelSize; y < mouseY - 8 + (Y + 1) * pixelSize; y++) { //y-pixels
					if (x >= 0 && x < ofGetWidth() && y >= 0 && y < ofGetHeight()) { //check for valid access to array
						output.setColor(x, y, avgColour);
					}
				}
			}
		}
	}
}

void Canvas::blurStart() {
	blurImg.allocate(output.getWidth(), output.getHeight(), 3);
	currentFrame.pasteInto(blurImg, 0, 0);
}

void Canvas::blurUpdate(int radius) {
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
				double k = 0.003775;
				for (int i = 0; i < size(blurMatrix); i++) {
					ofColor c = blurImg.getColor(x - 2 + (i % 5), y + ofMap(i, 0, 9, -2, 2));
					newR += c.r*0.04;
					newG += c.g*0.04;
					newB += c.b*0.04;
					//newB += c.b*blurMatrix[i];
				}
				output.setColor(x, y, ofColor(ceil(newR), ceil(newG), ceil(newB)));
			}
		}
	}
}


void Canvas::smudgeStart(int radius, float strength) {
	r = radius;
	int mouseX = ofGetMouseX();
	int mouseY = ofGetMouseY();
	smudgeStrength = strength;
	//get colours of starting area of smudge
	for (int y = mouseY - r; y <= mouseY + r; y++) {
		for (int x = mouseX - r; x <= mouseX + r; x++) {
			//if distance between pixel at (x,y) and pixel at (mouseX, mouseY) < r, save pixel's colour in array
			if (pow(pow(x - mouseX, 2) + pow(y - mouseY, 2), 0.5) <= r) {
				smudgeColours.push_back(output.getColor(x, y)); //get colour
			}
		}
	}
}

bool Canvas::smudgeUpdate(float resistance)
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
				ofColor newC = output.getColor(x, y)*(1 - smudgeStrength) + smudgeColours[i] * smudgeStrength;
				output.setColor(x, y, newC); //set new color
				smudgeColours[i] = newC;
				i++;
			}
		}
	}
	smudgeStrength -= resistance;
	return true;
}
