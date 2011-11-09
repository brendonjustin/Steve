//	Player.cpp
//	Player (cat) class

#include "Player.h"

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif


Player::Player() {
	turning = false;
}

//	Turn (responding to user input)
void Player::turn(bool right) {
	if (!turning) {
		turning = true;

		// TODO: animate?
		if (right) {
			direction += -90;
		} else {
			direction += 90;
		}
	}
}

//	Move the player 
Point Player::tick() {
	switch (direction) {
	case 0:
		break;
	case 90:
		break;
	case 180:
		break;
	case 270:
		break;
	}
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
