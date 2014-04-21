#ifndef PTRIANGLE_H
#define PTRIANGLE_H

#include "point.h"

//Stands for parametric triangle, holding the u,v values
// of the endpoints in relation to a particular patch.
class Ptriangle{
	int pointsStored;
	std::pair<float,float> points[3];
public:
	Ptriangle();
	Ptriangle(float x0, float y0, float x1, float y1, float x2, float y2);
	Ptriangle(std::pair<float,float> a, std::pair<float,float> b, std::pair<float,float> c);

	Ptriangle( const Ptriangle& other );

	Ptriangle& operator=(const Ptriangle& rhs);

	void addPoint(float x, float y);
	void addPoint(std::pair<float,float> p);
	std::pair<float,float>* getPoints();
	std::pair<float,float> getPoint(int i);
};

#endif