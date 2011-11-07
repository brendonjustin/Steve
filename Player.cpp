//	Player.cpp
//	Player (bike) class

#include "Player.h"

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
