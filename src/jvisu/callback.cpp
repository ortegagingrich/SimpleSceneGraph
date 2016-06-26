#include <list>
#include "sdl.h"
#include "window.h"


/*
 * Source for Input Callbacks (defined in window.h)
 */

JEventCallback::JEventCallback(JWindow *window): boundWindow(window) {
	window->callbackManager.registerCallback(this);
}

JEventCallback::~JEventCallback(){
	boundWindow->callbackManager.unregisterCallback(this);
}


/*
 * Source for Callback Manager (defined in window.h)
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


void CallbackManager::processEvent(SDL_Event event){
	/**
	 * Just passes the provided SDL_Event on to every registered callback.
	 */
	for(std::list<JEventCallback*>::const_iterator i = registered.begin(); i != registered.end(); i++){
		(*i)->callback(event);
	}
}

