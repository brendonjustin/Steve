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
	static const float right;
	static const float top;
	static const float fwd;

	float playerColor[3];
	GLuint texture;
	//GLuint texFrames[6];
	//GLuint texFrame00;
	//GLuint texFrame01;
	//GLuint texFrame02;
	//GLuint texFrame03;
	//GLuint texFrame04;
	//GLuint texFrame05;

	unsigned int catList; // Display lists base index.

	uint8_t direction;
	std::vector<Point> positions;

	void turn(bool);
	void drawCat();
	void draw();
	Point tick();
	Player();
	Player(float, float, uint8_t);

private:
	bool turning;
	void init(float, float, uint8_t);
};

