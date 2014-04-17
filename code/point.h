#ifndef ADD_H
#define ADD_H
 
class Point {
	float x;
	float y;
	float z;
public:
	Point() {}
	Point(float xx, float yy, float zz);

	Point(float *point[]);

	Point& operator+=(const Point& rhs);

 	friend Point operator+(Point lhs, const Point& rhs);
};

inline Point operator+(Point lhs, const Point& rhs);

#endif