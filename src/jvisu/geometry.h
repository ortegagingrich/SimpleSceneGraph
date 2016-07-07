#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "shared_exports.h"

#include "vectormath.h"


class SHARED_EXPORT Line2f {
public:
	Vector2f startPoint, endPoint;
	
	Line2f(Vector2f s, Vector2f e);
	Line2f(float xs, float ys, float xe, float ye);
};


class SHARED_EXPORT Rect2f {
public:
	float xMin, xMax;
	float yMin, yMax;
	
	Rect2f(float minx, float maxx, float miny, float maxy);
	
	Vector2f getCenter();
};





#endif
