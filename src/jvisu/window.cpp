/**
 * Contains the implementation of jvisu windows.
 */
#include <cstdio>
#include <cmath>
#include <list>
#include <string>

#include "sdl.h"
#include "window.h"
#include "layer.h"
#include "callback.h"
#include "input.h"
#include "vectormath.h"



/*
 * Constructors
 */

JWindow::JWindow(int sx, int sy, bool ha):
	hardwareAccelerated(ha),
	screenWidth(sx),
	screenHeight(sy),
	active(false) {

	windowName = "jvisu";
	activate();
	
	// Add a background layer
	
	LayerBackground *background = new LayerBackground("background");
	background->clearBackgroundImage();
	addLayerBottom(background);
	
}


int JWindow::activate(){
	/**
	 * Creates SDL components of the window.
	 */
	if(active){
		printf("Window \"%s\" already active.\n", windowName.c_str());
		return -1;
	}
	
	int result = create_SDL_window(&window, windowName.c_str(), screenWidth, screenHeight);
	
	if(result < 0) return -1;
	
	active = true;
	return 0;
}


/*
 * Destructors
 */
JWindow::~JWindow(){
	if(active) dispose();
}


int JWindow::dispose(){
	/**
	 * This method disposes of all SDL components of the window, but leaves the
	 * object in place
	 */
	if(!active){
		printf("Cannot Dispose inactive window.\n");
		return -1;
	}
	
	// Remove all layers
	while(!layers.empty()){
		Layer *layer = layers.front();
		printf("Deleting Layer \"%s\" from Window \"%s\"\n", layer->id.c_str(), 
		        windowName.c_str());
		// Note: This automatically removes the layer from the list.
		delete layer;
	}
	
	if(renderer != NULL){
		SDL_DestroyRenderer(renderer);
		renderer = NULL;
	}
	
	remove_SDL_window(window);
	active = false;
	
	return 0;
}


/*
 * Main Update Methods
 */
void JWindow::update(){
	/**
	 * This method should be called once per frame.
	 */
	if(!active){
		printf("Updating an inactive JWindow.\n");
		return;
	}
	
	
	//TODO: Develop a System of checking if refreshes are really needed.
	refresh();
	
	
	// Note: Processing of input should be the final step of the update cycle,
	//       as this may result in the end of the event.
	processInput();
}


void JWindow::refresh(){
	/**
	 * This method re-draws everything to the SDL surface.  Although only called
	 * when it is needed, in practice, this method is generally called every frame.
	 */
	
	// Reset or Create renderer
	if(renderer == NULL){
		renderer = create_SDL_renderer(window, (int) hardwareAccelerated);
	}
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
	SDL_RenderClear(renderer);
	
	
	// Fill in the buffer by rendering the layers in order
	std::list<Layer*>::iterator iter;
	for(iter = layers.begin(); iter != layers.end(); iter++){
		Layer *layer = *iter;
		layer->render(renderer);
	}
	
	
	// Actually render the screen now
	SDL_RenderPresent(renderer);
}


void JWindow::processInput(){
	/**
	 * Checks for and processes all SDL input events and calls the appropriate
	 * callbacks.
	 */
	SDL_Event sdlEvent;
	
	// Cycle through all active events
	while(SDL_PollEvent(&sdlEvent) != 0){
		processEvent(sdlEvent);
	}
}

void JWindow::processEvent(SDL_Event sdlEvent){
	InputEvent *event = InputEvent::createInputEvent(sdlEvent, this);
	processEvent(event);
	delete event;
}

void JWindow::processEvent(InputEvent *event){
	// Pass the event to callbacks
	callbackManager.processEvent(event);
	
	
	// Pass the event on to layers for processing.
	std::list<Layer*>::iterator iter;
	for(iter = layers.begin(); iter != layers.end(); iter++){
		Layer *layer = *iter;
		layer->processEvent(event);
	}
	
	
	// Internal Handling of events
	if(event->sdlEvent.type == SDL_QUIT && !event->isConsumed()){
		printf("Quitting?\n");
		dispose();
	}
}


