#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdlib.h>
#include "patch.h"
#include "point.h"

/**
* patchPoints notes:
* 	This is a 1D array containg the 3 dimensional point values of curves.
* 	The first curve will be the indices 0 to 3, the second from 4 to 7,
*   the third from 8 to 11,  and the fourth from 12 to 15.
*	Each vertex contains the three x, y, and z values of the respective point.
*/
class Patch {
	int i; //Curves inputted, to a max of 4.
	Point patchPoints[16]; // 4 vertices per curve. 4 curves.
public:
	Patch() {
		i = 0;
	}

	void addCurve(Point point1, Point point2, Point point3, Point point4) {
		int j = i*4;

		patchPoints[j++] = point1;
		patchPoints[j++] = point2;
		patchPoints[j++] = point3;
		patchPoints[j] = point4;
		
		i++;
	}

	Point* getPoints() {
		return patchPoints;
	}
};