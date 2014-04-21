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
	void adaptiveSubdivide(Patch patch, Ptriangle t, int count);
	void constructTriangle(Patch patch, Ptriangle t);
	void zeroSideFlat(Patch patch, Ptriangle t, int count);
	void oneSideFlat(Patch patch, Ptriangle t, int i, int count);
	void twoSideFlat(Patch patch, Ptriangle t, int i, int count);
	bool isFlat(Patch patch, Ptriangle t, int side);
	bool isFlat(Patch patch);
	Point findMidpoint(std::vector<Point> quad);

	int getTriangleNum();
	Triangle getTriangle(int i);

	void setFlatBound(float f);
};


#endif