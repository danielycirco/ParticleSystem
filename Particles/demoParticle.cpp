#include "demoParticle.h"

//------------------------------------------------------------------
demoParticle::demoParticle(){
	attractPoints = NULL;
}

//------------------------------------------------------------------
void demoParticle::setMode(particleMode newMode){
	mode = newMode;
}

//------------------------------------------------------------------
void demoParticle::setAttractPoints( vector <ofPoint> * attract ){ //4 points adress?????????????????????????????
	attractPoints = attract;
}

//------------------------------------------------------------------
void demoParticle::reset(){
	//the unique val allows us to set properties slightly differently for each particle
	uniqueVal = ofRandom(-10000, 10000);
	
	pos.x = ofRandomWidth();
	pos.y = ofRandomHeight();
	
	vel.x = ofRandom(-3.9, 3.9);
	vel.y = ofRandom(-3.9, 3.9);
	
	frc   = ofPoint(0,0,0);
	
	scale = ofRandom(0.5, 2.0);

	touche = false;
	
	if( mode == PARTICLE_MODE_NOISE ){
		drag  = ofRandom(0.97, 0.99);
		vel.y = fabs(vel.y) * 3.0; //make the particles all be going down
	}else{
		drag  = ofRandom(0.985, 0.998);	
	}
}

