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
	float color[3];
	uint8_t direction;
	GLuint texture;
	std::vector<Point> positions;

	void turn(bool);
	void drawCat();
	Point tick();
	Player();
	Player(float, float, uint8_t);

private:
	bool turning;
	void init(float, float, uint8_t);
};

