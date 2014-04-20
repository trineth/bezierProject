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
		quadNum = 0;
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

	Quad::Quad( const Quad& other ) {
		pointsStored = other.pointsStored;
		subdivided = other.subdivided;
		points[0] = other.points[0];
		points[1] = other.points[1];
		points[2] = other.points[2];
		points[3] = other.points[3];
	}

	Quad& Quad::operator=(const Quad& rhs) {
		this->pointsStored = rhs.pointsStored;
		this->quadNum = rhs.quadNum; std::cout << "zz\n";
		this->subdivided = rhs.subdivided; std::cout << "zz\n";
		this->points[0] = rhs.points[0]; std::cout << "zz\n";
		this->points[1] = rhs.points[1]; std::cout << "zz\n"; 
		this->points[2] = rhs.points[2]; std::cout << "zz\n";
		this->points[3] = rhs.points[3]; std::cout << "zz\n";
		// for (int i = 0; i < rhs.quadNum; i++) { std::cout << "zz\n";
		// 	this->quads[i] = rhs.quads[i];
		// }
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