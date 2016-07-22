#ifndef CALLBACK_H
#define CALLBACK_H

#include "shared_exports.h"

#include "sdl.h"

/*
 * Abstract Superclass for event callbacks
 */
class JWindow;
class Layer;
class ComponentInput2D;
class InputEvent;
class QuitEvent;
class KeyButtonEvent;
class MouseButtonEvent;
class MouseMotionEvent;

class CallbackManager;


class SHARED_EXPORT JEventCallback {
friend class CallbackManager;
public:
	JEventCallback(JWindow *window);
	JEventCallback(Layer *layer);
	JEventCallback(ComponentInput2D *component);
	
	virtual ~JEventCallback();
	virtual void callback(InputEvent *event, float tpf) = 0;
protected:
	CallbackManager *boundManager;
	
	virtual void precallback(InputEvent *event, float tpf);
};


class SHARED_EXPORT KeyButtonCallback : public JEventCallback {
public:
	KeyButtonCallback(JWindow *window);
	KeyButtonCallback(Layer *layer);
	KeyButtonCallback(ComponentInput2D *component);
	
	// Dummy Method; should never be called
	virtual void callback(InputEvent *event, float tpf){};
	virtual void callback(KeyButtonEvent *event, float tpf) = 0;
protected:
	virtual void precallback(InputEvent *event, float tpf);
};


class SHARED_EXPORT MouseButtonCallback : public JEventCallback {
public:
	MouseButtonCallback(JWindow *window);
	MouseButtonCallback(Layer *layer);
	MouseButtonCallback(ComponentInput2D *component);
	
	// Dummy Method; should never be called
	virtual void callback(InputEvent *event, float tpf){};
	virtual void callback(MouseButtonEvent *event, float tpf) = 0;
protected:
	virtual void precallback(InputEvent *event, float tpf);
};


class SHARED_EXPORT MouseMotionCallback : public JEventCallback {
public:
	MouseMotionCallback(JWindow *window);
	MouseMotionCallback(Layer *layer);
	MouseMotionCallback(ComponentInput2D *component);
	
	// Dummy Method; should never be called
	virtual void callback(InputEvent *event, float tpf){};
	virtual void callback(MouseMotionEvent *event, float tpf) = 0;
protected:
	virtual void precallback(InputEvent *event, float tpf);
};


class SHARED_EXPORT QuitEventCallback : public JEventCallback {
public:
	QuitEventCallback(JWindow *window);
	QuitEventCallback(Layer *layer);
	QuitEventCallback(ComponentInput2D *component);
	
	// Dummy Method; should never be called
	virtual void callback(InputEvent *event, float tpf){};
	virtual void callback(QuitEvent *event, float tpf) = 0;
protected:
	virtual void precallback(InputEvent *event, float tpf);
};


/*
 * Base class for Callback Managers
 * (Note: implementation defined in callback.cpp)
 */
class SHARED_EXPORT CallbackManager {
public:
	CallbackManager();
	~CallbackManager();
	
	void registerCallback(JEventCallback *callback);
	void unregisterCallback(JEventCallback *callback);
	virtual void processEvent(InputEvent *event, float tpf);
private:
	// Registered Callbacks
	std::list<JEventCallback*> registered;
};

/*
 * Special subclass of callback manager specifically for windows.
 * TopCallbackManager are also responsible for logging input for multi-event
 * phenomena (like double-clicking or dragging.)
 */
class SHARED_EXPORT TopCallbackManager: public CallbackManager {
friend class MouseButtonEvent;
public:
	virtual void processEvent(InputEvent *event, float tpf);
private:
	// Variables used to log input events
	
	
};


#endif


