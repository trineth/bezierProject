#ifndef BEZIER_H
#define BEZIER_H

#include "patch.h"
#include "point.h"

Point bezcurveinterp(Point *curve[], float u, Point &dPdu);
Point bezpatchinterp(Patch patch, float u, float v, Point &n);

Point cross(Point a, Point b);
void savesurfacepointandnormal(Point p, float iu, float iv, int numdiv);
void constructQuads(float numdiv);

#endif