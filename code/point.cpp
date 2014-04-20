#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include "Eigen/Dense"
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

	Point::Point( const Point& other ) {
		values[0] = other.values[0];
  		values[1] = other.values[1];
  		values[2] = other.values[2];
 		normal[0] = other.normal[0];
  		normal[1] = other.normal[1];
  		normal[2] = other.normal[2];
  		normal2[0] = other.normal2[0];
  		normal2[1] = other.normal2[1];
  		normal2[2] = other.normal2[2];
	}

	void Point::saveNormal(Point n, Point n2) {
		float* norm = n.getValues();
		normal[0] = norm[0];
		normal[1] = norm[1];
		normal[2] = norm[2];
		float* norm2 = n2.getValues();
		normal2[0] = norm2[0];
		normal2[1] = norm2[1];
		normal2[2] = norm2[2];
	}

	float* Point::getNormal(float x, float y, float z) {
		Eigen::Vector3f dir(values[0] - x, values[1] - y, values[2] - z);
		Eigen::Vector3f n1(normal[0], normal[1],normal[2]);
		Eigen::Vector3f n2(normal2[0], normal2[1],normal2[2]);
		float angle1 = n1.dot(dir);
		float angle2 = n2.dot(dir);
		if (angle1 > angle2) {
			return normal;
		} else {
			return normal2;
		}
		// return normal;
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
 		this->normal[0] = rhs.normal[0];
  		this->normal[1] = rhs.normal[1];
  		this->normal[2] = rhs.normal[2];
 		this->normal2[0] = rhs.normal2[0];
  		this->normal2[1] = rhs.normal2[1];
  		this->normal2[2] = rhs.normal2[2];
  		return *this;
 	}

 	// friend Point operator+(Point lhs, const Point& rhs);
 	// friend Point operator*(Point lhs, const float& rhs);
 	// friend Point operator*(const float& lhs, Point rhs);
 	// friend Point operator-(Point lhs, const Point& rhs);
// };