SDL_Surface *JWindow::createNewSurface(){
	/**
	 * Creates an empty surface with the same properties as the window surface.
	 */
	return SDL_CreateRGBSurface(0, screenWidth, screenHeight, 32, 0, 0, 0, 0);
}


/*
 * Property Methods
 */
bool JWindow::isActive(){ return active; }
int JWindow::getScreenWidth(){ return screenWidth;}
int JWindow::getScreenHeight(){ return screenHeight;}
float JWindow::getAspectRatio(){
	return (float) screenWidth / (float) screenHeight;
}

SDL_PixelFormat *JWindow::getFormat(){ return SDL_GetWindowSurface(window)->format;}


/*
 * Coordinate Transforms
 */

void JWindow::viewportToScreen(float xin, float yin, int &xout, int &yout) const {
	xout = std::floor(0.5f * (xin + 1) * screenWidth);
	yout = std::floor(0.5f * (1 - yin) * screenHeight);
	if(xout == screenWidth) xout--;
	if(yout == screenHeight) yout--;
}

void JWindow::screenToViewport(int xin, int yin, float &xout, float &yout) const {
	xout = 2 * ((xin + 0.5f) / (float) screenWidth) - 1;
	yout = 1 - 2 * ((yin + 0.5f) / (float) screenHeight);
}


/*
 * Layer Operations
 */

bool JWindow::registerLayer(Layer *layer){
	if(layer == NULL){
		printf("Cannot add NULL layer.\n");
		return false;
	}
	
	//TODO: Decide if this is really necessary.
	if(layer->getWindow() != this && layer->getWindow() != NULL){
		printf("Cannot add layer \"%s\" to this window. ", layer->id.c_str());
		printf(" It is bound to a different window.\n");
		return false;
	}
	
	// Check to make sure that there is no layer with the same id already.
	if(getLayerById(layer->id) != NULL){
		printf("Cannot add layer \"%s\" to this window. ", layer->id.c_str());
		printf(" A layer with the same ID already exists.\n");
		return false;
	}
	
	layer->setWindow(this);
	return true;
}


void JWindow::addLayerTop(Layer *layer){
	/**
	 * Adds the provided layer to the back of the layer list.  The provided 
	 * layer must be bound to the present window.
	 */
	if(!registerLayer(layer)) return;
	
	layers.push_back(layer);
}

void JWindow::addLayerBottom(Layer *layer){
	/**
	 * Adds the provided layer to the front of the layer list.  The provided 
	 * layer must be bound to the present window.
	 */
	if(!registerLayer(layer)) return;
	
	layers.push_front(layer);
}

void JWindow::addLayerAt(Layer *layer, int position){
	/**
	 * Adds the provided layer to the layer list at the specified position.  The
	 * provided layer must be bound to the present window.
	 */
	if(!registerLayer(layer)) return;
	
	if(layers.size() == 0){
		position = 0;
	}else{
		position = position % layers.size();
	}
	
	std::list<Layer*>::iterator iter = layers.begin();
	std::advance(iter, position);
	layers.insert(iter, layer);
}

void JWindow::removeLayer(Layer *layer){
	/**
	 * Attempts to remove a layer with the same id as the provided layer.  Note
	 * that this step does not call the layer's destructor; that is up to the
	 * user.  Note that calling a layer's destructor automatically calls this,
	 * so that is the preferred way to delete a layer.
	 */
	if(layer == NULL) return;
	
	std::list<Layer*>::iterator iter;
	for(iter = layers.begin(); iter != layers.end(); iter++){
		Layer *test = *iter;
		if(layer->id == test->id){
			layers.erase(iter);
			return;
		}
	}
	
}

Layer *JWindow::getLayerById(std::string id){
	/**
	 * Attempts to retrieve a layer with the specified id.  If no such layer
	 * exists, NULL is returned.
	 */
	
	std::list<Layer*>::iterator iter;
	for(iter = layers.begin(); iter != layers.end(); iter++){
		Layer *layer = *iter;
		if(layer->id == id) return layer;
	}
	
	return NULL;
}

std::list<Layer*> JWindow::getLayers(){
	/**
	 * Returns a list of all layers currently in this window.
	 */
	return layers;
}

