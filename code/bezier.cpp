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

Bezier::Bezier() {
	std::vector<Point> bezPoints(5);
	std::vector<Quad> bezQuads(5);
	
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
	std::cout << "Numdiv: " << numdiv << "\n";

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

			Point p1 = bezPoints[pos1]; std::cout << p1.getValues()[0] << " " << p1.getValues()[1] << " " << p1.getValues()[2] << "\n";
			Point p2 = bezPoints[pos2]; std::cout << p2.getValues()[0] << " " << p2.getValues()[1] << " " << p2.getValues()[2] << "\n";
			Point p3 = bezPoints[pos3]; std::cout << p3.getValues()[0] << " " << p3.getValues()[1] << " " << p3.getValues()[2] << "\n";
			Point p4 = bezPoints[pos4]; std::cout << p4.getValues()[0] << " " << p4.getValues()[1] << " " << p4.getValues()[2] << "\n";

			Quad quad(bezPoints[pos1],bezPoints[pos2],bezPoints[pos3],bezPoints[pos4]); std::cout << "ax\n";
			bezQuads.push_back(quad); std::cout << "bx\n";
			quadNum++; std::cout << "cx\n";	
		}
	}
}

int Bezier::getQuadNum() {
	return quadNum;
}

Quad Bezier::getQuad(int i) {
	std::cout << "az\n";
	if (i < bezQuads.size()) {
		//printQuad();
		// Quad quad = bezQuads[i];
		// std::cout << i << " bz\n";
 	// 	return quad;
		return bezQuads[i];
	} else {
		std::cout << "cz\n";
		std::cout << "bezQuads[" << i << "] access out of bounds. Max: " << bezQuads.size() << "\n";
		exit(1);
	}
}

void Bezier::printQuad() {
	std::cout << "Size of bezQuads: " << bezQuads.size() << "\n";
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

/** Adaptive Subdivision Methods **/

// void Bezier::adaptiveExecute(Patch patch, float step) {
// 	if (step >= 1) {
// 		//render as is
// 	} else {
// 		//subdivide in half.  Break into triangles, call adaptiveSubdivide(), incr step
// 		Patch one;
// 		one.addCurve()
// 	}
// }

// void Bezier::adaptiveSubdivide(Patch patch, float step) {
// 	if (flatnessCheck) {
// 		//render as flat quad
// 	} else if (step >= 1) {
// 		//render as is
// 	} else {
// 		//subdivide more and call adaptiveSubdivide(). increment step
// 	}
// }

// void Bezier::divideToTriangle(Patch patch, Patch &one, Patch &two) {
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

// void Bezier::produceSubdivision(Patch patch, float u, float v) {
	
// }

// bool Bezier::flatnessCheck(Patch patch) {

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