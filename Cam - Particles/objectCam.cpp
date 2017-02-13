#include "objectCam.h"

void objectCam::setup() {
	camWidth = 1024;  // try to grab at this size.
	camHeight = 768;

	vector<ofVideoDevice> devices = vidGrabber.listDevices();

	for (int i = 0; i < devices.size(); i++) {
		if (devices[i].bAvailable) {
			ofLogNotice() << devices[i].id << ": " << devices[i].deviceName;
		}
		else {
			ofLogNotice() << devices[i].id << ": " << devices[i].deviceName << " - unavailable ";
		}
	}

	vidGrabber.setDeviceID(0);
	vidGrabber.setDesiredFrameRate(60);
	vidGrabber.initGrabber(camWidth, camHeight);
}

void objectCam::update() {
	// Update cam
	vidGrabber.update();
}


int objectCam::getPosBallX() {
	ofPixels & pixels = vidGrabber.getPixels();
	int greenX = 0;
	int maxGreen = 0;
	for (int i = 0; i < pixels.size() / 3; i++) {
		if (3 * pixels[3 * i + 1] - pixels[3 * i] - pixels[3 * i + 2]>maxGreen) {
			maxGreen = 3 * pixels[3 * i + 1] - pixels[3 * i] - pixels[3 * i + 2];
			greenX = i % camWidth;
		}
	}
	return greenX;
}

int objectCam::getPosBallY() {
	ofPixels & pixels = vidGrabber.getPixels();
	int greenY = 0;
	int maxGreen = 0;
	for (int i = 0; i < pixels.size() / 3; i++) {
		if (3 * pixels[3 * i + 1] - pixels[3 * i] - pixels[3 * i + 2]>maxGreen) {
			maxGreen = 3 * pixels[3 * i + 1] - pixels[3 * i] - pixels[3 * i + 2];
			greenY = i / camWidth;
		}
	}
	return greenY;
}
