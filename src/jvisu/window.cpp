/**
 * Contains the implementation of jvisu windows.
 */
#include <cstdio>
#include <cmath>
#include <list>
#include <string>

#include "tick.h"
#include "sdl.h"
#include "window.h"
#include "layer.h"
#include "callback.h"
#include "input.h"
#include "vectormath.h"



/*
 * Constructors
 */

JWindow::JWindow(int sx, int sy, bool ha, std::string name):
	hardwareAccelerated(ha),
	screenWidth(sx),
	screenHeight(sy),
	active(false),
	pixelFormat(NULL),
	buffer(NULL),
	renderer(NULL), // until activation
	tickRecord(NULL)
{

	windowName = name;
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
	renderer = create_SDL_renderer(window, (int) hardwareAccelerated);
	
	if(result < 0 || renderer == NULL) return -1;

	// Pixel Format for the texture buffer
	pixelFormat = SDL_AllocFormat(SDL_PIXELFORMAT_RGBA8888);
	buffer = SDL_CreateTexture(renderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET, screenWidth, screenHeight);

	// Tick Record for fps calculations
	tickRecord = create_tick_record(10);
	
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
	if (!active){
		printf("Cannot Dispose inactive window.\n");
		return -1;
	}

	// Remove all layers
	while (!layers.empty()){
		Layer *layer = layers.front();
		printf("Deleting Layer \"%s\" from Window \"%s\"\n", layer->id.c_str(),
			windowName.c_str());
		// Note: This automatically removes the layer from the list.
		delete layer;
	}

	if (renderer != NULL){
		SDL_DestroyRenderer(renderer);
		renderer = NULL;
	}

	if (pixelFormat != NULL){
		SDL_FreeFormat(pixelFormat);
		pixelFormat = NULL;
	}

	if (buffer != NULL){
		SDL_DestroyTexture(buffer);
		buffer = NULL;
	}
	
	if(tickRecord != NULL){
		delete_tick_record(tickRecord);
		tickRecord = NULL;
	}
	
	remove_SDL_window(window);
	active = false;
	
	return 0;
}


/*
 * Main Update Methods
 */
void JWindow::update(float tpf){
	/**
	 * This method should be called once per frame.
	 */
	if(!active){
		printf("Updating an inactive JWindow.\n");
		return;
	}
	
	// Note: Processing of input should be the final step of the update cycle,
	//       as this may result in the end of the event.
	processInput(tpf);
	
	if(!active) return;
	
	
	// Update all layers
	std::list<Layer*>::iterator iter;
	for(iter = layers.begin(); iter != layers.end(); iter++){
		Layer *layer = *iter;
		layer->update(tpf);
	}
	
	
	// Draw the Window
	refresh();
}


float JWindow::tick(int target_fps){
	/**
	 * Optional Method to handle frame-rate advancement, measurement, stabilization, etc.
	 */
	return ::tick(target_fps, tickRecord);
}

float JWindow::getFPS() const {
	/**
	 * Measures the FPS of the window tick cycle.  Note:  this only works if the
	 * user is calling tick() on this window every cycle.
	 */
	return compute_tick_record_fps(tickRecord);
}


void JWindow::refresh(){
	/**
	 * This method re-draws everything to the SDL surface.  Although only called
	 * when it is needed, in practice, this method is generally called every frame.
	 */
	
	/* 
	 * First Draw to Buffer Texture
	 */
	SDL_SetRenderTarget(renderer, buffer);

	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
	SDL_RenderClear(renderer);
	
	// Fill in the buffer by rendering the layers in order
	std::list<Layer*>::iterator iter;
	for(iter = layers.begin(); iter != layers.end(); iter++){
		Layer *layer = *iter;
		layer->render(renderer);
	}
	
	
	/*
	 * Actually render the screen now
	 */
	SDL_SetRenderTarget(renderer, NULL);
	
	SDL_SetRenderDrawColor(renderer, 0x00, 0x00, 0x00, 0xff);
	SDL_RenderClear(renderer);

	SDL_RenderCopy(renderer, buffer, NULL, NULL);

	SDL_RenderPresent(renderer);
}


void JWindow::processInput(float tpf){
	/**
	 * Checks for and processes all SDL input events and calls the appropriate
	 * callbacks.
	 */
	SDL_Event sdlEvent;
	
	// Cycle through all active events
	while(SDL_PollEvent(&sdlEvent) != 0){
		processEvent(sdlEvent, tpf);
	}
}

void JWindow::processEvent(SDL_Event sdlEvent, float tpf){
	InputEvent *event = InputEvent::createInputEvent(sdlEvent, this);
	processEvent(event, tpf);
	delete event;
}

void JWindow::processEvent(InputEvent *event, float tpf){
	
	// Pass the event on to layers for processing.
	std::list<Layer*>::reverse_iterator iter;
	// Iterate backwards (reverse order from rendering)
	for(iter = layers.rbegin(); iter != layers.rend(); iter++){
		Layer *layer = *iter;
		layer->processEvent(event, tpf);
	}
	
	
	// Pass the event to callbacks
	callbackManager.processEvent(event, tpf);
	
	
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
bool JWindow::isActive() const { return active; }
int JWindow::getScreenWidth() const { return screenWidth;}
int JWindow::getScreenHeight() const { return screenHeight;}
float JWindow::getAspectRatio() const {
	return (float) screenWidth / (float) screenHeight;
}

SDL_PixelFormat *JWindow::getFormat() const {
	return pixelFormat;
	//return SDL_GetWindowSurface(window)->format;
}
SDL_Renderer *JWindow::getRenderer() {
	return renderer;
}


/*
 * Coordinate Transforms
 */

void JWindow::viewportToScreen(float xin, float yin, int &xout, int &yout) const {
	float ar = getAspectRatio();
	xout = std::floor(0.5f * (xin + ar) * screenWidth / ar);
	yout = std::floor(0.5f * (1 - yin) * screenHeight);
	
	// Just a bit of protection
	/*while(xout < 0) xout++;
	while(yout < 0) yout++;
	while(xout >= screenWidth) xout--;
	while(yout >= screenHeight) yout--;*/
}

void JWindow::screenToViewport(int xin, int yin, float &xout, float &yout) const {
	xout = (2 * ((xin + 0.5f) / (float) screenWidth) - 1) * getAspectRatio();
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


bool JWindow::isKeyPressed(SDL_Keycode keycode){
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);
	SDL_Scancode scancode = SDL_GetScancodeFromKey(keycode);
	return keystate[scancode] != 0;
}

bool JWindow::isLeftMouseButtonPressed(){
	return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT);
}

bool JWindow::isRightMouseButtonPressed(){
	return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT);
}

bool JWindow::isMiddleMouseButtonPressed(){
	return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_MIDDLE);
}

int JWindow::getMouseX(){
	int x;
	SDL_GetMouseState(&x, NULL);
	return x;
}

int JWindow::getMouseY(){
	int y;
	SDL_GetMouseState(NULL, &y);
	return y;
}



