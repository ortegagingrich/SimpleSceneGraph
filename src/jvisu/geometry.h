#ifndef GEOMETRY_H
#define GEOMETRY_H

#include "shared_exports.h"

#include "vectormath.h"


class SHARED_EXPORT Line2f {
public:
	Vector2f startPoint, endPoint;
	
	Line2f(){};
	Line2f(Vector2f s, Vector2f e);
	Line2f(float xs, float ys, float xe, float ye);
	
	Vector2f getMidpoint() const;
	
	void set(float xs, float ys, float xe, float ye);
	
	bool operator==(Line2f line) const;
};


class SHARED_EXPORT Rect2f {
public:
	float xMin, xMax;
	float yMin, yMax;
	
	Rect2f();
	Rect2f(float minx, float maxx, float miny, float maxy);
	Rect2f(Vector2f point1, Vector2f point2);
	
	Vector2f getCenter() const;
	
	void set(float minx, float maxx, float miny, float maxy);
	
	bool operator==(Rect2f rect) const;
};


/*
 * Intersection Methods
 */

bool calculate_intersection(Line2f l, Vector2f v, float tolerance); //TODO
bool calculate_intersection(Line2f l1, Line2f l2);                  //TODO
bool calculate_intersection(Line2f l1, Line2f l2, Vector2f &out);   //TODO
bool calculate_intersection(Rect2f r, Vector2f v);                  //TODO
bool calculate_intersection(Rect2f r, Line2f l);                    //TODO
bool calculate_intersection(Rect2f r, Line2f l, Line2f &out);       //TODO
bool calculate_intersection(Rect2f r1, Rect2f r2);                  //TODO
bool calculate_intersection(Rect2f r1, Rect2f r2, Rect2f &out);     //TODO



#endif
