#pragma once
#include "ofMain.h"
#include <list>

using namespace std;

class pixelTransform {
public:
	pixelTransform() {};
	~pixelTransform() {};

	void scale(float amountToScale);
	void transform(float amountToTransf);

private:
	//list<float> scale;
	//list<float> translate;
};