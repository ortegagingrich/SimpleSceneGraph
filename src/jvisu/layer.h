/**
 * Declarations of public-facing Layer classes
 */
#ifndef LAYER_H
#define LAYER_H

#include <list>
#include <string>
#include "sdl.h"


class JWindow;
class Node2D;
class Node3D;


// Abstract base class for layers
class Layer {
public:
	JWindow *window;
	const std::string id;
	
	Layer(JWindow *window, std::string id);
	virtual ~Layer();
	
	virtual void render(SDL_Renderer *renderer) = 0;
};


class LayerBackground: public Layer {
public:
	LayerBackground(JWindow *window, std::string id);
	LayerBackground(JWindow *window, std::string id, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
	LayerBackground(JWindow *window, std::string id, std::string imagePath);
	
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


class Layer2D: public Layer {
public:
	Node2D *root;
	
	Layer2D();
	~Layer2D();
	
	virtual void render(SDL_Renderer *renderer);
};


#endif
