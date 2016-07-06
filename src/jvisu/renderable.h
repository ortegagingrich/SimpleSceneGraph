/*
 * Definitions and Declarations for renderable objects.
 * These are small lightweight objects which are created by some Layer sub-types
 * at render time.
 * 
 * Note that all x and y coordinates used by renderables are viewport (float)
 * coordinates.  These define (0,0) as the center of the screen.  The positive
 * x-direction is right and the positive y-direction is up.
 */
#ifndef RENDERABLE_H
#define RENDERABLE_H

#include <list>
#include "sdl.h"


class JWindow;

class Renderable {
	/**
	 * Abstract base class of renderables
	 */
public:
	const float zLevel;
	Renderable(float z): zLevel(z) {};
	virtual ~Renderable(){};
	virtual void render(SDL_Renderer *renderer, JWindow *window) = 0;
};


class RenderableLine : public Renderable {
	/**
	 * Class of renderable lines
	 */
public:
	const float x1, x2, y1, y2;
	const float width;
	const Uint8 colorRed, colorGreen, colorBlue, colorAlpha;
	
	RenderableLine(float xi1, float yi1, float xi2, float yi2, float z, float w,
	               Uint8 cr, Uint8 cg, Uint8 cb, Uint8 ca);
	
	virtual void render(SDL_Renderer *renderer, JWindow *window);
};


class RenderablePoint : public Renderable {
	/**
	 * Class of renderable points
	 */
public:
	const float xPosition, yPosition;
	const float radius;
	const Uint8 colorRed, colorGreen, colorBlue, colorAlpha;
	
	
	RenderablePoint(float x, float y, float z, float r,
	                Uint8 cr, Uint8 cg, Uint8 cb, Uint8 ca);
	
	virtual void render(SDL_Renderer *renderer, JWindow *window);
};


/*
 * Helper Functions
 */
void sort_renderables_by_z_level(std::list<Renderable*> &renderables);


#endif

