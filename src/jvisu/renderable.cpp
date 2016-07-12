#include <cstdio>
#include <string>

#include "sdl.h"
#include "renderable.h"
#include "window.h"
#include "geometry.h"



/*
 * RenderableLine
 */

RenderableLine *RenderableLine::createRenderableLine(float xi1, float yi1,
           float xi2, float yi2, float z, int w, Uint8 cr, Uint8 cg, Uint8 cb, Uint8 ca)
{
	// clip viewport coordinates to the actual viewport
	Rect2f rect(-1, 1, -1, 1);
	Line2f line(xi1, yi1, xi2, yi2);
	
	if(calculate_intersection(rect, line, line)){
		xi1 = line.startPoint.x;
		yi1 = line.startPoint.y;
		xi2 = line.endPoint.x;
		yi2 = line.endPoint.y;
		return new RenderableLine(xi1, yi1, xi2, yi2, z, w, cr, cg, cb, ca);
	}else{
		return NULL;
	}
}


RenderableLine::RenderableLine(float xi1, float yi1, float xi2, float yi2, float z,
                               int w, Uint8 cr, Uint8 cg, Uint8 cb, Uint8 ca):
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
	
	SDL_RenderDrawLine(renderer, px1, py1, px2, py2);
	
	// TODO: Actually implement this part!!!
	/*printf("Pretending to draw line of width %d from (%f, %f) to (%f, %f) . . .",
	       width, x1, y1, x2, y2);
	printf("Actual Coordinates: (%d, %d) -> (%d, %d)\n", px1, py1, px2, py2);*/
}



/*
 * RenderablePoint
 */
RenderablePoint *RenderablePoint::createRenderablePoint(float x, float y,
	                     float z, int w, Uint8 cr, Uint8 cg, Uint8 cb, Uint8 ca)
{
	// Make a RenderablePoint only if the provided coordinates are onscreen.
	if(calculate_intersection(Rect2f(-1, 1, -1, 1), Vector2f(x, y))){
		return new RenderablePoint(x, y, z, w, cr, cg, cb, ca);
	}else{
		return NULL;
	}
}


RenderablePoint::RenderablePoint(float x, float y, float z, int w,
                                Uint8 cr, Uint8 cg, Uint8 cb, Uint8 ca):
	Renderable(z),
	xPosition(x),
	yPosition(y),
	width(w),
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
	SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0x00);
	//SDL_SetRenderDrawColor(renderer, colorRed, colorGreen, colorBlue, colorAlpha);
	
	SDL_RenderDrawPoint(renderer, pixelX, pixelY);
}


/*
 * RenderableText
 */

RenderableText *RenderableText::createRenderableText(float xp, float yp, float z,
                                float xo, float yo, float r, float th, float ar,
                                          std::string t, std::string f){
	// TODO: Insert checks to see if it really is in the viewport
	return new RenderableText(xp, yp, z, xo, yo, r, th, ar, t, f);
}


RenderableText::RenderableText(float xp, float yp, float z, float xo, float yo,
                    float r, float th, float ar, std::string t, std::string f):
	Renderable(z),
	text(t),
	font(f),
	xPosition(xp),
	yPosition(yp),
	xOffset(xo),
	yOffset(yo),
	rotation(r),
	textHeight(th),
	aspectRatio(ar)
{}


void RenderableText::render(SDL_Renderer *renderer, JWindow *window){
	// TODO: Implement this
	printf("Pretending to render text: %s\n", text.c_str());
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
