//	Player.h
//	Player (nyancat) class

#include <vector>

struct Point {
	float x;
	float z;
};

class Player {
public:
	float color[3];
	uint8_t direction;
	std::vector<Point> positions;

	void turn(bool);
	void DrawCat();
	Point tick();
	Player();
	Player(float, float);

private:
	bool turning;
};

