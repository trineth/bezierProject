#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include "Eigen/Dense"
#include "bezier.h"
#include "patch.h"
#include "point.h"
#include "quad.h"
#include "triangle.h"
#include "ptriangle.h"

Bezier::Bezier() {
	std::vector<Point> bezPoints(5);
	std::vector<Quad> bezQuads(5);
	std::vector<Triangle> bezTriangles(5);
	quadNum = 0;
	triangleNum = 0;
}

// given the control points of a bezier curve
// and a parametric value, return the curve 
// point and derivative
Point Bezier::bezcurveinterp(std::vector<Point> curve, float u, Point &dPdu) {
	// first, split each of the three segments
	// to form two new ones AB and BC
	Point A = curve[0] * (1.0-u) + curve[1] * u;
	Point B = curve[1] * (1.0-u) + curve[2] * u;
	Point C = curve[2] * (1.0-u) + curve[3] * u;
	// now, split AB and BC to form a new segment DE
	Point D = A * (1.0-u) + B * u;
	Point E = B * (1.0-u) + C * u;
	// finally, pick the right point on DE,
	// this is the point on the curve
	Point p = D * (1.0-u) + E * u;
	// compute derivative also
	dPdu = 3 * (E - D);
	return p;
}

// given a control patch and (u,v) values, find 
// the surface point and normal
Point Bezier::bezpatchinterp(Patch patch, float u, float v, Point &n) {
	Point p;
	Point dPdu;
	Point dPdv;
	Point trash;
	std::vector<Point> vcurve(4);
	std::vector<Point> ucurve(4);

	bool horizontal = true;
	bool vertical = false;

	// build control points for a Bezier curve in v
	vcurve[0] = bezcurveinterp(patch.getCurve(0, horizontal), u, trash);
	vcurve[1] = bezcurveinterp(patch.getCurve(1, horizontal), u, trash);
	vcurve[2] = bezcurveinterp(patch.getCurve(2, horizontal), u, trash);
	vcurve[3] = bezcurveinterp(patch.getCurve(3, horizontal), u, trash);
	// build control points for a Bezier curve in u
	ucurve[0] = bezcurveinterp(patch.getCurve(0, vertical), v, trash);
	ucurve[1] = bezcurveinterp(patch.getCurve(1, vertical), v, trash);
	ucurve[2] = bezcurveinterp(patch.getCurve(2, vertical), v, trash);
	ucurve[3] = bezcurveinterp(patch.getCurve(3, vertical), v, trash);
	// evaluate surface and derivative for u and v
	p = bezcurveinterp(vcurve, v, dPdv);
	p = bezcurveinterp(ucurve, u, dPdu);
	// take cross product of partials to find normal
	n = crossP(dPdu, dPdv);
	n = n / n.length();
	Point n2 = crossP(dPdv, dPdu);
	n2 = n2 / n2.length();
	p.saveNormal(n, n2);
	return p;
}

//  Returns cross product between two 3D vectors
Point Bezier::crossP(Point a, Point b) {
	float* valueA = a.getValues();
	float* valueB = b.getValues();
	Eigen::Vector3f vecA(valueA[0], valueA[1], valueA[2]);
	Eigen::Vector3f vecB(valueB[0], valueB[1], valueB[2]);
	Eigen::Vector3f result = vecA.cross(vecB);
	Point point(result[0], result[1], result[2]);
	return point;
}

//******************************************************************
// Uniform Subdivision Methods
//******************************************************************

// given a patch, perform uniform subdivision
void Bezier::subdividepatch(Patch patch, float step) {
	// compute how many subdivisions there 
	// are for this step size
	float epsilon = 0;
	float numdiv = ((1 + epsilon)/ step) + 1;
	float u, v;
	Point p, n;

	bezPoints.clear();
	bezPoints.reserve((numdiv+1)*(numdiv+1));
	//std::cout << "Numdiv: " << numdiv << "\n";

	// for each parametric value of u
	for (int iu = 0; iu < numdiv; iu++) {
		u = iu * step;
		// for each parametric value of v
		for (int iv = 0; iv < numdiv; iv++) {
			v = iv * step;
			// evaluate surface
			p = bezpatchinterp(patch, u, v, n);
			savesurfacepointandnormal(p);
		}
	}
	constructQuads(numdiv);
	//printQuad();
}

