#ifndef QUAD_H
#define QUAD_H

#include "point.h"

class Quad{
	int pointsStored;
	int quadNum;
	bool subdivided;
	Point points[4];
	//Quad* quads;
public:
	Quad();
	Quad(Point one, Point two, Point three, Point four);

	Quad( const Quad& other );

	Quad& operator=(const Quad& rhs);

	void addPoint(Point p);
	bool isSubdivided();
	Point* getPoints();
};

#endif