/**
 * Declarations of public-facing scene graph classes
 */
#ifndef SCENE_GRAPH_H
#define SCENE_GRAPH_H

#include <list>
#include <string>
#include "sdl.h"


class JWindow;
class Node2D;
class Node3D;


// Abstract base class for layers
class Layer {
public:
	const std::string id;
	
	Layer(JWindow *window, std::string id);
	virtual ~Layer();
	
	virtual void render(SDL_Surface *surface) = 0;
protected:
	JWindow *window;
};


class LayerBackground: public Layer {
public:
	LayerBackground(JWindow *window, std::string id);
	LayerBackground(JWindow *window, std::string id, Uint8 red, Uint8 green, Uint8 blue);
	LayerBackground(JWindow *window, std::string id, std::string imagePath);
	
	virtual ~LayerBackground();
	
	virtual void render(SDL_Surface *surface);
	
	void setBackgroundImage(std::string path);
	void setBackgroundColor(Uint8 red, Uint8 green, Uint8 blue);
	
private:
	Uint8 backgroundColorRed, backgroundColorGreen, backgroundColorBlue;
	SDL_Surface *backgroundColorSurface = NULL;
	SDL_Surface *backgroundImageSurface = NULL;
};


class Layer2D: public Layer {
public:
	Node2D *root;
	
	~Layer2D();
	
	virtual void render(SDL_Surface *surface);
};


#endif
