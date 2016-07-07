#include "geometry.h"
#include "vectormath.h"


/*
 * Rect2f
 */

Rect2f::Rect2f(float minx, float maxx, float miny, float maxy):
	xMin(minx),
	xMax(maxx),
	yMin(miny),
	yMax(maxy)
{}


Vector2f Rect2f::getCenter(){
	Vector2f out;
	out.x = 0.5 * (xMin + xMax);
	out.y = 0.5 * (yMin + yMax);
	return out;
}





