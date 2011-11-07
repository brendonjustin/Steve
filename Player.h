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
	float direction;
	Point currentPos;
	//vector<Point> prevPts;

	void turn(bool right);
	Point tick();

private:
	bool turning;
};

