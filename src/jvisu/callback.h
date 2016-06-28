#ifndef CALLBACK_H
#define CALLBACK_H

#include "sdl.h"

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


#endif


