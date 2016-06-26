/**
 * Include file for window/viewport options
 */
#ifndef WINDOW_H
#define WINDOW_H

#include <list>
#include <string>
#include "sdl.h"
#include "scene_graph.h"



/*
 * Abstract Superclass for event callbacks
 */
class JWindow;

class JEventCallback {
public:
	JWindow *boundWindow;
	
	JEventCallback(JWindow *window);
	virtual ~JEventCallback();
	virtual void callback(SDL_Event event) = 0;
};


/*
 * Callback Manager for JWindows
 * (Note: implementation defined in callback.cpp)
 */
class CallbackManager {
public:
	CallbackManager();
	~CallbackManager();
	
	void registerCallback(JEventCallback *callback);
	void unregisterCallback(JEventCallback *callback);
	void processEvent(SDL_Event event);
private:
	// Registered Callbacks
	std::list<JEventCallback*> registered;
};


/*
 * Main window class
 */
class JWindow {
public:
	CallbackManager callbackManager;
	
	JWindow(int sx, int sy);
	~JWindow();
	int activate();
	int dispose();
	
	void update();
	
	
	bool isActive();
	
	int getScreenWidth();
	int getScreenHeight();
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
	
	SDL_Surface *buffer = NULL;
	
	void refresh();
	void processInput();
	
};


#endif
