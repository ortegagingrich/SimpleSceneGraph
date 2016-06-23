/**
 * Include file for window/viewport options
 */
#ifndef WINDOW_H
#define WINDOW_H

#include <list>
#include <string>
#include <SDL2/SDL.h>



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

private:
	SDL_Window *window;
	std::string windowName;
	
	int screenWidth, screenHeight;
	bool needsRefresh, active;
	
	void refresh();
	void processInput();
	
};


#endif
