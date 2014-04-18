#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include "bezier.h"
#include "patch.h"
#include "point.h"

// given the control points of a bezier curve
// and a parametric value, return the curve 
// point and derivative
Point bezcurveinterp(Point curve[], float u, Point &dPdu) {
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
Point bezpatchinterp(Patch patch, float u, float v, Point &n) {
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
	n = n / length(n);
	return p;
}

// // given a patch, perform uniform subdivision
// void subdividepatch(Patch patch, float step) {
// 	// compute how many subdivisions there 
// 	// are for this step size
// 	numdiv = ((1 + epsilon)/ step);
// 	// for each parametric value of u
// 	for (iu = 0 to numdiv);
// 	u = iu * step;
// 	// for each parametric value of v
// 	for (iv = 0 to numdiv);
// 	v = iv * step;
// 	// evaluate surface
// 	Point n;
// 	p = bezpatchinterp(patch, u, v, n);
// 	savesurfacepointandnormal(p,n);
// }

// };