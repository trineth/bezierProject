#ifndef POINT_H
#define POINT_H
 
class Point {
	float x;
	float y;
	float z;
public:
	Point() {}
	Point(float xx, float yy, float zz);

	Point(float *point[]);

	float* getValues();

	Point& operator+=(const Point& rhs);
	Point& operator-=(const Point& rhs);
	Point& operator=(const Point& rhs);

 	friend Point operator+(Point lhs, const Point& rhs);
 	friend Point operator*(Point lhs, const float& rhs);
 	friend Point operator*(const float& lhs, Point rhs);
 	friend Point operator-(Point lhs, const Point& rhs);
};

inline Point operator+(Point lhs, const Point& rhs);
inline Point operator-(Point lhs, const Point& rhs);
inline Point operator*(Point lhs, const float& rhs);
inline Point operator*(const float& lhs, Point rhs);

#endif