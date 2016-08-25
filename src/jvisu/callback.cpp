#include <list>
#include "sdl.h"
#include "window.h"
#include "input.h"
#include "scene_graph.h"


/*
 * Source for Input Callbacks (defined in window.h)
 */

JEventCallback::JEventCallback(Window *window):
	boundManager(&(window->callbackManager)) 
{
	boundManager->registerCallback(this);
}

JEventCallback::JEventCallback(Layer *layer):
	boundManager(&(layer->callbackManager))
{
	boundManager->registerCallback(this);
}

JEventCallback::JEventCallback(Component2D *component):
	boundManager(&(component->callbackManager))
{
	boundManager->registerCallback(this);
}


JEventCallback::~JEventCallback(){
	if(boundManager != NULL) boundManager->unregisterCallback(this);
}

//Default pre-callback just checks to make sure the event is not consumed
void JEventCallback::precallback(InputEvent *event, float tpf){
	if(!event->isConsumed()){
		callback(event, tpf);
	}
}


/*
 * Source for KeyButtonCallback
 */
KeyButtonCallback::KeyButtonCallback(Window *window): JEventCallback(window){}
KeyButtonCallback::KeyButtonCallback(Layer *layer): JEventCallback(layer){}
KeyButtonCallback::KeyButtonCallback(Component2D *component):
	JEventCallback(component){}

void KeyButtonCallback::precallback(InputEvent *event, float tpf){
	if(event->getType() == "KEYBUTTON" && !event->isConsumed()){
		callback((KeyButtonEvent*) event, tpf);
	}
}



/*
 * Source for MouseButtonCallback
 */

MouseButtonCallback::MouseButtonCallback(Window *window): JEventCallback(window){}
MouseButtonCallback::MouseButtonCallback(Layer *layer): JEventCallback(layer){}
MouseButtonCallback::MouseButtonCallback(Component2D *component):
	JEventCallback(component){}

void MouseButtonCallback::precallback(InputEvent *event, float tpf){
	if(event->getType() == "MOUSEBUTTON" && !event->isConsumed()){
		callback((MouseButtonEvent*) event, tpf);
	}
}



/*
 * Source for MouseMotionCallback
 */

MouseMotionCallback::MouseMotionCallback(Window *window): JEventCallback(window){}
MouseMotionCallback::MouseMotionCallback(Layer *layer): JEventCallback(layer){}
MouseMotionCallback::MouseMotionCallback(Component2D *component):
	JEventCallback(component){}

void MouseMotionCallback::precallback(InputEvent *event, float tpf){
	if(event->getType() == "MOUSEMOTION" && !event->isConsumed()){
		callback((MouseMotionEvent*) event, tpf);
	}
}



/*
 * Source for QuitEventCallback
 */

QuitEventCallback::QuitEventCallback(Window *window): JEventCallback(window){}
QuitEventCallback::QuitEventCallback(Layer *layer): JEventCallback(layer){}
QuitEventCallback::QuitEventCallback(Component2D *component):
	JEventCallback(component){}

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


void CallbackManager::registerCallback(JEventCallback *callback){
	registered.push_front(callback);
}

void CallbackManager::unregisterCallback(JEventCallback *callback){
	registered.remove(callback);
}


void CallbackManager::processEvent(InputEvent *event, float tpf){
	/**
	 * Just passes the provided SDL_Event on to every registered callback.
	 */
	std::list<JEventCallback*>::const_iterator i;
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



