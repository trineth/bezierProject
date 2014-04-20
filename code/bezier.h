#ifndef BEZIER_H
#define BEZIER_H

#include "patch.h"
#include "point.h"
#include "quad.h"
#include "triangle.h"
#include "ptriangle.h"

class Bezier{
	std::vector<Point> bezPoints;
	std::vector<Quad> bezQuads;
	std::vector<Triangle> bezTriangles;
	int quadNum;
	int triangleNum;
	float flatBound;
public:
	Bezier();

	Point bezcurveinterp(std::vector<Point> curve, float u, Point &dPdu);
	Point bezpatchinterp(Patch patch, float u, float v, Point &n);
	Point crossP(Point a, Point b);

//******************************************************************
// Uniform Subdivision Methods
//******************************************************************
	void subdividepatch(Patch patch, float step);
	void savesurfacepointandnormal(Point &p);
	void constructQuads(float numdiv);

	int getQuadNum();
	Quad getQuad(int i);
	void printQuad();


//******************************************************************
// Adaptive Subdivision Methods
//******************************************************************
	void adaptiveExecute(Patch patch, float step);
	void adaptiveSubdivide(Patch patch, int itrLeft, Ptriangle t);
	void constructTriangle(Patch patch, Ptriangle t);
	void zeroSideFlat(Patch patch, int itrLeft, Ptriangle t);
	void oneSideFlat(Patch patch, int itrLeft, Ptriangle t, int i);
	void twoSideFlat(Patch patch, int itrLeft, Ptriangle t, int i);
	bool isFlat(Patch patch, Ptriangle t, int side);
	bool isFlat(Patch patch);
	Point findMidpoint(std::vector<Point> quad);

	int getTriangleNum();
	Triangle getTriangle(int i);
};


#endif