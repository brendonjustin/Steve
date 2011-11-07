//	Player.h
//	Player (bike) class

#include <vector>

struct Point {
	float x;
	float z;
};

class Player {
public:
	float[3] color;
	float direction;
	Point currentPos;
	vector<Point> prevPts;
};

