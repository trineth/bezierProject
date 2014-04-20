#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include "ptriangle.h"
#include "point.h"

// class Ptriangle{
// 	int pointsStored;
// 	Point points[4];
// public:
	Ptriangle::Ptriangle() {
		pointsStored = 0;
	}

	Ptriangle::Ptriangle(std::pair<float,float> a, std::pair<float,float> b,
		std::pair<float,float> c) {
		pointsStored = 3;
		points[0] = a;
		points[1] = b;
		points[2] = c;
	}

	Ptriangle::Ptriangle( const Ptriangle& other ) {
		pointsStored = other.pointsStored;
		points[0] = other.points[0];
		points[1] = other.points[1];
		points[2] = other.points[2];
	}


	Ptriangle& Ptriangle::operator=(const Ptriangle& rhs) {
		this->pointsStored = rhs.pointsStored;
		this->points[0] = rhs.points[0];
		this->points[1] = rhs.points[1]; 
		this->points[2] = rhs.points[2];
	}

	void Ptriangle::addPoint(float x, float y) {
		if (pointsStored >= 3) {
			return;
		}
		std::pair<float,float> p(x,y);
		points[pointsStored] = p;
		pointsStored++;
	}

	void Ptriangle::addPoint(std::pair<float,float> p) {
		if (pointsStored >= 3) {
			return;
		}
		points[pointsStored] = p;
		pointsStored++;
	}

	std::pair<float,float>* Ptriangle::getPoints() {
		return points;
	}

	std::pair<float,float> Ptriangle::getPoint(int i) {
		return points[i];
	}