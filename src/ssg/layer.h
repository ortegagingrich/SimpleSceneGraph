/**
 * Declarations of public-facing Layer classes
 */
#ifndef LAYER_H
#define LAYER_H

#include "shared_exports.h"

#include <list>
#include <string>
#include "sdl.h"
#include "viewport.h"
#include "callback.h"
#include "button_manager.h"


namespace ssg {

	class Window;
	class InputEvent;
	class Node2D;
	class NodeRoot2D;
	class Renderable;


	// Abstract base class for layers
	class SHARED_EXPORT Layer {
	public:
		const std::string id;
		CallbackManager callbackManager;
	
		Layer(std::string id);
		virtual ~Layer();
		
	
	internal:
	
		virtual void update(float tpf){};
		virtual void render(SDL_Renderer *renderer) = 0;
		virtual void processEvent(InputEvent *event, float tpf);
	
	public:
	
		Window *getWindow();
		void setWindow(Window *window);
	
	protected:
		Window *window;
	};


	class SHARED_EXPORT LayerBackground: public Layer {
	public:
		LayerBackground(std::string id);
		LayerBackground(std::string id, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
		LayerBackground(std::string id, std::string imagePath);
	
		virtual ~LayerBackground();
		
	internal:
		virtual void render(SDL_Renderer *renderer);
		
	public:
		void clearBackgroundImage();
		void setBackgroundImage(std::string path, Uint8 alpha);
		void setBackgroundColor(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha);
	
	private:
		Uint8 backgroundColorRed, backgroundColorGreen, backgroundColorBlue, backgroundColorAlpha;
		Uint8 backgroundImageAlpha;
		SDL_Surface *backgroundImageSurface = NULL;
	};


	// Put Viewport Here!!! (It's a good idea, I promise.)
	class SHARED_EXPORT Layer2D: public Layer {
	public:
		Viewport2D viewport;
		ButtonManager buttonManager;
	
	
		Layer2D(std::string id);
		~Layer2D();
		
	internal:
		virtual void update(float tpf);
		virtual void render(SDL_Renderer *renderer);
		virtual void processEvent(InputEvent *event, float tpf);
		
	public:
		Node2D *getRootNode();
	
	private:
		NodeRoot2D *rootNode;
	
		std::list<Renderable*> renderables;
	};
}

#endif
