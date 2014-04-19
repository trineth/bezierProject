#ifndef QUAD_H
#define QUAD_H

#include "point.h"

class Quad{
	int pointsStored;
	bool subdivided;
	Point points[4];
	Quad* quads;
public:
	Quad();
	Quad(Point one, Point two, Point three, Point four);

	void addPoint(Point p);
	bool isSubdivided();
	Point* getPoints();
};

#endif