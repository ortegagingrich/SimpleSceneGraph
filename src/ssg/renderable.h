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

#include <string>
#include <list>

#include "shared_exports.h"

#include "sdl.h"
#include "geometry.h"


namespace ssg {

	class Window;
	class Texture;


	class Renderable {
		/**
		 * Abstract base class of renderables
		 */
	public:
		const float zLevel;
		float zMod;  // Used to distinguish between identical z-Levels
	
		virtual ~Renderable(){};
	
		virtual std::string getType() const {return "Renderable";};
	
		virtual void render(SDL_Renderer *renderer, Window *window) = 0;

	protected:
		Renderable(float z): zLevel(z), zMod(0.0f) {};
	};


	class RenderableLine : public Renderable {
		/**
		 * Class of renderable lines
		 */
	public:
		static RenderableLine *createRenderableLine(
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
		);
	
		const float x1, x2, y1, y2;
		const int width;
		const Uint8 colorRed, colorGreen, colorBlue, colorAlpha;
	
		virtual std::string getType() const {return "RenderableLine";};
	
		virtual void render(SDL_Renderer *renderer, Window *window);
	
	protected:
		RenderableLine(float xi1, float yi1, float xi2, float yi2, float z, int w,
			           Uint8 cr, Uint8 cg, Uint8 cb, Uint8 ca);
	};


	class RenderablePoint : public Renderable {
		/**
		 * Class of renderable points
		 */
	public:
		static RenderablePoint *createRenderablePoint(
			float x,
			float y,
			float z,
			int w,
			Uint8 cr,
			Uint8 cg,
			Uint8 cb,
			Uint8 ca,
			Rect2f cullRect
		);
	
		const float xPosition, yPosition;
		const int width;
		const Uint8 colorRed, colorGreen, colorBlue, colorAlpha;
	
		virtual std::string getType() const {return "RenderablePoint";};
	
		virtual void render(SDL_Renderer *renderer, Window *window);
	
	protected:
		RenderablePoint(float x, float y, float z, int w,
			            Uint8 cr, Uint8 cg, Uint8 cb, Uint8 ca);
	};



	class RenderableSprite : public Renderable {
		/**
		 * Class of renderable images.
		 */
	public:
		static RenderableSprite *createRenderableSprite(
			float xp,
			float yp,
			float w,
			float h,
			float z,
			float r,
			Texture *tex,
			Rect2f cullRect
		);
	
	
		const float xPosition, yPosition; // Viewport Coordinates
		const float width, height;  // Viewport Coordinates
		const float rotation;
	
		const Texture *texture;
	
		virtual std::string getType() const {return "RenderableSprite";};
	
		virtual void render(SDL_Renderer *renderer, Window *window);

	protected:
		RenderableSprite(float xp, float yp, float w, float h, float z, float r,
			             Texture *tex);
	};



	class RenderableSpriteFixed : public Renderable {
		/**
		 * Class of renderable images fixed to pixels.  These can be neither scaled
		 * nor rotated.
		 */
	public:
		static RenderableSpriteFixed *createRenderableSpriteFixed(
			float xp,
			float yp,
			int xo,
			int yo,
			float z,
			Texture *tex,
			Rect2f cullRect
		);
	
		// All in Viewport Coordinates
		const float xPosition, yPosition;
		const int xOffset, yOffset;
		const int width, height;
	
		const Texture *texture;
	
		virtual std::string getType() const {return "RenderableSpriteFixed";};
	
		virtual void render(SDL_Renderer *renderer, Window *window);
	
	protected:
		RenderableSpriteFixed(float xp, float yp, int xo, int yo, float z, Texture *tex);
	};



	/*
	 * Helper Functions
	 */
	void sort_renderables_by_z_level(std::list<Renderable*> &renderables);
}

#endif

