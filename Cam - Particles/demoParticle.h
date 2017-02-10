#pragma once
#include "ofMain.h"

enum particleMode{
	PARTICLE_MODE_ATTRACT = 0,
	PARTICLE_MODE_REPEL,
	PARTICLE_MODE_NEAREST_POINTS,
	PARTICLE_MODE_SNOW,
	PARTICLE_MODE_DUST,
	PARTICLE_MODE_GRILL,
	PARTICLE_MODE_LIFE,
	PARTICLE_MODE_X,
	PARTICLE_MODE_WORD
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
		ofPoint posInit;
		ofPoint vel;
		ofPoint frc;
		bool touche;
		bool live;
		int lifetime;
		
		float drag; 
		float uniqueVal;
		float scale;
		
		particleMode mode;
		
		vector <ofPoint> * attractPoints; 

		int ballX;
		int ballY;
};