#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include "triangle.h"
#include "point.h"

// class Triangle{
// 	int pointsStored;
// 	Point points[3];
// public:
	Triangle::Triangle() {
		pointsStored = 0;
	}

	Triangle::Triangle(Point one, Point two, Point three) {
		pointsStored = 3;
		points[0] = one;
		points[1] = two;
		points[2] = three;
	}

	Triangle::Triangle( const Triangle& other ) {
		pointsStored = other.pointsStored;
		points[0] = other.points[0];
		points[1] = other.points[1];
		points[2] = other.points[2];
	}

	Triangle& Triangle::operator=(const Triangle& rhs) {
		this->pointsStored = rhs.pointsStored;
		this->points[0] = rhs.points[0];
		this->points[1] = rhs.points[1]; 
		this->points[2] = rhs.points[2];
	}

	void Triangle::addPoint(Point p) {
		if (pointsStored >= 3) {
			return;
		}
		points[pointsStored] = p;
		pointsStored++;
	}

	Point* Triangle::getPoints() {
		return points;
	}

	Point Triangle::getPoint(int i) {
		return points[i];
	}