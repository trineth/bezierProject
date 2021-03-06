#ifndef POINT_H
#define POINT_H
 
class Point {
	float values[3];
	float normal[3];
	float normal2[3];
public:
	Point() {}
	Point(float xx, float yy, float zz);

	Point(float point[]);
	Point( const Point& other );

	void saveNormal(Point n, Point n2);
	float* getNormal(float x, float y, float z);
	float* getValues();
	float length();

	Point& operator+=(const Point& rhs);
	Point& operator-=(const Point& rhs);
	Point& operator=(const Point& rhs);

 	friend Point operator+(Point lhs, const Point& rhs);
 	friend Point operator-(Point lhs, const Point& rhs);
 	friend Point operator*(Point lhs, const float& rhs);
 	friend Point operator*(const float& lhs, Point rhs);
 	friend Point operator/(Point lhs, const float& rhs);
};

inline Point operator+(Point lhs, const Point& rhs) {
	lhs.values[0] = lhs.values[0] + rhs.values[0];
	lhs.values[1] = lhs.values[1] + rhs.values[1];
	lhs.values[2] = lhs.values[2] + rhs.values[2];

	return lhs;
}

inline Point operator-(Point lhs, const Point& rhs) {
	lhs.values[0] = lhs.values[0] - rhs.values[0];
	lhs.values[1] = lhs.values[1] - rhs.values[1];
	lhs.values[2] = lhs.values[2] - rhs.values[2];

	return lhs;
}

inline Point operator*(Point lhs, const float& rhs) {
	lhs.values[0] = lhs.values[0] * rhs;
	lhs.values[1] = lhs.values[1] * rhs;
	lhs.values[2] = lhs.values[2] * rhs;

	return lhs;
}

inline Point operator*(const float& lhs, Point rhs) {
	rhs.values[0] = rhs.values[0] * lhs;
	rhs.values[1] = rhs.values[1] * lhs;
	rhs.values[2] = rhs.values[2] * lhs;

	return rhs;
}

inline Point operator/(Point lhs, const float& rhs) {
	lhs.values[0] = lhs.values[0] / rhs;
	lhs.values[1] = lhs.values[1] / rhs;
	lhs.values[2] = lhs.values[2] / rhs;

	return lhs;
}

#endif