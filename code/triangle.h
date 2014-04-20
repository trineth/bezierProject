#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "point.h"

class Triangle{
	int pointsStored;
	Point points[3];
public:
	Triangle();
	Triangle(Point one, Point two, Point three);

	Triangle( const Triangle& other );

	Triangle& operator=(const Triangle& rhs);

	void addPoint(Point p);
	Point* getPoints();
	Point getPoint(int i);
};

#endif