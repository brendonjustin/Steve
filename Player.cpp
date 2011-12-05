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

static const unsigned int TRAIL_HEIGHT = 15;
static const unsigned int FRAME_INTERVAL = 12;
static const unsigned int FRAME_CYCLE_INTERVAL = 72;

static const unsigned int INVERSE_CAT_SPEED = 1;

const float Player::RIGHT = 0.25;
const float Player::TOP = 85;
const float Player::BOT = -15;
const float Player::FWD = 20;
const float Player::BACK = 0;

Player::Player(GLuint *textureFrames, GLuint *rainbowTexture) {
	this->init(10, 10, 0, textureFrames, rainbowTexture);
}

Player::Player(float initialX, float initialZ, uint8_t initialDirection, GLuint *textureFrames, GLuint *rainbowTexture) {
	this->init(initialX, initialZ, initialDirection, textureFrames, rainbowTexture);
}

void Player::init(float initialX, float initialZ, uint8_t initialDirection, GLuint *textureFrames, GLuint *rainbowTexture) {
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
	rainbowTex = rainbowTexture;

	playerColor[0] = 1.0;
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

	currentPt.x -= sin(direction * M_PI_2) / INVERSE_CAT_SPEED;
	currentPt.z -= cos(direction * M_PI_2) / INVERSE_CAT_SPEED;

	positions[positions.size() - 1] = currentPt;

	return currentPt;
}

// 	Player Cat
void Player::drawCat(){
	glPushMatrix();
	glTranslatef(positions[positions.size() - 1].x, 0, positions[positions.size() - 1].z);
	glRotatef(this->direction * 90, 0.0, 1.0, 0.0);

	glColor3f(0.0, 0.0, 0.0);
	glRotatef(180.0, 0.0, 1.0, 0.0); // make cat point down the z-axis initially.
	glScalef(0.25, 0.25, 0.25);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	//	Just use the first frame of the animation
	//glBindTexture(GL_TEXTURE_2D, *(texFrames+0));

	//	Rotate texture every 8 calls to draw, making an animation
	glBindTexture(GL_TEXTURE_2D, *(texFrames+(frameCount++/FRAME_INTERVAL)));
	frameCount = frameCount % FRAME_CYCLE_INTERVAL;

	glBegin(GL_QUADS);

	//	Back side?
	glVertex3f(-RIGHT, BOT, BACK);
	glVertex3f( RIGHT, BOT, BACK);
	glVertex3f( RIGHT, TOP, BACK);
	glVertex3f(-RIGHT, TOP, BACK);

	//	Right side
	glTexCoord2f(0.0, 0.0); glVertex3f(-RIGHT, BOT, BACK);
	glTexCoord2f(1.0, 0.0); glVertex3f(-RIGHT, BOT,  FWD);
	glTexCoord2f(1.0, 1.0); glVertex3f(-RIGHT, TOP,  FWD);
	glTexCoord2f(0.0, 1.0); glVertex3f(-RIGHT, TOP, BACK);

	//	Left side
	glTexCoord2f(1.0, 0.0); glVertex3f( RIGHT, BOT,  FWD);
	glTexCoord2f(0.0, 0.0); glVertex3f( RIGHT, BOT, BACK);
	glTexCoord2f(0.0, 1.0); glVertex3f( RIGHT, TOP, BACK);
	glTexCoord2f(1.0, 1.0); glVertex3f( RIGHT, TOP,  FWD);

	//	Bottom side?
	glVertex3f(-RIGHT, BOT, BACK);
	glVertex3f( RIGHT, BOT, BACK);
	glVertex3f( RIGHT, BOT,  FWD);
	glVertex3f(-RIGHT, BOT,  FWD);

	//	Front side?
	glVertex3f(-RIGHT, BOT,  FWD);
	glVertex3f( RIGHT, BOT,  FWD);
	glVertex3f( RIGHT, TOP,  FWD);
	glVertex3f(-RIGHT, TOP,  FWD);

	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);
	glFlush();

	glPopMatrix();
}

void Player::drawTrail() {
	glPushMatrix();
	glColor3fv(playerColor);

	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glBindTexture( GL_TEXTURE_2D, *rainbowTex );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT );
	glTexParameterf( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT );

	glBegin(GL_QUADS);

	Point playerPt;
	Point playerPt2;

	float xDiff = 0.0, zDiff = 0.0;
	float rptCount = 1.0;

	for(int i=0; i < positions.size() - 1; ++i) {
		playerPt = positions[i];
		playerPt2 = positions[i+1];

		xDiff = fabs(playerPt.x - playerPt2.x);
		zDiff = fabs(playerPt.z - playerPt2.z);

		if (xDiff > zDiff) {
			rptCount = xDiff / 10;
		} else {
			rptCount = zDiff / 10;
		}

		//cout << playerPt.x << "  " << playerPt.z << "  ";
		//	Top left, top right, bottom right, bottom left
		glTexCoord2f(0.0, 1.0); glVertex3f(playerPt.x, TRAIL_HEIGHT, playerPt.z);
		glTexCoord2f(rptCount, 1.0); glVertex3f(playerPt2.x, TRAIL_HEIGHT, playerPt2.z);
		glTexCoord2f(rptCount, 0.0); glVertex3f(playerPt2.x, 5, playerPt2.z);
		glTexCoord2f(0.0, 0.0); glVertex3f(playerPt.x, 5, playerPt.z);
	}

	glEnd();
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	glPopMatrix();
}

void Player::draw() {
	this->drawCat();
	this->drawTrail();
}
