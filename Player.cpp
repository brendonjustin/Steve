//	Player.cpp
//	Player (cat) class

#include "Player.h"
#include "png_loader.h"
#include <cmath>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#ifndef M_PI
static const float M_PI = 3.14159265;
#endif

#ifndef M_PI_2
static const float M_PI_2 = 1.57079633;
#endif

static const string NYAN_TEXTURE = "frame00.png";
static int tex_width = 400, tex_height = 280;

Player::Player() {
	this->init(10, 10, 0);
}

Player::Player(float initialX, float initialZ, uint8_t initialDirection) {
	this->init(initialX, initialZ, initialDirection);

	//	Load the nyan cat!
	texture = loadTexture(NYAN_TEXTURE, tex_width, tex_height);

	//	Handle errors
	if (texture == 0) {
		// TODO
	}
}

void Player::init(float initialX, float initialZ, uint8_t initialDirection) {
	turning = false;

	Point initialPt;
	initialPt.x = initialX;
	initialPt.z = initialZ;

	positions.push_back(initialPt);
	positions.push_back(initialPt);

	direction = initialDirection;
}

//	Turn (responding to user input)
void Player::turn(bool turnRight) {
	positions.push_back(positions[positions.size() - 1]);

	if (!turning) {
		turning = true;

		// TODO: animate?
		if (turnRight) {
			direction -= 1;
		} else {
			direction += 1;
		}

		direction = direction % 4;
	}

	turning = false;
}

//	Move the player 
Point Player::tick() {
	Point currentPt = positions[positions.size() - 1];

	currentPt.x -= sin(direction * M_PI_2) / 10;
	currentPt.z -= cos(direction * M_PI_2) / 10;

	switch (direction) {
	case 0:
		break;
	case 1:
		break;
	case 2:
		break;
	case 3:
		break;
	}

	positions[positions.size() - 1] = currentPt;
}

// 	Player Cat
void Player::drawCat(){

	float player1color[3] = {1.0, 0.0, 0.0}; 
	glRotatef(180.0, 0.0, 1.0, 0.0); // make cat point down the z-axis initially.
	//glScalef(0.5, 1.0, 1.0);

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);
	glBindTexture(GL_TEXTURE_2D, texture);
	glBegin(GL_QUADS);

	//	unknown
	//glTexCoord2f(0.0, 0.0); glVertex3f(-1,-1,-1);
	//glTexCoord2f(0.0, 1.0); glVertex3f( 1,-1,-1);
	//glTexCoord2f(1.0, 1.0); glVertex3f( 1, 1,-1);
	//glTexCoord2f(1.0, 0.0); glVertex3f(-1, 1,-1);
	 
	glVertex3f(-1,-1,-1);
	glVertex3f( 1,-1,-1);
	glVertex3f( 1, 1,-1);
	glVertex3f(-1, 1,-1);

	//	Top?
	glVertex3f(-1,-1,-1);
	glVertex3f(-1,-1, 1);
	glVertex3f(-1, 1, 1);
	glVertex3f(-1, 1,-1);
	 
	//	unknown
	//glTexCoord2f(0.0, 0.0); glVertex3f( 1,-1, 1);
	//glTexCoord2f(0.0, 1.0); glVertex3f( 1,-1,-1);
	//glTexCoord2f(1.0, 1.0); glVertex3f( 1, 1,-1);
	//glTexCoord2f(1.0, 0.0); glVertex3f( 1, 1, 1);
 
	glVertex3f( 1,-1, 1);
	glVertex3f( 1,-1,-1);
	glVertex3f( 1, 1,-1);
	glVertex3f( 1, 1, 1);

	//	Left side?
	glTexCoord2f(0.0, 0.0); glVertex3f(-1,-1,-1);
	glTexCoord2f(0.0, 1.0); glVertex3f( 1,-1,-1);
	glTexCoord2f(1.0, 1.0); glVertex3f( 1,-1, 1);
	glTexCoord2f(1.0, 0.0); glVertex3f(-1,-1, 1);
 
 	//	unknown
	//glTexCoord2f(0.0, 0.0); glVertex3f(-1,-1, 1);
	//glTexCoord2f(0.0, 1.0); glVertex3f( 1,-1, 1);
	//glTexCoord2f(1.0, 1.0); glVertex3f( 1, 1, 1);
	//glTexCoord2f(1.0, 0.0); glVertex3f(-1, 1, 1);

	glVertex3f(-1,-1, 1);
	glVertex3f( 1,-1, 1);
	glVertex3f( 1, 1, 1);
	glVertex3f(-1, 1, 1);

	glEnd();
	glFlush();
	glDisable(GL_TEXTURE_2D);
}
