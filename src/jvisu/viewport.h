/*
 * Definitions and Declarations related to 2D and 3D viewports.
 * These are "owned" by Layer2D and Layer3D objects
 */
#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "vectormath.h"


class Layer2D;


//TODO: Implement all of this
class Viewport2D {
friend class Layer2D;
public:
	float centerX, centerY;
	float radiusX, radiusY;
	
	Viewport2D();
	
	void worldToViewport(float xin, float yin, float &xout, float &yout);
	void viewportToWorld(float xin, float yin, float &xout, float &yout);
	Vector2f worldToViewport(Vector2f in);
	Vector2f viewportToWorld(Vector2f in);
	
	bool inViewport(float x, float y);
	bool inViewport(Vector2f point);
	
	bool clipLineToViewport(Vector2f &point1, Vector2f &point2);
};



#endif


