#ifndef BEZIER_H
#define BEZIER_H

#include "patch.h"
#include "point.h"
#include "quad.h"

class Bezier{
	std::vector<Point> bezPoints;
	std::vector<Quad> bezQuads;
	int quadNum;
	float flatBound;
public:
	Bezier();

	Point bezcurveinterp(std::vector<Point> curve, float u, Point &dPdu);
	Point bezpatchinterp(Patch patch, float u, float v, Point &n);
	void subdividepatch(Patch patch, float step);

	Point crossP(Point a, Point b);
	void savesurfacepointandnormal(Point &p);
	void constructQuads(float numdiv);

	int getQuadNum();
	Quad getQuad(int i);
	void printQuad();
};


#endif