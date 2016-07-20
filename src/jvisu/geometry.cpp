#include <cstdio>
#include "geometry.h"
#include "vectormath.h"
#include "utils.h"


/*
 * Line2f
 */

Line2f::Line2f(Vector2f s, Vector2f e): startPoint(s), endPoint(e){}
Line2f::Line2f(float xs, float ys, float xe, float ye){
	set(xs, ys, xe, ye);
}


Vector2f Line2f::getMidpoint() const {
	return 0.5 * (startPoint + endPoint);
}


void Line2f::set(float xs, float ys, float xe, float ye){
	startPoint.x = xs;
	startPoint.y = ys;
	endPoint.x = xe;
	endPoint.y = ye;
}


bool Line2f::operator==(Line2f line) const {
	if(startPoint == line.startPoint){
		return endPoint == line.endPoint;
	}else if(endPoint == line.startPoint){
		return startPoint == line.endPoint;
	}
	return false;
}


/*
 * Rect2f
 */

Rect2f Rect2f::boundPoints(Vector2f points[], int point_count){
	/**
	 * Constructs and returns a rectangle bounding all of the provided points.
	 */
	Rect2f bound;
	
	if(point_count < 1) return bound;
	
	bound.set(points[0].x, points[0].x, points[0].y, points[0].y);
	for(int i = 0; i < point_count; i++){
		Vector2f p = points[i];
		
		bound.xMin = p.x < bound.xMin ? p.x : bound.xMin;
		bound.xMax = p.x > bound.xMax ? p.x : bound.xMax;
		bound.yMin = p.y < bound.yMin ? p.y : bound.yMin;
		bound.yMax = p.y > bound.yMax ? p.y : bound.yMax;
	}
	
	
	return bound;
}


Rect2f::Rect2f(): xMin(0), xMax(0), yMin(0), yMax(0) {}


Rect2f::Rect2f(float minx, float maxx, float miny, float maxy):
	xMin(minx),
	xMax(maxx),
	yMin(miny),
	yMax(maxy)
{}

Rect2f::Rect2f(Vector2f point1, Vector2f point2){
	xMin = point1.x < point2.x ? point1.x : point2.x;
	xMax = point1.x > point2.x ? point1.x : point2.x;
	yMin = point1.y < point2.y ? point1.y : point2.y;
	yMax = point1.y > point2.y ? point1.y : point2.y;
}


Vector2f Rect2f::getCenter() const {
	Vector2f out;
	out.x = 0.5 * (xMin + xMax);
	out.y = 0.5 * (yMin + yMax);
	return out;
}

float Rect2f::getHeight() const {
	return xMax - xMin;
}

float Rect2f::getWidth() const {
	return yMax - yMin;
}


void Rect2f::set(float minx, float maxx, float miny, float maxy){
	xMin = minx;
	xMax = maxx;
	yMin = miny;
	yMax = maxy;
}


bool Rect2f::operator==(Rect2f rect) const {
	return xMax == rect.xMax && xMin == rect.xMin && yMax == rect.yMax && yMin == rect.yMin;
}


/*
 * Intersection Methods
 */

bool calculate_intersection(Line2f l1, Line2f l2){
	/**
	 * Checks to see if the two line segments intersect at a SINGLE point.  If
	 * the line segments are collinear, false is always returned!
	 */
	Vector2f dummy;
	return calculate_intersection(l1, l2, dummy);
}

bool calculate_intersection(Line2f l1, Line2f l2, Vector2f &out){
	/**
	 * Checks to see if the two line segments intersect at a SINGLE point.  If
	 * the line segments are collinear, false is always returned!  If a single
	 * unique point of intersection exists, the provided vector is overwritten
	 * with its coordinates, regardless of whether that point is on both segments
	 * or beyond their ranges.
	 */
	Vector2f p, q, r, s;
	p = l1.startPoint;
	q = l2.startPoint;
	r = l1.endPoint - p;
	s = l2.endPoint - q;
	
	// Some intermediate quantities
	Vector2f diff = q - p;
	float rxs = r.cross(s);
	
	// Parallel if rxs is zero
	if(rxs == 0) return false;
	
	// Scalars t, u so that p + t*r = q + u*s
	float t = diff.cross(s) / rxs;
	float u = diff.cross(r) / rxs;
	
	// Point of intersection
	out = p + t * r;
	
	// Check to see if the point of intersection is in the proper range
	if(t >= 0 && t <= 1 && u >= 0 && u <= 1){
		return true;
	}else{
		return false;
	}
}

bool calculate_intersection(Rect2f rect, Vector2f point){
	float x = point.x;
	float y = point.y;
	return x >= rect.xMin && x <= rect.xMax && y >= rect.yMin && y <= rect.yMax;
}

bool calculate_intersection(Rect2f rect, Line2f line){
	Line2f dummy;
	return calculate_intersection(rect, line, dummy);
}

bool calculate_intersection(Rect2f rect, Line2f line, Line2f &out){
	if(cohen_sutherland(line, rect)){
		out = line;
		return true;
	}
	return false;
}

bool calculate_intersection(Rect2f r1, Rect2f r2){
	Rect2f dummy;
	return calculate_intersection(r1, r2, dummy);
}

bool calculate_intersection(Rect2f r1, Rect2f r2, Rect2f &out){
	float newMinX, newMinY, newMaxX, newMaxY;
	newMinX = r1.xMin > r2.xMin ? r1.xMin : r2.xMin;
	newMaxX = r1.xMax < r2.xMax ? r1.xMax : r2.xMax;
	newMinY = r1.yMin > r2.yMin ? r1.yMin : r2.yMin;
	newMaxY = r1.yMax < r2.yMax ? r1.yMax : r2.yMax;
	
	// Check for consistency
	if(newMinX > newMaxX || newMinY > newMaxY) return false;
	
	out.xMin = newMinX;
	out.xMax = newMaxX;
	out.yMin = newMinY;
	out.yMax = newMaxY;
	return true;
}



