#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofSetVerticalSync(true);
	
	int num = 20;
	p.assign(num, demoParticle());
	currentMode = PARTICLE_MODE_ATTRACT;

	currentModeStr = "1 - PARTICLE_MODE_ATTRACT: attracts to mouse"; 

	resetParticles();

	camera.setup();
}

//--------------------------------------------------------------
void ofApp::resetParticles(){
	//Words
	textCounter = 0;

	//Sistema 3
	if (currentMode == PARTICLE_MODE_NEAREST_POINTS) {
		attractPoints.clear();

		for (int i = 0; i < 3; i++) {
			attractPoints.push_back(ofPoint(ofMap(i, 0, 3, 100, ofGetWidth() - 100), ofRandom(100, ofGetHeight() - 100)));
		}
		///////// Atracción a lineas del triangulo
		for (int i = 0; i < 3; i++) {
			ofPoint dx;
			if (i == 2) dx = (attractPoints[0] - attractPoints[i]) / 20;
			else dx = (attractPoints[i + 1] - attractPoints[i]) / 20;
			for (int j = 0; j < 20; j++) pLine[j + i * 20].pos = attractPoints[i] + dx*j;
		}
		for (int i = 0; i < 60; i++) attractPoints.push_back(pLine[i].pos);

	}

	for(int i = 0; i < 4; i++){
		attractPoints.push_back( ofPoint( ofMap(i, 0, 4, 100, ofGetWidth()-100) , ofRandom(100, ofGetHeight()-100) ) );
	}
	
	for(unsigned int i = 0; i < p.size(); i++){
		p[i].setMode(currentMode);		
		p[i].setAttractPoints(&attractPoints);;
		p[i].reset();
	}	
}

//--------------------------------------------------------------
void ofApp::update(){
	// Cam
	int posBallX = camera.getPosBallX();
	int posBallY = camera.getPosBallY();
	camera.update();

	//Sistema 3
	if (currentMode == PARTICLE_MODE_NEAREST_POINTS) {
		for (unsigned int i = 0; i < attractPoints.size(); i++) {
			attractPoints[i].x = attractPoints[i].x + ofSignedNoise(i * 10, ofGetElapsedTimef() * 1.7) * 1.0;
			attractPoints[i].y = attractPoints[i].y + ofSignedNoise(i * -10, ofGetElapsedTimef() * 0.7) * 1.0;
		}
		///////// Atracción a lineas del triangulo
		for (int i = 0; i < 3; i++) {
			ofPoint dx;
			if (i == 2) dx = (attractPoints[0] - attractPoints[i]) / 20;
			else dx = (attractPoints[i + 1] - attractPoints[i]) / 20;
			for (int j = 0; j < 20; j++) pLine[j + i * 20].pos = attractPoints[i] + dx*j;
		}
		for (int i = 3; i < 60; i++) attractPoints[i] = pLine[i].pos;
	}

	for(unsigned int i = 0; i < p.size(); i++){
		p[i].setMode(currentMode);
		p[i].newPosition(posBallX, posBallY);
		p[i].update();
	}
	
}

//--------------------------------------------------------------
void ofApp::draw(){
	ofBackground(0,0,0);
	//ofBackgroundGradient(ofColor(60,60,60), ofColor(10,10,10), OF_GRADIENT_LINEAR);

	// cam
	//camera.vidGrabber.draw(0, 0);

	for(unsigned int i = 0; i < p.size(); i++){
		p[i].draw();
	}
	
	//Sistema 3
	ofSetColor(190);
	if (currentMode == PARTICLE_MODE_NEAREST_POINTS) {
		ofNoFill();
		ofDrawTriangle(attractPoints[0].x, attractPoints[0].y, attractPoints[1].x, attractPoints[1].y, attractPoints[2].x, attractPoints[2].y);
		ofFill();
		for (unsigned int i = 0; i < 3; i++) {
			ofNoFill();
			ofDrawCircle(attractPoints[i], 20);
			ofFill();
			ofDrawCircle(attractPoints[i], 4);
		}
		ofSetColor(255, 255, 255);
		for (unsigned int i = 0; i < pLine.size(); i++) {
			ofDrawCircle(pLine[i].pos, 4);
		}

	}

	ofSetColor(230);	
	ofDrawBitmapString(currentModeStr + "\n\nSpacebar to reset. \nKeys 1-4 to change mode. \nFramerate: " + ofToString(ofGetFrameRate(), 0), 10, 20);
	
	// Object identificator
	ofNoFill();
	ofDrawCircle(camera.getPosBallX(),  camera.getPosBallY(), 10);
	ofFill();
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == '1') {
		currentMode = PARTICLE_MODE_ATTRACT;
		currentModeStr = "1 - PARTICLE_MODE_ATTRACT: attracts to mouse";
		resetParticles();
	}
	if (key == '2') {
		currentMode = PARTICLE_MODE_REPEL;
		currentModeStr = "2 - PARTICLE_MODE_REPEL: repels from mouse";
		resetParticles();
	}
	if (key == '3') {
		currentMode = PARTICLE_MODE_NEAREST_POINTS;
		currentModeStr = "3 - PARTICLE_MODE_NEAREST_POINTS: hold 'f' to disable force";
		resetParticles();
	}
	if (key == '4') {
		currentMode = PARTICLE_MODE_SNOW;
		currentModeStr = "4 - PARTICLE_MODE_SNOW: snow particle simulation";
		resetParticles();
	}
	if (key == '5') {
		currentMode = PARTICLE_MODE_DUST;
		currentModeStr = "5 - PARTICLE_MODE_DUST: Waves";
		resetParticles();
	}
	if (key == '6') {
		currentMode = PARTICLE_MODE_GRILL;
		currentModeStr = "6 - PARTICLE_MODE_GRILL: Grill";
		resetParticles();
	}
	if (key == '7') {
		currentMode = PARTICLE_MODE_LIFE;
		currentModeStr = "7 - PARTICLE_MODE_LIFE: Lifetime";
		resetParticles();
	}

	if (key == 'f') ofToggleFullscreen();

	if( key == ' ' )resetParticles();
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}