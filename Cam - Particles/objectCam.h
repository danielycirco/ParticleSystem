#pragma once

#include "ofMain.h"

class objectCam{

public:
	void setup();
	void update();

	int getPosBallX();
	int getPosBallY();

	ofVideoGrabber vidGrabber;
	int camWidth;
	int camHeight;

};

