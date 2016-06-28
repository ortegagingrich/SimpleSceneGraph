/*
 * Definitions and Declarations for renderable objects.
 * These are small lightweight objects which are created by some Layer sub-types
 * at render time.
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
	virtual ~Renderable();
	virtual void render(SDL_Renderer *renderer, JWindow *window) = 0;
};


/*
 * Helper Functions
 */
void sort_renderables_by_z_level(std::list<Renderable*> &renderables);


#endif

