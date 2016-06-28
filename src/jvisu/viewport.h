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
	
	Viewport2D();
	
	Vector2f getCenter();
	float getRadiusX();
	float getRadiusY();
	float getAspectRatio();
	bool isAspectRatioPreserved();
	bool isAspectRatioLocked();
	
	
	void setCenter(float x, float y);
	void setCenter(Vector2f center);
	void setRadiusX(float rx);
	void setRadiusY(float ry);
	void setAspectRatio(float newRatio);
	
	void setAspectRatioPreserved(bool preserved);
	void lockAspectRatio();
	void unlockAspectRatio();
	
	
	bool inViewport(float x, float y);
	bool inViewport(Vector2f point);
	
	void worldToViewport(float xin, float yin, float &xout, float &yout);
	void viewportToWorld(float xin, float yin, float &xout, float &yout);
	Vector2f worldToViewport(Vector2f in);
	Vector2f viewportToWorld(Vector2f in);

protected:
	void forceAspectRatio(float newRatio);
private:
	float centerX, centerY;
	float radiusX, radiusY;
	float inverseRadiusX, inverseRadiusY;
	float minX, minY, maxX, maxY;
	bool aspectPreserved;
	bool aspectLocked;
	bool scaleY;
	
	void setRadii(float rx, float ry);
};



#endif


