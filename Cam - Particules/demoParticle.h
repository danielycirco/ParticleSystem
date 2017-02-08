#pragma once

#include "ofMain.h"

enum particleMode{
	PARTICLE_MODE_ATTRACT = 0,
	PARTICLE_MODE_REPEL,
	PARTICLE_MODE_NEAREST_POINTS,
	PARTICLE_MODE_NOISE,
	PARTICLE_MODE_SNOW,
	PARTICLE_MODE_GRILL
};

class demoParticle{

	public:
		demoParticle();
		
		void setMode(particleMode newMode);	
		void setAttractPoints( vector <ofPoint> * attract );

		void reset();
		void update();
		void draw();

		void newPosition(int, int);
		
		ofPoint pos;
		ofPoint vel;
		ofPoint frc;
		bool touche;
		
		float drag; 
		float uniqueVal;
		float scale;
		
		particleMode mode;
		
		vector <ofPoint> * attractPoints; 

		int ballX;
		int ballY;
};