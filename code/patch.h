#ifndef ADD_H
#define ADD_H
 
/**
* patchPoints notes:
* 	This is a 1D array containg the 3 dimensional point values of curves.
* 	The first curve will be the indices 0 to 11, the second from 12 to 24,
*   the third from 25 to 36,  and the fourth from 37 to 38.
*	Each subset of three values are the x, y, and z values of the respective point.
*/
class Patch {
	float patchPoints[48]; // 3 points per vertex.  4 vertices per curve. 4 curves.
public:
	Patch();

	addCurve(float *point1[3], float *point2[3], float *point3[3], float *point4[3]);


};

#endif