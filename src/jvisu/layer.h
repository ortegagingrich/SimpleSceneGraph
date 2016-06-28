/**
 * Declarations of public-facing Layer classes
 */
#ifndef LAYER_H
#define LAYER_H

#include <list>
#include <string>
#include "sdl.h"
#include "viewport.h"


class JWindow;
class Node2D;
class Node3D;
class Renderable;


// Abstract base class for layers
class Layer {
public:
	const std::string id;
	
	Layer(std::string id);
	virtual ~Layer();
	
	virtual void render(SDL_Renderer *renderer) = 0;
	
	JWindow *getWindow();
	void setWindow(JWindow *window);
	
protected:
	JWindow *window;
};


class LayerBackground: public Layer {
public:
	LayerBackground(std::string id);
	LayerBackground(std::string id, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	LayerBackground(std::string id, std::string imagePath);
	
	virtual ~LayerBackground();
	
	virtual void render(SDL_Renderer *renderer);
	
	void clearBackgroundImage();
	void setBackgroundImage(std::string path, Uint8 alpha);
	void setBackgroundColor(Uint8 red, Uint8 green, Uint8 blue, Uint8 alpha);
	
private:
	Uint8 backgroundColorRed, backgroundColorGreen, backgroundColorBlue, backgroundColorAlpha;
	Uint8 backgroundImageAlpha;
	SDL_Surface *backgroundImageSurface = NULL;
};


// Put Viewport Here!!! (It's a good idea, I promise.)
class Layer2D: public Layer {
public:
	Viewport2D viewport;
	
	Node2D *rootNode;
	
	Layer2D(std::string id);
	~Layer2D();
	
	virtual void render(SDL_Renderer *renderer);
private:
	std::list<Renderable*> renderables;
};


#endif
