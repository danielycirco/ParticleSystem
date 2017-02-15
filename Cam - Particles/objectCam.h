#pragma once

#include "ofMain.h"
#include "ofxCvColorImage.h"
#include "ofxCvContourFinder.h"

class objectCam {

public:
	void setup();
	void update();
	void posBall();

	ofVideoGrabber vidGrabber;
	ofxCvContourFinder contour;
	ofxCvGrayscaleImage grayImage;
	ofxCvColorImage image;


	int camWidth;
	int camHeight;

	ofPoint pos[5];
	int nbBallDetected;
};

