#include <vector>
#include <iostream>
#include <fstream>
#include <cmath>
#include <stdlib.h>

/**
* patchPoints notes:
* 	This is a 1D array containg the 3 dimensional point values of curves.
* 	The first curve will be the indices 0 to 11, the second from 12 to 24,
*   the third from 25 to 36,  and the fourth from 37 to 38.
*	Each subset of three values are the x, y, and z values of the respective point.
*/
class Patch {
	int i; //Curves inputted, to a max of 4.
	float patchPoints[48]; // 3 points per vertex.  4 vertices per curve. 4 curves.
public:
	Patch() {
		i = 0;
	}

	addCurve(float *point1[3], float *point2[3], float *point3[3], float *point4[3]) {
		int j = i*12;

		patchPoints[j++] = point1[0];
		patchPoints[j++] = point1[1];
		patchPoints[j++] = point1[2];

		patchPoints[j++] = point2[0];
		patchPoints[j++] = point2[1];
		patchPoints[j++] = point2[2];

		patchPoints[j++] = point3[0];
		patchPoints[j++] = point3[1];
		patchPoints[j++] = point3[2];
		
		patchPoints[j++] = point4[0];
		patchPoints[j++] = point4[1];
		patchPoints[j++] = point4[2];
		i++;
	}


};