//------------------------------------------------------------------
void demoParticle::update(){

	//1 - APPLY THE FORCES BASED ON WHICH MODE WE ARE IN 
	
	if (mode == PARTICLE_MODE_ATTRACT) {
		ofPoint attractPt(ofGetMouseX(), ofGetMouseY());
		frc = attractPt - pos; // we get the attraction force/vector by looking at the mouse pos relative to our pos
		frc.normalize(); //by normalizing we disregard how close the particle is to the attraction point 

		if (ofGetMousePressed()){
			vel *= drag; //apply drag
			vel += frc * 0.6; //apply force
		}else {
			vel *= 0.99;
		}
	}
	else if( mode == PARTICLE_MODE_REPEL ){
		ofPoint attractPt(ofGetMouseX(), ofGetMouseY());
		frc = attractPt-pos; 
		
		//let get the distance and only repel points close to the mouse
		float dist = frc.length();
		frc.normalize(); 
		
		vel *= drag;
		if( dist < 150 ){
			vel += -frc * 0.6; //notice the frc is negative 
		}else{
			//if the particles are not close to us, lets add a little bit of random movement using noise. this is where uniqueVal comes in handy. 			
			frc.x = ofSignedNoise(uniqueVal, pos.y * 0.01, ofGetElapsedTimef()*0.2);
			frc.y = ofSignedNoise(uniqueVal, pos.x * 0.01, ofGetElapsedTimef()*0.2);
			vel += frc * 0.04;
		}
	}
	else if( mode == PARTICLE_MODE_NOISE ){
		//lets simulate falling snow 
		//the fake wind is meant to add a shift to the particles based on where in x they are
		//we add pos.y as an arg so to prevent obvious vertical banding around x values - try removing the pos.y * 0.006 to see the banding
		float fakeWindX = ofSignedNoise(pos.x * 0.003, pos.y * 0.006, ofGetElapsedTimef() * 0.6);
		
		frc.x = fakeWindX * 0.25 + ofSignedNoise(uniqueVal, pos.y * 0.04) * 0.6;
		frc.y = ofSignedNoise(uniqueVal, pos.x * 0.006, ofGetElapsedTimef()*0.2) * 0.09 + 0.18;

		vel *= drag; 
		vel += frc * 0.4;
		
		//we do this so as to skip the bounds check for the bottom and make the particles go back to the top of the screen
		if( pos.y + vel.y > ofGetHeight() ){
			pos.y -= ofGetHeight();
		}
	}
	else if( mode == PARTICLE_MODE_NEAREST_POINTS ){
		
		if( attractPoints ){

			//1 - find closest attractPoint 
			ofPoint closestPt;
			int closest = -1; 
			float closestDist = 9999999;
			
			for(unsigned int i = 0; i < attractPoints->size(); i++){
				float lenSq = ( attractPoints->at(i)-pos ).lengthSquared();
				if( lenSq < closestDist ){
					closestDist = lenSq;
					closest = i;
				}
			}
			
			//2 - if we have a closest point - lets calcuate the force towards it
			if( closest != -1 ){
				closestPt = attractPoints->at(closest);				
				float dist = sqrt(closestDist);
				
				//in this case we don't normalize as we want to have the force proportional to distance 
				frc = closestPt - pos;
		
				vel *= drag;
				 
				//lets also limit our attraction to a certain distance and don't apply if 'f' key is pressed
				if( dist < 300 && dist > 40 && !ofGetKeyPressed('f') ){
					vel += frc * 0.003;
				}else{
					//if the particles are not close to us, lets add a little bit of random movement using noise. this is where uniqueVal comes in handy. 			
					frc.x = ofSignedNoise(uniqueVal, pos.y * 0.01, ofGetElapsedTimef()*0.2);
					frc.y = ofSignedNoise(uniqueVal, pos.x * 0.01, ofGetElapsedTimef()*0.2);
					vel += frc * 0.4;
				}
				
			}
		
		}
		
	}
	else if (mode == PARTICLE_MODE_SNOW) {
		float fakeWindX = ofSignedNoise(pos.x * 0.003, pos.y * 0.006, ofGetElapsedTimef() * 0.6);
		
		ofPoint actualPt(ofGetMouseX(), ofGetMouseY());
		ofPoint previousPt(ofGetPreviousMouseX(), ofGetPreviousMouseY());
				
		ofPoint frc1 = actualPt - previousPt;
		ofPoint	frc2 = actualPt - pos; // we get the attraction force/vector by looking at the mouse pos relative to our pos
		
									   //let get the distance and only repel points close to the mouse
		float dist = frc1.length();
		frc1.normalize();
		frc2.normalize(); //by normalizing we disregard how close the particle is to the attraction point 
		
		frc = frc1*0.2 + frc2*0.8;

		vel *= drag;
		if (dist > 10) {
			vel += frc * 0.4; //notice the frc is negative 
		}
		else {
			//if the particles are not close to us, lets add a little bit of random movement using noise. this is where uniqueVal comes in handy. 			
			frc.x = fakeWindX * 0.25 + ofSignedNoise(uniqueVal, pos.y * 0.04) * 0.6;
			frc.y = ofSignedNoise(uniqueVal, pos.x * 0.006, ofGetElapsedTimef()*0.2) * 0.09 + 0.28;

			vel *= drag;
			vel += frc * 0.4;

			//we do this so as to skip the bounds check for the bottom and make the particles go back to the top of the screen
			/*if (pos.y + vel.y > ofGetHeight()) {
				pos.y -= ofGetHeight();
			}*/
		}
	}
	if (mode == PARTICLE_MODE_GRILL) {
		ofPoint attractPt(ofGetMouseX(), ofGetMouseY());
		frc = attractPt - pos;

		//let get the distance and only repel points close to the mouse
		float dist = frc.length();

		frc.y = ofSignedNoise(uniqueVal, pos.x * 0.006, ofGetElapsedTimef()*0.2) * 0.09 + 0.28;
		frc.normalize();

		vel.x = 0;

		if ((dist < 50) || (touche == true)) {
			//vel.y *= drag; //apply drag
			vel.y += frc.y * 2.6; //apply force
			touche = true;
		}
		else {
			vel.y = 0;
		}
		if (pos.y + vel.y > ofGetHeight()) {
			vel.y = 0;
		}
	}
	
	//2 - UPDATE OUR POSITION
	
	pos += vel; 
	
	
	//3 - (optional) LIMIT THE PARTICLES TO STAY ON SCREEN 
	//we could also pass in bounds to check - or alternatively do this at the ofApp level
	
	if( pos.x > ofGetWidth() ){
		pos.x = ofGetWidth();
		vel.x *= -1.0;
	}else if( pos.x < 0 ){
		pos.x = 0;
		vel.x *= -1.0;
	}
	if( pos.y > ofGetHeight() ){
		pos.y = ofGetHeight();
		vel.y *= -1.0;
	}
	else if( pos.y < 0 ){
		pos.y = 0;
		vel.y *= -1.0;
	}	
	
}

//------------------------------------------------------------------
void demoParticle::draw(){

	if( mode == PARTICLE_MODE_ATTRACT ){
		ofSetColor(255, 63, 180);
	}
	else if( mode == PARTICLE_MODE_REPEL ){
		ofSetColor(208, 255, 63);
	}
	else if( mode == PARTICLE_MODE_NOISE ){
		ofSetColor(99, 63, 255);
	}
	else if( mode == PARTICLE_MODE_NEAREST_POINTS ){
		ofSetColor(103, 160, 237);
	}
	else if (mode == PARTICLE_MODE_SNOW) {
		ofSetColor(255, 255, 255);
	}
	else if (mode == PARTICLE_MODE_GRILL) {
		ofSetColor(255, 0, 255);
	}

	ofDrawCircle(pos.x, pos.y, scale * 2.0);

	//ofDrawTriangle(pos.x, pos.y, pos.x+10, pos.y+10, pos.x-10, pos.y+10);
		
	/*
	char letters[] = "abcdefghijklmnopqrstuvwxyz";
	char letter = letters[rand() % 26];
	ofDrawBitmapString(letter, pos.x, pos.y);
	*/
}

