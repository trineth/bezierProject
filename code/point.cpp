#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include "point.h"

// class Point {
// 	float x;
// 	float y;
// 	float z;
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

	float* Point::getValues() {
		return values;
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
  		return *this;
 	}

 	// friend Point operator+(Point lhs, const Point& rhs);
 	// friend Point operator*(Point lhs, const float& rhs);
 	// friend Point operator*(const float& lhs, Point rhs);
 	// friend Point operator-(Point lhs, const Point& rhs);
// };