void Bezier::savesurfacepointandnormal(Point &p) {
	bezPoints.push_back(p);
}

void Bezier::constructQuads(float numdiv) {
	bezQuads.clear();
	bezQuads.reserve((numdiv)*(numdiv));
	quadNum = 0;

	int pos1, pos2, pos3, pos4;
	int position;
	for (int u = 0; u < numdiv-1; u++) {
		for (int v = 0; v < numdiv-1; v++) {
			pos1 = u*numdiv + v;
			pos2 = u*numdiv + v + 1; 
			pos3 = (u+1)*numdiv + v + 1;
			pos4 = (u+1)*numdiv + v;  //Points stored c-clockwise position

			Point p1 = bezPoints[pos1]; //std::cout << p1.getValues()[0] << " " << p1.getValues()[1] << " " << p1.getValues()[2] << "\n";
			Point p2 = bezPoints[pos2]; //std::cout << p2.getValues()[0] << " " << p2.getValues()[1] << " " << p2.getValues()[2] << "\n";
			Point p3 = bezPoints[pos3]; //std::cout << p3.getValues()[0] << " " << p3.getValues()[1] << " " << p3.getValues()[2] << "\n";
			Point p4 = bezPoints[pos4]; //std::cout << p4.getValues()[0] << " " << p4.getValues()[1] << " " << p4.getValues()[2] << "\n";

			Quad quad(bezPoints[pos1],bezPoints[pos2],bezPoints[pos3],bezPoints[pos4]);
			bezQuads.push_back(quad); 
			quadNum++;
		}
	}
}

int Bezier::getQuadNum() {
	return quadNum;
}

Quad Bezier::getQuad(int i) {
	if (i < bezQuads.size()) {
		return bezQuads[i];
	} else {
		std::cout << "bezQuads[" << i << "] access out of bounds. Max: " << bezQuads.size() << "\n";
		exit(1);
	}
}

void Bezier::printQuad() {
	for (int i = 0; i < bezQuads.size(); i++) {
		Point* p = bezQuads[i].getPoints();
		float* values = p[0].getValues();
		std::cout << values[0] << " " << values[1] << " " << values [2] << "\n";
		values = p[1].getValues();
		std::cout << values[0] << " " << values[1] << " " << values [2] << "\n";
		values = p[2].getValues();
		std::cout << values[0] << " " << values[1] << " " << values [2] << "\n";
		values = p[3].getValues();
		std::cout << values[0] << " " << values[1] << " " << values [2] << "\n";
		std::cout << "\n";
	}
}

//******************************************************************
// Adaptive Subdivision Methods
//******************************************************************

// Sets up the adaptive tessellation process, recursively
// calling adaptiveSubdivide if the patch is not flat.
void Bezier::adaptiveExecute(Patch patch, float step) {
	Ptriangle triangle1;
	triangle1.addPoint(0,0);
	triangle1.addPoint(1,1);
	triangle1.addPoint(1,0);
	Ptriangle triangle2;
	triangle2.addPoint(0,0);
	triangle2.addPoint(0,1);
	triangle2.addPoint(1,1);
	bezTriangles.clear();
	triangleNum = 0;
	if (step >= 1) {
		//render as is:
		//split into two triangles, add them to triangle list
		bezTriangles.reserve(2);
		constructTriangle(patch, triangle1);
		constructTriangle(patch, triangle2);
	} else if (isFlat(patch)) {
		//render as flat
		/* code */
		//split into two triangles, add them to triangle list
		bezTriangles.reserve(2);
		constructTriangle(patch, triangle1);
		constructTriangle(patch, triangle2);
	} else {
		//subdivide in half.  Break into triangles, call adaptiveSubdivide(), incr step
		//Triangle 1 has points at (u,v): (0,0) (1,1) (1,0)
		//Triangle 2 has points at (u,v): (0,0) (0,1) (1,1)
		int iterationsLeft = (1 / step) - 1;
		bezTriangles.reserve(pow(6,iterationsLeft));
		adaptiveSubdivide(patch, iterationsLeft, triangle1);
		adaptiveSubdivide(patch, iterationsLeft, triangle2);
	}
}

