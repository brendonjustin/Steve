//	Player.h
//	Player (bike) class

#include <vector>

struct Point {
	float x;
	float z;
};

class Player {
public:
	float color[3];
	int direction;
	Point currentPos;
	//vector<Point> prevPts;

	void turn(bool right);
	void DrawCat();
	Point tick();
	Player();

private:
	bool turning;
};

