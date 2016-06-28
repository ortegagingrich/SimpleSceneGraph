/*
 * Source Implementation of Viewports
 */
#include <cstdio>
#include "viewport.h"
#include "vectormath.h"


/*
 * Source for Viewport2D
 */

Viewport2D::Viewport2D():
	centerX(0.0f),
	centerY(0.0f),
	aspectPreserved(false),
	aspectLocked(true),
	scaleY(false)
{
	setRadii(1.0f, 1.0f);
}


Vector2f Viewport2D::getCenter(){return Vector2f(centerX, centerY);}
float Viewport2D::getRadiusX(){return radiusX;}
float Viewport2D::getRadiusY(){return radiusY;}
float Viewport2D::getAspectRatio(){return radiusX / radiusY;}
bool Viewport2D::isAspectRatioPreserved(){return aspectPreserved;}
bool Viewport2D::isAspectRatioLocked(){return aspectLocked;}


void Viewport2D::setCenter(float x, float y){
	centerX = x;
	centerY = y;
	
	// Just to make sure pre-computed values are updated appropriately
	setRadii(radiusX, radiusY);
}

void Viewport2D::setCenter(Vector2f center){
	setCenter(center.x, center.y);
}

void Viewport2D::setRadiusX(float rx){
	if(!(rx > 0)){
		printf("Cannot give a viewport an x-radius of \"%f\".\n", rx);
		return;
	}
	
	scaleY = true;
	
	float ry;
	if(aspectPreserved){
		ry = rx / getAspectRatio();
	}else{
		ry = radiusY;
	}
	setRadii(rx, ry);
}

void Viewport2D::setRadiusY(float ry){
	if(!(ry > 0)){
		printf("Cannot give a viewport a y-radius of \"%f\".\n", ry);
		return;
	}
	
	scaleY = false;
	
	float rx;
	if(aspectPreserved){
		rx = ry * getAspectRatio();
	}else{
		rx = radiusX;
	}
	setRadii(rx, ry);
}

// Private method that everything changing radii eventually calls
void Viewport2D::setRadii(float rx, float ry){
	radiusX = rx;
	radiusY = ry;
	inverseRadiusX = 1.0 / rx;
	inverseRadiusY = 1.0 / ry;
	
	minX = centerX - radiusX;
	maxX = centerX + radiusX;
	minY = centerY - radiusY;
	maxY = centerY + radiusY;
}

void Viewport2D::setAspectRatio(float newRatio){
	if(aspectLocked){
		printf("Cannot set viewport aspect ratio.  ");
		printf("This viewport's aspect ratio is locked to its Window.\n");
	}else{
		forceAspectRatio(newRatio);
	}
}
void Viewport2D::forceAspectRatio(float newRatio){
	if(!(newRatio > 0)){
		printf("Cannot give a viewport an aspect ratio of \"%f\".\n", newRatio);
		return;
	}
	
	if(scaleY){ // Preserve radiusX and scale radiusY to the new aspect ratio.
		setRadii(radiusX, radiusX / newRatio);
	}else{ // Preserve radiusY and scale radiusX to the new aspect ratio.
		setRadii(radiusY * newRatio, radiusY);
	}
}


void Viewport2D::setAspectRatioPreserved(bool preserved){
	aspectPreserved = preserved;
}
void Viewport2D::lockAspectRatio(){aspectLocked = true;}
void Viewport2D::unlockAspectRatio(){aspectLocked = false;}


bool Viewport2D::inViewport(Vector2f point){
	return inViewport(point.x, point.y);
}
bool Viewport2D::inViewport(float x, float y){
	return x >= minX && x <= maxX && y >= minY && y <= maxY;
}


void Viewport2D::worldToViewport(float xin, float yin, float &xout, float &yout){
	Vector2f in(xin, yin);
	Vector2f out = worldToViewport(in);
	xout = out.x;
	yout = out.y;
}

void Viewport2D::viewportToWorld(float xin, float yin, float &xout, float &yout){
	Vector2f in(xin, yin);
	Vector2f out = viewportToWorld(in);
	xout = out.x;
	yout = out.y;
}

Vector2f Viewport2D::worldToViewport(Vector2f in){
	Vector2f offset = in - getCenter();
	offset.scale(inverseRadiusX, inverseRadiusY);
	
	return offset;
}

Vector2f Viewport2D::viewportToWorld(Vector2f in){
	in.scale(radiusX, radiusY);
	return in + getCenter();
}


