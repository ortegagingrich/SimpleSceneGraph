#ifndef CALLBACK_H
#define CALLBACK_H

#include "shared_exports.h"

#include "sdl.h"

namespace ssg {


	/*
	 * Abstract Superclass for event callbacks
	 */
	class Window;
	class Layer;
	class Component2D;
	class InputEvent;
	class QuitEvent;
	class KeyButtonEvent;
	class MouseButtonEvent;
	class MouseMotionEvent;

	class CallbackManager;


	class SHARED_EXPORT EventCallback {
	friend class CallbackManager;
	public:
	
	
		EventCallback(): boundManager(NULL) {};
		EventCallback(Window *window);
		EventCallback(Layer *layer);
		EventCallback(Component2D *component);
	
		virtual ~EventCallback();
		virtual void callback(InputEvent *event, float tpf) = 0;
	protected:
		CallbackManager *boundManager;
	
		virtual void precallback(InputEvent *event, float tpf);
	};


	class SHARED_EXPORT KeyButtonCallback : public EventCallback {
	public:
		KeyButtonCallback(Window *window);
		KeyButtonCallback(Layer *layer);
		KeyButtonCallback(Component2D *component);
	
		// Dummy Method; should never be called
		virtual void callback(InputEvent *event, float tpf){};
		virtual void callback(KeyButtonEvent *event, float tpf) = 0;
	protected:
		virtual void precallback(InputEvent *event, float tpf);
	};


	class SHARED_EXPORT MouseButtonCallback : public EventCallback {
	public:
		MouseButtonCallback(Window *window);
		MouseButtonCallback(Layer *layer);
		MouseButtonCallback(Component2D *component);
	
		// Dummy Method; should never be called
		virtual void callback(InputEvent *event, float tpf){};
		virtual void callback(MouseButtonEvent *event, float tpf) = 0;
	protected:
		virtual void precallback(InputEvent *event, float tpf);
	};


	class SHARED_EXPORT MouseMotionCallback : public EventCallback {
	public:
		MouseMotionCallback(Window *window);
		MouseMotionCallback(Layer *layer);
		MouseMotionCallback(Component2D *component);
	
		// Dummy Method; should never be called
		virtual void callback(InputEvent *event, float tpf){};
		virtual void callback(MouseMotionEvent *event, float tpf) = 0;
	protected:
		virtual void precallback(InputEvent *event, float tpf);
	};


	class SHARED_EXPORT QuitEventCallback : public EventCallback {
	public:
		QuitEventCallback(Window *window);
		QuitEventCallback(Layer *layer);
		QuitEventCallback(Component2D *component);
	
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
	
		void registerCallback(EventCallback *callback);
		void unregisterCallback(EventCallback *callback);
		
	internal:
		virtual void processEvent(InputEvent *event, float tpf);
	private:
		// Registered Callbacks
		std::list<EventCallback*> registered;
	};

	/*
	 * Special subclass of callback manager specifically for windows.
	 * TopCallbackManager are also responsible for logging input for multi-event
	 * phenomena (like double-clicking or dragging.)
	 */
	class SHARED_EXPORT TopCallbackManager: public CallbackManager {
	friend class MouseButtonEvent;
	internal:
		virtual void processEvent(InputEvent *event, float tpf);
	private:
		// Variables used to log input events
	
	
	};
}

#endif


