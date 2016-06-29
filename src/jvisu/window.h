/**
 * Include file for window/viewport options
 */
#ifndef WINDOW_H
#define WINDOW_H

#include <list>
#include <string>
#include "sdl.h"
#include "layer.h"
#include "callback.h"


class CallbackManager;
class JEventCallback;
class Vector2f;

/*
 * Main window class
 */
class JWindow {
public:
	const bool hardwareAccelerated;
	
	CallbackManager callbackManager;
	
	JWindow(int sx, int sy, bool ha);
	~JWindow();
	int activate();
	int dispose();
	
	void update();
	
	
	bool isActive();
	int getScreenWidth();
	int getScreenHeight();
	float getAspectRatio();
	
	// Coordinate Transformations
	void viewportToScreen(float xin, float yin, int &xout, int &yout) const;
	void screenToViewport(int xin, int yin, float &xout, float &yout) const;
	
	
	SDL_PixelFormat *getFormat();
	SDL_Surface *createNewSurface();
	
	
	void addLayerTop(Layer *layer);
	void addLayerBottom(Layer *layer);
	void addLayerAt(Layer *layer, int position);
	void removeLayer(Layer *layer);
	Layer *getLayerById(std::string id);
	std::list<Layer*> getLayers();

private:
	SDL_Window *window;
	std::string windowName;
	
	int screenWidth, screenHeight;
	bool active;
	
	std::list<Layer*> layers;
	
	SDL_Renderer *renderer = NULL;
	
	bool registerLayer(Layer *layer);
	
	void refresh();
	void processInput();
	
};


#endif
