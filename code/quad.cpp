#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include "quad.h"
#include "point.h"

// class Quad{
// 	int pointsStored;
// 	bool subdivided;
// 	Point points[4];
// 	Quad* quads;
// public:
	Quad::Quad() {
		subdivided = false;
		pointsStored = 0;
	}

	Quad::Quad(Point one, Point two, Point three, Point four) {
		subdivided = false;
		pointsStored = 4;
		points[0] = one;
		points[1] = two;
		points[2] = three;
		points[3] = four;
	}

	void Quad::addPoint(Point p) {
		if (pointsStored >= 4) {
			return;
		}
		points[pointsStored] = p;
		pointsStored++;
	}

	bool Quad::isSubdivided() {
		return subdivided;
	}

	Point* Quad::getPoints() {
		return points;
	}