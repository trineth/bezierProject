#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include "point.h"

class Point {
	float x;
	float y;
	float z;
public:
	Point() {}
	Point(float xx, float yy, float zz) {
		x = xx;
		y = yy;
		z = zz;
	}

	Point(float *point[]) {
		x = point[0];
		y = point[1];
		z = point[2];
	}

	float* getValues() {
		float values[3] = {x,y,z};
		return values;
	}

	Point& operator+=(const Point& rhs) {
    	// actual addition of rhs to *this
  		this->x += rhs.x;
  		this->y += rhs.y;
  		this->z += rhs.z;
    	return *this;
 	}

 	Point& operator-=(const Point& rhs) {
    	// actual addition of rhs to *this
  		this->x -= rhs.x;
  		this->y -= rhs.y;
  		this->z -= rhs.z;
    	return *this;
 	}


 	Point& operator=(const Point& rhs) {
 		this->x = rhs.x;
  		this->y = rhs.y;
  		this->z = rhs.z;
  		return *this;
 	}

 	friend Point operator+(Point lhs, const Point& rhs);
 	friend Point operator*(Point lhs, const float& rhs);
 	friend Point operator*(const float& lhs, Point rhs);
 	friend Point operator-(Point lhs, const Point& rhs);
};

inline Point operator+(Point lhs, const Point& rhs) {
	lhs.x = lhs.x + rhs.x;
	lhs.y = lhs.y + rhs.y;
	lhs.z = lhs.z + rhs.z;

	return lhs;
}

inline Point operator-(Point lhs, const Point& rhs) {
	lhs.x = lhs.x - rhs.x;
	lhs.y = lhs.y - rhs.y;
	lhs.z = lhs.z - rhs.z;

	return lhs;
}

inline Point operator*(Point lhs, const float& rhs) {
	lhs.x = lhs.x * rhs;
	lhs.y = lhs.y * rhs;
	lhs.z = lhs.z * rhs;

	return lhs;
}

inline Point operator*(const float& lhs, Point rhs) {
	rhs.x = rhs.x * lhs;
	rhs.y = rhs.y * lhs;
	rhs.z = rhs.z * lhs;

	return rhs;
}
