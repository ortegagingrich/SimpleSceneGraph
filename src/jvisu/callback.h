#ifndef CALLBACK_H
#define CALLBACK_H

#include "sdl.h"

/*
 * Abstract Superclass for event callbacks
 */
class JWindow;
class Layer;
class ComponentInput2D;
class InputEvent;

class CallbackManager;


class JEventCallback {
friend class CallbackManager;
public:
	JEventCallback(JWindow *window);
	JEventCallback(Layer *layer);
	JEventCallback(ComponentInput2D *component);
	
	virtual ~JEventCallback();
	virtual void callback(InputEvent *event) = 0;
protected:
	CallbackManager *boundManager;
	
	virtual void precallback(InputEvent *event);
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
	void processEvent(InputEvent *event);
private:
	// Registered Callbacks
	std::list<JEventCallback*> registered;
};


#endif