// Evaluates t at its endpoints, subdivides if necessary according to info from patch.
void Bezier::adaptiveSubdivide(Patch patch, int itrLeft, Ptriangle t) {
	if (itrLeft == 0) {
		//render as is
		constructTriangle(patch, t);
		return;
	}

	bool flatSides[3] = {false, false, false}; //true if flat, false if not.
	int numFlat = 0;
	//subdivide more and call adaptiveSubdivide()
	//for each edge
	for (int i = 0; i < 3; i++) {
		if (isFlat(patch, t, i)) {
			//render as is
			flatSides[i] = true;
			numFlat++;
		}
	}

	if (numFlat == 0) {
		zeroSideFlat(patch, itrLeft, t);
	} else if (numFlat == 1) {

		for (int i = 0; i < 3; i++) {
			if (!flatSides[i]) {
				oneSideFlat(patch, itrLeft, t, i);
			}
		}

	} else if (numFlat == 2) {

		if (!flatSides[0] && !flatSides[1]) {
			twoSideFlat(patch, itrLeft, t, 2);
		} else if (!flatSides[1] && !flatSides[2]) {
			twoSideFlat(patch, itrLeft, t, 0);
		} else {
			twoSideFlat(patch, itrLeft, t, 1);
		}

	} else {
		constructTriangle(patch, t);
	}
}

//Constructs and places the triangle in bezTriangles
//to be rendered in main.cc
void Bezier::constructTriangle(Patch patch, Ptriangle t) {
	Point temp;
	std::pair<float,float> p0 = t.getPoint(0);
	std::pair<float,float> p1 = t.getPoint(1);
	std::pair<float,float> p2 = t.getPoint(2);

	Point zero = bezpatchinterp(patch, p0.first, p0.second, temp);
	Point one = bezpatchinterp(patch, p1.first, p1.second, temp);
	Point two = bezpatchinterp(patch, p2.first, p2.second, temp);

	Triangle triangle(zero, one, two);
	bezTriangles.push_back(triangle);
	triangleNum++;
}

//Handles the case where no sides of the triangle are flat.
void Bezier::zeroSideFlat(Patch patch, int itrLeft, Ptriangle t) {
	std::pair<float,float> p1 = t.getPoint(0);
	std::pair<float,float> p2 = t.getPoint(1);
	std::pair<float,float> p3 = t.getPoint(2);
	float midU0 = (p1.first + p2.first) * 0.5;
	float midV0 = (p1.second + p2.second) * 0.5;
	float midU1 = (p2.first + p3.first) * 0.5;
	float midV1 = (p2.second + p3.second) * 0.5;
	float midU2 = (p3.first + p1.first) * 0.5;
	float midV2 = (p3.second + p1.second) * 0.5;

	std::pair<float,float> x (midU0, midV0); //Unflat side midpoint0
	std::pair<float,float> y (midU1, midV1); //Unflat side midpoint1
	std::pair<float,float> z (midU2, midV2); //Unflat side midpoint2
	std::pair<float,float> a = p1;
	std::pair<float,float> b = p2;
	std::pair<float,float> c = p3; //Opposing the flat side

	float centerX = a.first + (2/3)*(a.first-y.first);
	float centerY = a.second + (2/3)*(a.second-y.second);
	std::pair<float,float> center (centerX,centerY);  //center of the triangle

	//Six triangle subdivisions.
	Ptriangle t1(a,center,x);
	Ptriangle t2(x,center,b);
	Ptriangle t3(b,center,y);
	Ptriangle t4(y,center,c);
	Ptriangle t5(c,center,z);
	Ptriangle t6(z,center,a);

	adaptiveSubdivide(patch, itrLeft-1, t1);
	adaptiveSubdivide(patch, itrLeft-1, t2);
	adaptiveSubdivide(patch, itrLeft-1, t3);
	adaptiveSubdivide(patch, itrLeft-1, t4);
	adaptiveSubdivide(patch, itrLeft-1, t5);
	adaptiveSubdivide(patch, itrLeft-1, t6);
}

