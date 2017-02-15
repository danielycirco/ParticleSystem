#include "objectCam.h"

void objectCam::setup() {
	camWidth = 320;  // try to grab at this size.
	camHeight = 240;

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
	for (int i = 0; i < 5; i++)
	{
		pos[i] = { 0,0,0 };
	}
	nbBallDetected = 0;

}

void objectCam::update() {
	// Update cam
	vidGrabber.update();
}



void objectCam::posBall() {

	ofPixels &	pixels = vidGrabber.getPixels();

	image = pixels;
	image.blurGaussian(15);
	image.convertRgbToHsv();

	ofPixels & pixels_im = image.getPixels();


	//Limites des valeurs de HSV pour détecter le vert
	int hmin = 20;
	int hmax = 100;
	int smin = 40;
	int smax = 255;
	int vmin = 6;
	int vmax = 255;

	for (int i = 0; i<camWidth*camHeight - 2; i++)
	{
		if (ofInRange(pixels_im[3 * i], hmin, hmax)&ofInRange(pixels_im[3 * i + 1], smin, smax)&ofInRange(pixels_im[3 * i + 2], vmin, vmax))
		{
			pixels_im[3 * i] = 255;
			pixels_im[3 * i + 1] = 255;
			pixels_im[3 * i + 2] = 255;
		}
		else
		{
			pixels_im[3 * i] = 0;
			pixels_im[3 * i + 1] = 0;
			pixels_im[3 * i + 2] = 0;
		}
	}

	image = pixels_im;

	for (int i = 0; i < 6; i++) {
		image.erode();
	}
	for (int i = 0; i < 6; i++) {
		image.dilate();
	}

	image = pixels_im;

	image.convertToGrayscalePlanarImage(grayImage, 0);
	contour.findContours(grayImage, 100, 30000, 10, false, true);

	nbBallDetected = contour.nBlobs;

	if (nbBallDetected > 0)
	{
		for (int i = 0; i < nbBallDetected; i++)
		{
			pos[i] = contour.blobs[i].centroid;
		}
	}
}