#include <cmath>
#include <cstdio>
#include <string>

#include "sdl.h"
#include "renderable.h"
#include "window.h"
#include "vectormath.h"
#include "geometry.h"
#include "texture.h"


using namespace jvisu;


/*
 * RenderableLine
 */

RenderableLine *RenderableLine::createRenderableLine(
	float xi1,
	float yi1,
	float xi2,
	float yi2,
	float z,
	int w,
	Uint8 cr,
	Uint8 cg,
	Uint8 cb,
	Uint8 ca,
	Rect2f cullRect
){
	// clip viewport coordinates to the actual viewport
	Line2f line(xi1, yi1, xi2, yi2);
	
	if(calculate_intersection(cullRect, line, line)){
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


void RenderableLine::render(SDL_Renderer *renderer, Window *window){
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
RenderablePoint *RenderablePoint::createRenderablePoint(
	float x,
	float y,
	float z,
	int w,
	Uint8 cr,
	Uint8 cg,
	Uint8 cb,
	Uint8 ca,
	Rect2f cullRect
){
	// Make a RenderablePoint only if the provided coordinates are onscreen.
	if(calculate_intersection(cullRect, Vector2f(x, y))){
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


void RenderablePoint::render(SDL_Renderer *renderer, Window *window){
	// First, compute the pixel coordinates of the point center
	int pixelX, pixelY;
	window->viewportToScreen(xPosition, yPosition, pixelX, pixelY);
	
	// Set render draw color
	SDL_SetRenderDrawColor(renderer, 0xff, 0x00, 0x00, 0x00);
	//SDL_SetRenderDrawColor(renderer, colorRed, colorGreen, colorBlue, colorAlpha);
	
	SDL_RenderDrawPoint(renderer, pixelX, pixelY);
}


/*
 * RenderableSprite
 */

// Helper function for culling sprites
static bool shouldCullSprite(
	float x,
	float y,
	float w,
	float h,
	float r,
	Rect2f cullRect
){
	/**
	 * Checks to see if the provided rotated rectangle should be culled given
	 * the provided cullRect.
	 */
	Rect2f checkRect;
	if(r == 0 && false){// Temporary: always use more conservative estimate
		// No rotation, so use the actual sprite rectangle
		checkRect.set(x, x + w, y, y + h);
	}else{
		// There is rotation, so do a conservative heuristic rectangle
		float maxdist = Vector2f(w, h).norm();
		checkRect.set(x - maxdist, x + maxdist, y - maxdist, y + maxdist);
	}
	
	// If there is no intersection, do not make a renderable
	return !calculate_intersection(checkRect, cullRect);
}


RenderableSprite *RenderableSprite::createRenderableSprite(
	float x,
	float y,
	float w,
	float h,
	float z,
	float r,
	Texture *tex,
	Rect2f cullRect
){
	if(tex == NULL){
		return NULL;	
	}
	if(!tex->isLoaded()){
		return NULL;
	}
	
	// Quick check to make sure the sprite is onscreen
	if(shouldCullSprite(x, y, w, h, r, cullRect)) return NULL;
	
	// Otherwise, make the renderable
	return new RenderableSprite(x, y, w, h, z, r, tex);
}


RenderableSprite::RenderableSprite(float x, float y, float w, float h, float z,
float r, Texture *tex):
	Renderable(z),
	xPosition(x),
	yPosition(y),
	width(w),
	height(h),
	rotation(r),
	texture(tex)
{}


void RenderableSprite::render(SDL_Renderer *renderer, Window *window){
	SDL_Texture *sdlTexture = texture->getSdlTexture();
	float deg = rotation * RAD_2_DEG;
	
	SDL_Rect dstrect;
	window->viewportToScreen(xPosition, yPosition, dstrect.x, dstrect.y);
	dstrect.w = round(0.5 * window->getScreenHeight() * width);
	dstrect.h = round(0.5 * window->getScreenHeight() * height); 
	
	//dstrect.w = 50;
	//dstrect.h = 50;
	
	
	render_copy_clip(renderer, sdlTexture, NULL, &dstrect, -deg);
	
	
	/*SDL_Point center;
	center.x = 0;
	center.y = 0;
	SDL_RendererFlip flip = SDL_FLIP_NONE;*/
	//SDL_RenderCopy(renderer, sdlTexture, NULL, &dstrect);
	//SDL_RenderCopyEx(renderer, sdlTexture, NULL, &dstrect, -deg, &center, flip);
}


/*
 * RenderableSpriteFixed
 */

RenderableSpriteFixed *RenderableSpriteFixed::createRenderableSpriteFixed(
	float xp,
	float yp,
	int xo,
	int yo,
	float z,
	Texture *tex,
	Rect2f cullRect
){
	if(tex == NULL){
		return NULL;	
	}
	if(!tex->isLoaded()){
		return NULL;
	}
	
	//TODO: Cannot Cull images without knowledge of window;
	
	// Otherwise, make the renderable
	return new RenderableSpriteFixed(xp, yp, xo, yo, z, tex);
}


RenderableSpriteFixed::RenderableSpriteFixed(
	float xp,
	float yp,
	int xo,
	int yo,
	float z,
	Texture *tex
):
	Renderable(z),
	xPosition(xp),
	yPosition(yp),
	xOffset(xo),
	yOffset(yo),
	width(tex->width),
	height(tex->height),
	texture(tex)
{}


void RenderableSpriteFixed::render(SDL_Renderer *renderer, Window *window){
	
	SDL_Texture *sdlTexture = texture->getSdlTexture();
	
	SDL_Rect dstrect;
	window->viewportToScreen(xPosition, yPosition, dstrect.x, dstrect.y);
	dstrect.x -= xOffset;
	dstrect.y -= yOffset; 
	dstrect.w = width;
	dstrect.h = height;
	
	/*
	 * Since we were unable to cull the sprites of this type at creation-time,
	 * we must decide now if we are to render them or not.
	 */
	Rect2f checkRect, cullRect;
	cullRect.xMin = 0;
	cullRect.yMin = 0;
	cullRect.xMax = window->getScreenWidth();
	cullRect.yMax = window->getScreenHeight();
	
	checkRect.xMin = dstrect.x;
	checkRect.yMin = dstrect.y;
	checkRect.xMax = dstrect.x + width;
	checkRect.yMax = dstrect.y + height;
	
	if(!calculate_intersection(checkRect, cullRect)) return;
	
	
	SDL_RenderCopy(renderer, sdlTexture, NULL, &dstrect);
}
	


/*
 * Helper Functions
 */

static bool compare_zlevel(const Renderable* a, const Renderable* b){
	if(a->zLevel == b->zLevel) return a->zMod < b->zMod;
	return a->zLevel < b->zLevel;
}

void sort_renderables_by_z_level(std::list<Renderable*> &renderables){
	/**
	 * Sorts the provided list of renderables by their zLevel values
	 */
	renderables.sort(compare_zlevel);
}

