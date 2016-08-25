#include <list>
#include "sdl.h"
#include "window.h"
#include "input.h"
#include "scene_graph.h"


/*
 * Source for Input Callbacks (defined in window.h)
 */

EventCallback::EventCallback(Window *window):
	boundManager(&(window->callbackManager)) 
{
	boundManager->registerCallback(this);
}

EventCallback::EventCallback(Layer *layer):
	boundManager(&(layer->callbackManager))
{
	boundManager->registerCallback(this);
}

EventCallback::EventCallback(Component2D *component):
	boundManager(&(component->callbackManager))
{
	boundManager->registerCallback(this);
}


EventCallback::~EventCallback(){
	if(boundManager != NULL) boundManager->unregisterCallback(this);
}

//Default pre-callback just checks to make sure the event is not consumed
void EventCallback::precallback(InputEvent *event, float tpf){
	if(!event->isConsumed()){
		callback(event, tpf);
	}
}


/*
 * Source for KeyButtonCallback
 */
KeyButtonCallback::KeyButtonCallback(Window *window): EventCallback(window){}
KeyButtonCallback::KeyButtonCallback(Layer *layer): EventCallback(layer){}
KeyButtonCallback::KeyButtonCallback(Component2D *component):
	EventCallback(component){}

void KeyButtonCallback::precallback(InputEvent *event, float tpf){
	if(event->getType() == "KEYBUTTON" && !event->isConsumed()){
		callback((KeyButtonEvent*) event, tpf);
	}
}



/*
 * Source for MouseButtonCallback
 */

MouseButtonCallback::MouseButtonCallback(Window *window): EventCallback(window){}
MouseButtonCallback::MouseButtonCallback(Layer *layer): EventCallback(layer){}
MouseButtonCallback::MouseButtonCallback(Component2D *component):
	EventCallback(component){}

void MouseButtonCallback::precallback(InputEvent *event, float tpf){
	if(event->getType() == "MOUSEBUTTON" && !event->isConsumed()){
		callback((MouseButtonEvent*) event, tpf);
	}
}



/*
 * Source for MouseMotionCallback
 */

MouseMotionCallback::MouseMotionCallback(Window *window): EventCallback(window){}
MouseMotionCallback::MouseMotionCallback(Layer *layer): EventCallback(layer){}
MouseMotionCallback::MouseMotionCallback(Component2D *component):
	EventCallback(component){}

void MouseMotionCallback::precallback(InputEvent *event, float tpf){
	if(event->getType() == "MOUSEMOTION" && !event->isConsumed()){
		callback((MouseMotionEvent*) event, tpf);
	}
}



/*
 * Source for QuitEventCallback
 */

QuitEventCallback::QuitEventCallback(Window *window): EventCallback(window){}
QuitEventCallback::QuitEventCallback(Layer *layer): EventCallback(layer){}
QuitEventCallback::QuitEventCallback(Component2D *component):
	EventCallback(component){}

void QuitEventCallback::precallback(InputEvent *event, float tpf){
	if(event->getType() == "QUIT" && !event->isConsumed()){
		callback((QuitEvent*) event, tpf);
	}
}



/*
 * Source for CallbackManager (defined in window.h)
 */

CallbackManager::CallbackManager(){}

CallbackManager::~CallbackManager(){
	while(!registered.empty()){
		delete registered.front();
	}
}


void CallbackManager::registerCallback(EventCallback *callback){
	registered.push_front(callback);
}

void CallbackManager::unregisterCallback(EventCallback *callback){
	registered.remove(callback);
}


void CallbackManager::processEvent(InputEvent *event, float tpf){
	/**
	 * Just passes the provided SDL_Event on to every registered callback.
	 */
	std::list<EventCallback*>::const_iterator i;
	for(i = registered.begin(); i != registered.end(); i++){
		(*i)->precallback(event, tpf);
	}
}

/*
 * Souce for TopCallbackManager
 */

void TopCallbackManager::processEvent(InputEvent *event, float tpf){
	//TODO: Take care of input logging, etc. here
	
	// Call super method
	CallbackManager::processEvent(event, tpf);
}



