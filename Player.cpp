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
#endif

#ifndef M_PI_2
static const float M_PI_2 = 1.57079633;
#endif

const float Player::right = 1.0;
const float Player::top = 100;
const float Player::fwd = 20;

Player::Player(GLuint *textureFrames) {
	this->init(10, 10, 0, textureFrames);
}

Player::Player(float initialX, float initialZ, uint8_t initialDirection, GLuint *textureFrames) {
	this->init(initialX, initialZ, initialDirection, textureFrames);
}

void Player::init(float initialX, float initialZ, uint8_t initialDirection, GLuint *textureFrames) {
	frameCount = 0;

	turning = false;

	Point initialPt;
	initialPt.x = initialX;
	initialPt.z = initialZ;

	positions.push_back(initialPt);
	positions.push_back(initialPt);

	direction = initialDirection;

	//	Use the textures passed in from init
	texFrames = textureFrames;

	playerColor[0] = 0.0;
	playerColor[1] = 0.0;
	playerColor[2] = 0.0; 

	//	Note that this isn't used, the way texture animation is done
	catList = glGenLists(1);
	glNewList(catList, GL_COMPILE);

	glPushMatrix();
	this->drawCat();
	glPopMatrix();

	glEndList();
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

	positions[positions.size() - 1] = currentPt;

	return currentPt;
}

// 	Player Cat
void Player::drawCat(){
	glColor3fv(playerColor);
	glRotatef(180.0, 0.0, 1.0, 0.0); // make cat point down the z-axis initially.
	glScalef(0.25, 0.25, 0.25);

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL);

	//	Just use the first frame of the animation
	//glBindTexture(GL_TEXTURE_2D, *(texFrames+0));

	//	Rotate texture every 8 calls to draw, making an animation
	glBindTexture(GL_TEXTURE_2D, *(texFrames+((++frameCount)/8)));
	frameCount = frameCount % 48;

	glBegin(GL_QUADS);

	//	Back side?
	glVertex3f(-right,   0,   0);
	glVertex3f( right,   0,   0);
	glVertex3f( right, top,   0);
	glVertex3f(-right, top,   0);

	//	Right side?
	//	Currently draws nyan cat upside-down
	glTexCoord2f(0.0, 1.0); glVertex3f(-right,   0,   0);
	glTexCoord2f(1.0, 1.0); glVertex3f(-right,   0, fwd);
	glTexCoord2f(1.0, 0.0); glVertex3f(-right, top, fwd);
	glTexCoord2f(0.0, 0.0); glVertex3f(-right, top,   0);
	 
	//	Left side
	glTexCoord2f(1.0, 0.0); glVertex3f( right,   0, fwd);
	glTexCoord2f(0.0, 0.0); glVertex3f( right,   0,   0);
	glTexCoord2f(0.0, 1.0); glVertex3f( right, top,   0);
	glTexCoord2f(1.0, 1.0); glVertex3f( right, top, fwd);

	//	Bottom side?
	glVertex3f(-right,   0,   0);
	glVertex3f( right,   0,   0);
	glVertex3f( right,   0, fwd);
	glVertex3f(-right,   0, fwd);
 
	//	Front side?
	glVertex3f(-right,   0, fwd);
	glVertex3f( right,   0, fwd);
	glVertex3f( right, top, fwd);
	glVertex3f(-right, top, fwd);

	glEnd();
	glFlush();
	glDisable(GL_TEXTURE_2D);
}

void Player::draw() {
	glPushMatrix();
	glTranslatef(positions[positions.size() - 1].x, 0, positions[positions.size() - 1].z);
	glRotatef(this->direction * 90, 0.0, 1.0, 0.0);
	//glCallList(catList);
	this->drawCat();
	glPopMatrix();
}
