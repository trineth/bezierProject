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

// given the control points of a bezier curve
// and a parametric value, return the curve 
// point and derivative
Point Bezier::bezcurveinterp(Point curve[], float u, Point &dPdu) {
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
	Point vcurve[4];
	Point ucurve[4];

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
	n = cross(dPdu, dPdv);
	n = n / n.length();
	p.saveNormal(n);
	return p;
}

// given a patch, perform uniform subdivision
void Bezier::subdividepatch(Patch patch, float step) {
	// compute how many subdivisions there 
	// are for this step size
	int epsilon = 0;
	int numdiv = ((1 + epsilon)/ step);
	float u, v;
	Point p, n;

	bezPoints.clear();
	bezPoints.resize(numdiv*numdiv);

	// for each parametric value of u
	for (int iu = 0; iu < numdiv; iu++) {
		u = iu * step;
		// for each parametric value of v
		for (int iv = 0; iv < numdiv; iv++) {
			v = iv * step;
			// evaluate surface
			p = bezpatchinterp(patch, u, v, n);
			savesurfacepointandnormal(p,iu,iv,numdiv);
		}
	}
	constructQuads(numdiv);
}

//  Returns cross product between two 3D vectors
Point Bezier::cross(Point a, Point b) {
	float* valueA = a.getValues();
	float* valuaB = b.getValues();
	Eigen::Vector3f vecA(valueA[0], valueA[1], valueA[2]);
	Eigen::Vector3f vecB(valueB[0], valueB[1], valueB[2]);
	Eigen::Vector3f result = vecA.cross(vec2);
	Point point(result.[0], result[1], result[2]);
	return point;
}

void Bezier::savesurfacepointandnormal(Point p, float iu, float iv, int numdiv) {
	bezPoints[position] = p;
}

void Bezier::constructQuads(float numdiv) {
	int v = 0;
	int position;
	while (v < numdiv-1) {
		int u = 0;
		while (u < numdiv-1) {
			pos1 = v*numdiv + u;
			pos2 = v*numdiv + u+1;
			pos3 = (v+1)*numdiv + u+1;
			pos4 = (v+1)*numdiv + u;  //Points stored clockwise position
			Quad quad(bezPoints[pos1],bezPoints[pos2],bezPoints[pos3],bezPoints[pos4]);
			bezQuads.push_back(quad);
			u++;
		}
		v++;
	}
}