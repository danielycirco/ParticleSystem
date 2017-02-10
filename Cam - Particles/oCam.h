#pragma once

#include "ofMain.h"

class oCam {

public:
	void setup();
	void update();

	int getPosBallX();
	int getPosBallY();

	ofVideoGrabber vidGrabber;
	int camWidth;
	int camHeight;

};
