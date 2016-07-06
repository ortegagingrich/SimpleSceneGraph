#include <cstdio>

#include "sdl.h"
#include "renderable.h"
#include "window.h"



/*
 * RenderableLine
 */

RenderableLine::RenderableLine(float xi1, float yi1, float xi2, float yi2, float z,
                               float w, Uint8 cr, Uint8 cg, Uint8 cb, Uint8 ca):
	Renderable(z),
	x1(xi1), x2(xi2), y1(yi1), y2(yi2),
	width(w),
	colorRed(cr),
	colorGreen(cg),
	colorBlue(cb),
	colorAlpha(ca)
{}


void RenderableLine::render(SDL_Renderer *renderer, JWindow *window){
	// Compute screen coordinates
	int px1, py1, px2, py2;
	window->viewportToScreen(x1, y1, px1, py1);
	window->viewportToScreen(x2, y2, px2, py2);
	
	// Draw Color
	SDL_SetRenderDrawColor(renderer, colorRed, colorGreen, colorBlue, colorAlpha);
	
	// TODO: Actually implement this part!!!
	printf("Pretending to draw line of width %f from (%f, %f) to (%f, %f) . . .",
	       width, x1, y1, x2, y2);
	printf("Actual Coordiantes: (%d, %d) -> (%d, %d)\n", px1, py1, px2, py2);
}



/*
 * RenderablePoint
 */

RenderablePoint::RenderablePoint(float x, float y, float z, float r,
                                Uint8 cr, Uint8 cg, Uint8 cb, Uint8 ca):
	Renderable(z),
	xPosition(x),
	yPosition(y),
	radius(r),
	colorRed(cr),
	colorGreen(cg),
	colorBlue(cb),
	colorAlpha(ca)
{}


void RenderablePoint::render(SDL_Renderer *renderer, JWindow *window){
	// First, compute the pixel coordinates of the point center
	int pixelX, pixelY;
	window->viewportToScreen(xPosition, yPosition, pixelX, pixelY);
	
	// Set render draw color
	SDL_SetRenderDrawColor(renderer, colorRed, colorGreen, colorBlue, colorAlpha);
	
	// TODO: Actually implement this part!!!
	printf("Pretending to draw point of radius %f at (%f, %f, %f) . . . ",
	       radius, xPosition, yPosition, zLevel);
	printf("Actual Coordinates: (%d, %d)\n", pixelX, pixelY);
}



/*
 * Helper Functions
 */

static bool compare_zlevel(const Renderable* a, const Renderable* b){
	return a->zLevel < b->zLevel;
}

void sort_renderables_by_z_level(std::list<Renderable*> &renderables){
	/**
	 * Sorts the provided list of renderables by their zLevel values
	 */
	renderables.sort(compare_zlevel);
}
