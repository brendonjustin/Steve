//	Player.cpp
//	Player (cat) class

#include "Player.h"
#include <cmath>

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

#ifndef M_PI
static const float M_PI = 3.14159265;
static const float M_PI_2 = 1.57079633;
#endif

Player::Player() {
	turning = false;
}

Player::Player(float initialX, float initialZ) {
	turning = false;

	Point initialPt;
	initialPt.x = initialX;
	initialPt.z = initialZ;

	positions.push_back(initialPt);
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
}

//	Move the player 
Point Player::tick() {
	Point currentPt = positions[positions.size() - 1];

	currentPt.x -= sin(direction * M_PI_2);
	currentPt.z -= cos(direction * M_PI_2);

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
void Player::DrawCat(){

	float player1color[3] = {1.0, 0.0, 0.0}; 
	glRotatef(180.0, 0.0, 1.0, 0.0); // make cat point down the z-axis initially.
	glScalef(0.5, 1.0, 1.0);
	glBegin(GL_QUADS);
	glVertex3f(-1,-1,-1);
	glVertex3f( 1,-1,-1);
	glVertex3f( 1, 1,-1);
	glVertex3f(-1, 1,-1);
	 
	glVertex3f(-1,-1,-1);
	glVertex3f(-1,-1, 1);
	glVertex3f(-1, 1, 1);
	glVertex3f(-1, 1,-1);
	 
	glVertex3f( 1,-1, 1);
	glVertex3f( 1,-1,-1);
	glVertex3f( 1, 1,-1);
	glVertex3f( 1, 1, 1);
 
	glVertex3f(-1,-1,-1);
	glVertex3f( 1,-1,-1);
	glVertex3f( 1,-1, 1);
	glVertex3f(-1,-1, 1);
 
	glVertex3f(-1,-1, 1);
	glVertex3f( 1,-1, 1);
	glVertex3f( 1, 1, 1);
	glVertex3f(-1, 1, 1);

	glEnd();

}
