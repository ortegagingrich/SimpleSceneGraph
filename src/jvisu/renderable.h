/*
 * Definitions and Declarations for renderable objects.
 * These are small lightweight objects which are created by some Layer sub-types
 * at render time.
 * 
 * Note that all x and y coordinates used by renderables are viewport (float)
 * coordinates.  These define (0,0) as the center of the screen.  The positive
 * x-direction is right and the positive y-direction is up.
 * 
 * Also note that all renderables have protected constructors.  They should be
 * created only using static "factory" methods which ensure that the created
 * renderables are indeed valid and return NULL in situations where the creation
 * of valid renderables is impossible.
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
	
	virtual ~Renderable(){};
	virtual void render(SDL_Renderer *renderer, JWindow *window) = 0;

protected:
	Renderable(float z): zLevel(z) {};
};


class RenderableLine : public Renderable {
	/**
	 * Class of renderable lines
	 */
public:
	static RenderableLine *createRenderableLine(float xi1, float yi1, float xi2,
	       float yi2, float z, int w, Uint8 cr, Uint8 cg, Uint8 cb, Uint8 ca);
	
	const float x1, x2, y1, y2;
	const int width;
	const Uint8 colorRed, colorGreen, colorBlue, colorAlpha;
	
	virtual void render(SDL_Renderer *renderer, JWindow *window);
	
protected:
	RenderableLine(float xi1, float yi1, float xi2, float yi2, float z, int w,
	               Uint8 cr, Uint8 cg, Uint8 cb, Uint8 ca);
};


class RenderablePoint : public Renderable {
	/**
	 * Class of renderable points
	 */
public:
	static RenderablePoint *createRenderablePoint(float x, float y, float z,
	                          int w, Uint8 cr, Uint8 cg, Uint8 cb, Uint8 ca);
	
	const float xPosition, yPosition;
	const int width;
	const Uint8 colorRed, colorGreen, colorBlue, colorAlpha;
	
	virtual void render(SDL_Renderer *renderer, JWindow *window);
	
protected:
	RenderablePoint(float x, float y, float z, int w,
	                Uint8 cr, Uint8 cg, Uint8 cb, Uint8 ca);
};


/*
 * Helper Functions
 */
void sort_renderables_by_z_level(std::list<Renderable*> &renderables);


#endif

