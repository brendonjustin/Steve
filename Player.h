//	Player.h
//	Player (nyancat) class

#include <vector>
#ifdef __APPLE__
#  include <stdint.h>
#else
#  include <cstdint>
#endif

#ifdef __APPLE__
#  include <GLUT/glut.h>
#else
#  include <GL/glut.h>
#endif

struct Point {
	float x;
	float z;
};

class Player {
public:
	static const float RIGHT;
	static const float TOP;
	static const float BOT;
	static const float FWD;
	static const float BACK;

	bool collided;

	float playerColor[3];
	GLuint *texFrames;
	GLuint *rainbowTex;
	unsigned int frameCount;

	unsigned int catList; // Display lists base index.

	uint8_t direction;
	std::vector<Point> positions;

	void turn(bool);
	void draw();
	Point tick();
	Player(GLuint *, GLuint *);
	Player(float, float, uint8_t, GLuint *, GLuint *);

private:
	void init(float, float, uint8_t, GLuint *, GLuint *);
	void drawCat();
	void drawTrail();
};