// Handles the case where one side of the triangle is flat.
//i is the indicator of the one flat side. ie p1-p2 is flat
void Bezier::oneSideFlat(Patch patch, int itrLeft, Ptriangle t, int i) {
	std::pair<float,float> p1 = t.getPoint(i);
	std::pair<float,float> p2 = t.getPoint((i+1)%3);
	std::pair<float,float> p3 = t.getPoint((i+2)%3);

	float midU1 = (p2.first + p3.first) * 0.5;
	float midV1 = (p2.second + p3.second) * 0.5;
	float midU2 = (p3.first + p1.first) * 0.5;
	float midV2 = (p3.second + p1.second) * 0.5;

	std::pair<float,float> x (midU1, midV1); //Unflat side midpoint1
	std::pair<float,float> y (midU2, midV2); //Unflat side midpoint2
	std::pair<float,float> a = p1;
	std::pair<float,float> b = p2;
	std::pair<float,float> c = p3; //Opposing the flat side

	float centerX = a.first + (2/3)*(a.first-x.first);
	float centerY = a.second + (2/3)*(a.second-x.second);
	std::pair<float,float> center (centerX,centerY);  //center of the triangle

	//Five triangle subdivisions.
	Ptriangle t1(a,center,b);
	Ptriangle t2(b,center,x);
	Ptriangle t3(x,center,c);
	Ptriangle t4(c,center,y);
	Ptriangle t5(y,center,a);

	adaptiveSubdivide(patch, itrLeft-1, t1);
	adaptiveSubdivide(patch, itrLeft-1, t2);
	adaptiveSubdivide(patch, itrLeft-1, t3);
	adaptiveSubdivide(patch, itrLeft-1, t4);
	adaptiveSubdivide(patch, itrLeft-1, t5);
}

//Handles the case where two sides of the triangle are flat.
// i is the indicatos the side not flat.
void Bezier::twoSideFlat(Patch patch, int itrLeft, Ptriangle t, int i) {
	std::pair<float,float> p1 = t.getPoint(i);
	std::pair<float,float> p2 = t.getPoint((i+1)%3);
	float midU = (p1.first + p2.first) * 0.5;
	float midV = (p1.second + p2.second) * 0.5;

	std::pair<float,float> x (midU, midV); //Unflat side midpoint
	std::pair<float,float> a = t.getPoint((i+2)%3);  //Opposing point to unflat side
	std::pair<float,float> b = t.getPoint((i)%3);
	std::pair<float,float> c = t.getPoint((i+1)%3); //Adjacent points to unflat side


	float centerX = a.first + (2/3)*(a.first-x.first);
	float centerY = a.second + (2/3)*(a.second-x.second);
	std::pair<float,float> center (centerX,centerY);  //center of the triangle

	//Four triangle subdivisions.
	Ptriangle t1(b,center,x);
	Ptriangle t2(x,center,c);
	Ptriangle t3(c,center,a);
	Ptriangle t4(a,center,b);

	adaptiveSubdivide(patch, itrLeft-1, t1);
	adaptiveSubdivide(patch, itrLeft-1, t2);
	adaptiveSubdivide(patch, itrLeft-1, t3);
	adaptiveSubdivide(patch, itrLeft-1, t4);
}

//Checks for whether the given triangle is beneath the flatBound
bool Bezier::isFlat(Patch patch, Ptriangle t, int side) {
	Point trash;
	std::pair<float,float> p1 = t.getPoint(side);
	std::pair<float,float> p2 = t.getPoint((side+1)%3);

	//Calculate midpoint of flat triangle.
	Point A = bezpatchinterp(patch, p1.first, p1.second, trash);
	Point B = bezpatchinterp(patch, p2.first, p2.second, trash);
	Point C = A * 0.5 + B * 0.5;

	//Calculate point on bez curve
	float midU = (p1.first + p2.first) * 0.5;
	float midV = (p1.second + p2.second) * 0.5;
	Point D = bezpatchinterp(patch, midU, midV, trash);

	if ((D - C).length() < flatBound) {
		return true;
	}
	return false;
}

