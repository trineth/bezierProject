#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include "point.h"

// class Point {
// 	float values[3];
//	Point normal;
// public:
	Point::Point(float xx, float yy, float zz) {
		values[0] = xx;
		values[1] = yy;
		values[2] = zz;
	}

	Point::Point(float point[]) {
		values[0]= point[0];
		values[1] = point[1];
		values[2] = point[2];
	}

	void Point::saveNormal(Point n) {
		normal = n;
	}

	Point Point::normal() {
		return normal;
	}

	float* Point::getValues() {
		return values;
	}

	float Point::length() {
		float sum = std::pow(values[0], 2) + std::pow(values[1], 2) + std::pow(values[2], 2);
		float mag = std::sqrt(sum);
		return mag;
	}

	Point& Point::operator+=(const Point& rhs) {
    	// actual addition of rhs to *this
  		this->values[0] += rhs.values[0];
  		this->values[1] += rhs.values[1];
  		this->values[2] += rhs.values[2];
    	return *this;
 	}

 	Point& Point::operator-=(const Point& rhs) {
    	// actual addition of rhs to *this
  		this->values[0] -= rhs.values[0];
  		this->values[1] -= rhs.values[1];
  		this->values[2] -= rhs.values[2];
    	return *this;
 	}


 	Point& Point::operator=(const Point& rhs) {
 		this->values[0] = rhs.values[0];
  		this->values[1] = rhs.values[1];
  		this->values[2] = rhs.values[2];
  		this->normal = rhs.normal;
  		return *this;
 	}

 	// friend Point operator+(Point lhs, const Point& rhs);
 	// friend Point operator*(Point lhs, const float& rhs);
 	// friend Point operator*(const float& lhs, Point rhs);
 	// friend Point operator-(Point lhs, const Point& rhs);
// };