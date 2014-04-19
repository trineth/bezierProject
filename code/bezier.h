#ifndef BEZIER_H
#define BEZIER_H

#include "patch.h"
#include "point.h"
#include "quad.h"

class Bezier{
	std::vector<Point> bezPoints;
	std::vector<Quad> bezQuads;
public:
	Bezier() {};

	Point bezcurveinterp(Point *curve[], float u, Point &dPdu);
	Point bezpatchinterp(Patch patch, float u, float v, Point &n);
	void subdividepatch(Patch patch, float step);

	Point cross(Point a, Point b);
	void savesurfacepointandnormal(Point p, float iu, float iv, int numdiv);
	void constructQuads(float numdiv);

	int getQuadNum();
	Quad getQuad(int i);
};


#endif