//Checks for whether the patch is beneath the flatBound
bool Bezier::isFlat(Patch patch) {
	Point temp;
	Point pCurve = bezpatchinterp(patch, 0.5, 0.5, temp);

	Point p;
	Point dPdu;
	Point dPdv;
	Point trash;
	Point* vline[2];
	Point* uline[2];

	bool horizontal = true;
	bool vertical = false;

	// build end points for a flat line edge in v
	std::vector<Point> endPoints = patch.getEndPoints();
	p = findMidpoint(endPoints);

	Point distance = pCurve - p;
	if (distance.length() < flatBound) {
		return true;
	}
	return false;
}

//Finds the midpoint of a flat quad
Point Bezier::findMidpoint(std::vector<Point> quad) {
	Point A = quad[0] * 0.5 + quad[1] * 0.5;
	Point C = quad[2] * 0.5 + quad[3] * 0.5;

	Point E = A * 0.5 + C * 0.5;
	return E;
}

int Bezier::getTriangleNum() {
	return triangleNum;
}

Triangle Bezier::getTriangle(int i) {
	if (i < bezTriangles.size()) {
		return bezTriangles[i];
	} else {
		std::cout << "bezTriangles[" << i << "] access out of bounds. Max: " << bezTriangles.size() << "\n";
		exit(1);
	}
}
// //Returns a four element array of {end, mid, mid, end} of
// //the control points of the half-curve
// void Bezier::splitCurve(Point curve[], Point &half[], Point &otherHalf[]) {
// 	Point A = curve[0] * 0.5 + curve[1] * 0.5;
// 	Point B = curve[1] * 0.5 + curve[2] * 0.5;
// 	Point C = curve[2] * 0.5 + curve[3] * 0.5;
// 	// now, split AB and BC to form a new segment DE
// 	Point D = A * 0.5 + B * 0.5;
// 	Point E = B * 0.5 + C * 0.5;
// 	// finally, pick the right point on DE,
// 	// this is the point on the curve
// 	Point p = D * 0.5 + E * 0.5;
// 	// compute derivative also

// 	half[0] = curve[0];
// 	half[1] = A;
// 	half[2] = D;
// 	half[3] = p;

// 	otherHalf[0] = p;
// 	otherHalf[1] = E;
// 	otherhalf[2] = C;
// 	otherHalf[3] = curve[3];
// }

// #include <vector>
// #include <iostream>
// #include <fstream>
// #include <cmath>
// #include <stdlib.h>
// #include "Eigen/Dense"
// #include "bezier.h"
// #include "patch.h"
// #include "point.h"
// #include "quad.h"

// Bezier::Bezier() {
// 	num = 0;
// }

// // given the control points of a bezier curve
// // and a parametric value, return the curve 
// // point and derivative
// Point Bezier::bezcurveinterp(std::vector<Point> curve, float u, Point &dPdu) {
// 	// first, split each of the three segments
// 	// to form two new ones AB and BC
// 	Point A = curve[0] * (1.0-u) + curve[1] * u;
// 	Point B = curve[1] * (1.0-u) + curve[2] * u;
// 	Point C = curve[2] * (1.0-u) + curve[3] * u;
// 	// now, split AB and BC to form a new segment DE
// 	Point D = A * (1.0-u) + B * u;
// 	Point E = B * (1.0-u) + C * u;
// 	// finally, pick the right point on DE,
// 	// this is the point on the curve
// 	Point p = D * (1.0-u) + E * u;
// 	// compute derivative also
// 	dPdu = 3 * (E - D);
// 	return p;
// }

// Point Bezier::bezcurveinterp(Point curve[], float u, Point &dPdu) {
// 	// first, split each of the three segments
// 	// to form two new ones AB and BC
// 	Point A = curve[0] * (1.0-u) + curve[1] * u;
// 	Point B = curve[1] * (1.0-u) + curve[2] * u;
// 	Point C = curve[2] * (1.0-u) + curve[3] * u;
// 	// now, split AB and BC to form a new segment DE
// 	Point D = A * (1.0-u) + B * u;
// 	Point E = B * (1.0-u) + C * u;
// 	// finally, pick the right point on DE,
// 	// this is the point on the curve
// 	Point p = D * (1.0-u) + E * u;
// 	// compute derivative also
// 	dPdu = 3 * (E - D);
// 	return p;
// }

