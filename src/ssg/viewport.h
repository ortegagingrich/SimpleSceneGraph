/*
 * Definitions and Declarations related to 2D and 3D viewports.
 * These are "owned" by Layer2D and Layer3D objects
 */
#ifndef VIEWPORT_H
#define VIEWPORT_H

#include "shared_exports.h"

#include "vectormath.h"
#include "geometry.h"


namespace ssg {

	class Layer2D;


	class SHARED_EXPORT Viewport2D {
	friend class Layer2D;
	public:
	
		Viewport2D();
	
		Vector2f getCenter() const;
		float getRadiusX() const;
		float getRadiusY() const;
		float getInverseRadiusX() const;
		float getInverseRadiusY() const;
		float getAspectRatio() const;
		bool isAspectRatioPreserved() const;
		bool isAspectRatioLocked() const;
	
	
		void setCenter(float x, float y);
		void setCenter(Vector2f center);
		void setRadiusX(float rx);
		void setRadiusY(float ry);
		void setAspectRatio(float newRatio);
	
		void setAspectRatioPreserved(bool preserved);  // Deprecated - should always be preserved
		void lockAspectRatio();  // Deprecated
		void unlockAspectRatio(); // Deprecated - should always be locked
	
	
		bool inViewport(float x, float y) const;
		bool inViewport(Vector2f point) const;
	
		void worldToViewport(float xin, float yin, float &xout, float &yout) const;
		void viewportToWorld(float xin, float yin, float &xout, float &yout) const;
		Vector2f worldToViewport(Vector2f in) const;
		Vector2f viewportToWorld(Vector2f in) const;
	
		Rect2f getWorldRect() const;
		Rect2f getViewportRect() const;

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

}

#endif


