#include <list>
#include "sdl.h"
#include "window.h"
#include "input.h"
#include "scene_graph.h"


/*
 * Source for Input Callbacks (defined in window.h)
 */

JEventCallback::JEventCallback(JWindow *window):
	boundManager(&(window->callbackManager)) 
{
	boundManager->registerCallback(this);
}

JEventCallback::JEventCallback(Layer *layer):
	boundManager(&(layer->callbackManager))
{
	boundManager->registerCallback(this);
}

JEventCallback::JEventCallback(ComponentInput2D *component):
	boundManager(&(component->callbackManager))
{
	boundManager->registerCallback(this);
}


JEventCallback::~JEventCallback(){
	boundManager->unregisterCallback(this);
}

//Default pre-callback just checks to make sure the event is not consumed
void JEventCallback::precallback(InputEvent *event){
	if(!event->isConsumed()){
		callback(event);
	}
}


/*
 * Source for QuitEventCallback
 */

QuitEventCallback::QuitEventCallback(JWindow *window): JEventCallback(window){}
QuitEventCallback::QuitEventCallback(Layer *layer): JEventCallback(layer){}
QuitEventCallback::QuitEventCallback(ComponentInput2D *component):
	JEventCallback(component){}

void QuitEventCallback::precallback(InputEvent *event){
	if(event->getType() == "QUIT" && !event->isConsumed()){
		callback((QuitEvent*) event);
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


void CallbackManager::processEvent(InputEvent *event){
	/**
	 * Just passes the provided SDL_Event on to every registered callback.
	 */
	std::list<JEventCallback*>::const_iterator i;
	for(i = registered.begin(); i != registered.end(); i++){
		(*i)->precallback(event);
	}
}

/*
 * Souce for TopCallbackManager
 */

void TopCallbackManager::processEvent(InputEvent *event){
	//TODO: Take care of input logging, etc. here
	
	// Call super method
	CallbackManager::processEvent(event);
}