// // given a control patch and (u,v) values, find 
// // the surface point and normal
// Point Bezier::bezpatchinterp(Patch patch, float u, float v, Point &n) {
// 	Point p, dPdu, dPdv, trash;
// 	Point vcurve[4], ucurve[4];
// 	bool horizontal = true;
// 	bool vertical = false;

// 	// build control points for a Bezier curve in v
// 	vcurve[0] = bezcurveinterp(patch.getCurve(0, horizontal), u, trash);
// 	vcurve[1] = bezcurveinterp(patch.getCurve(1, horizontal), u, trash);
// 	vcurve[2] = bezcurveinterp(patch.getCurve(2, horizontal), u, trash);
// 	vcurve[3] = bezcurveinterp(patch.getCurve(3, horizontal), u, trash);
// 	// build control points for a Bezier curve in u
// 	ucurve[0] = bezcurveinterp(patch.getCurve(0, vertical), v, trash);
// 	ucurve[1] = bezcurveinterp(patch.getCurve(1, vertical), v, trash);
// 	ucurve[2] = bezcurveinterp(patch.getCurve(2, vertical), v, trash);
// 	ucurve[3] = bezcurveinterp(patch.getCurve(3, vertical), v, trash);
// 	// evaluate surface and derivative for u and v

// 	p = bezcurveinterp(vcurve, v, dPdv);
// 	p = bezcurveinterp(ucurve, u, dPdu);
// 	// take cross product of partials to find normal
// 	n = cross(dPdu, dPdv);
// 	n = n / n.getLength();
// 	p.saveNormal(n);
// 	return p;
// }

// // given a patch, perform uniform subdivision
// void Bezier::subdividepatch(Patch patch, float step) {
// 	// compute how many subdivisions there 
// 	// are for this step size
// 	int epsilon = 0;
// 	int numdiv = ((1 + epsilon)/ step);
// 	float u, v;
// 	Point p, n;

// 	bezPoints.clear();
// 	bezPoints.resize(numdiv*numdiv);

// 	// for each parametric value of u
// 	for (int iu = 0; iu <= numdiv; iu++) {
// 		u = iu * step;
// 		// for each parametric value of v
// 		for (int iv = 0; iv <= numdiv; iv++) {
// 			v = iv * step;
// 			// evaluate surface
// 			p = bezpatchinterp(patch, u, v, n);
// 			savesurfacepointandnormal(p);
// 		}
// 	}
// 	num = numdiv;
// }

// //  Returns cross product between two 3D vectors
// Point Bezier::cross(Point a, Point b) {
// 	float* valueA = a.getValues();
// 	float* valueB = b.getValues();
// 	Eigen::Vector3f vecA(valueA[0], valueA[1], valueA[2]);
// 	Eigen::Vector3f vecB(valueB[0], valueB[1], valueB[2]);
// 	Eigen::Vector3f result = vecA.cross(vecB);
// 	Point point(result[0], result[1], result[2]);
// 	return point;
// }

// void Bezier::savesurfacepointandnormal(Point p) {
// 	bezPoints.push_back(p);
// }

// void Bezier::constructQuads(float numdiv) {
// 	int cap = numdiv - 1; 
// 	bezQuads.clear();
// 	int pos1,pos2,pos3,pos4;
// 	for (int v = 0; v < cap; v++) {
// 		for (int u = 0; u < cap; u++) {
// 			pos1 = v*numdiv + u; 
// 			pos2 = v*numdiv + u+1;
// 			pos3 = (v+1)*numdiv + u+1;
// 			pos4 = (v+1)*numdiv + u;  //Points stored clockwise position 
// 			Quad quad;
// 			quad.addPoint(bezPoints[pos1]);
// 			quad.addPoint(bezPoints[pos2]);
// 			quad.addPoint(bezPoints[pos3]);
// 			quad.addPoint(bezPoints[pos4]);
// 			bezQuads.push_back(quad);
// 		}
// 	}
// }

// std::vector<Quad> Bezier::getQuads() {
// 	return bezQuads;
// }

// Point Bezier::getPoint(int position) {
// 	return bezPoints[position];
// }

// int Bezier::getNumdiv() {
// 	return num;
// }