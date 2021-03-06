/**
 * Contains the implementation of ssg windows.
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

using namespace ssg;



/*
 * Constructors
 */

Window::Window(int sx, int sy, bool ha, std::string name):
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


int Window::activate(){
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
Window::~Window(){
	if(active) dispose();
}


int Window::dispose(){
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
void ssg::Window::update(float tpf){
	/**
	 * This method should be called once per frame.  It takes care of all updating
	 * of layers/scene graphs/components/input/callbacks, etc.  It also redraws
	 * the screen.
	 * 
	 * @param tpf the time (in seconds) since the last frame.
	 */
	if(!active){
		printf("Updating an inactive Window.\n");
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


float ssg::Window::tick(int target_fps){
	/**
	 * Optional Method to handle frame-rate advancement, measurement, stabilization, etc.
	 * 
	 * If the user wishes to have the ssg window maintain a set framerate, this
	 * method should be called once for every time Window::update() is called.
	 * 
	 * @param target_fps the target framerate
	 * @return the actual time (in seconds) taken by the last frame (i.e. since
	 * the last time that Window::tick() was called).
	 */
	return ::tick(target_fps, tickRecord);
}

float ssg::Window::getFPS() const {
	/**
	 * Measures the FPS of the window tick cycle.  Note:  this only works if the
	 * user is calling tick() on this window every cycle.
	 * 
	 * @return a float containing the approximate frame rate in hertz.
	 */
	return compute_tick_record_fps(tickRecord);
}


void ssg::Window::refresh(){
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


void ssg::Window::processInput(float tpf){
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

void Window::processEvent(SDL_Event sdlEvent, float tpf){
	/**
	 * Internal Method: Force the window to process the provided pseudo-input event.
	 * This method is called internally from the primary update routine and is
	 * only exposed for the purposes of white-box testing.
	 * 
	 * @param sdlEvent the SDL event struct representing the spoofed event
	 * @param tpf the time, in seconds, since the last frame
	 */
	InputEvent *event = InputEvent::createInputEvent(sdlEvent, this);
	processEvent(event, tpf);
	delete event;
}

void Window::processEvent(InputEvent *event, float tpf){
	/**
	 * Internal Method: Force the window to process the provided pseudo-input event.
	 * This method is called internally from the primary update routine and is
	 * only exposed for the purposes of white-box testing.
	 * 
	 * @param event the input event object
	 * @param tpf the time, in seconds, since the last frame
	 */
	
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


SDL_Surface *Window::createNewSurface(){
	/*
	 * Creates an empty surface with the same properties as the window surface.
	 */
	return SDL_CreateRGBSurface(0, screenWidth, screenHeight, 32, 0, 0, 0, 0);
}


/*
 * Property Methods
 */
bool Window::isActive() const { 
	/**
	 * Checks to see if the window is currently active.
	 * 
	 * @return a boolean specifying whether or not the window is active
	 */
	return active;
}

int Window::getScreenWidth() const {
	/**
	 * Returns the width of the window's screen in pixels.
	 * 
	 * @return an integer representing the window width
	 */
	return screenWidth;
}

int Window::getScreenHeight() const {
	/**
	 * Returns the height of the window's screen in pixels.
	 * 
	 * @return an integer representing the window height
	 */
	return screenHeight;
}

float Window::getAspectRatio() const {
	/**
	 * Returns the window screen's aspect ratio.  The aspect ratio is defined as
	 * the screen width divided by the screen height.
	 * 
	 * @return a float containing the aspect ratio
	 */
	return (float) screenWidth / (float) screenHeight;
}

SDL_PixelFormat *Window::getFormat() const {
	return pixelFormat;
	//return SDL_GetWindowSurface(window)->format;
}
SDL_Renderer *Window::getRenderer() {
	return renderer;
}


/*
 * Coordinate Transforms
 */

void ssg::Window::viewportToScreen(float xin, float yin, int &xout, int &yout) const {
	/**
	 * Computes the transformation of coordinates from viewport coordinates to 
	 * screen coordinates.
	 * 
	 * Screen Coordinates are the same as in SDL.  They are integers representing
	 * pixels.  The top-left corner of the window is the origin.  The positive
	 * x-direction is to the right and the positive y-direction is downward.
	 * 
	 * Viewport Coordinates are floats independent of pixels.  The origin is at
	 * the center of the screen/window.  The positive x-direction is to the right
	 * and the positive y-direction is upward.  The top and bottom of the screen/
	 * window are at y = 1.0f and y = -1.0f, respectively.  The sides of the screen are
	 * scaled appropriately to maintain the aspect ratio.  For example, if the
	 * window has an aspect ratio of 2:1, the left edge of the screen will be
	 * x = -2.0f and the right edge will be x = 2.0f.
	 * 
	 * @param xin the input x-coordinate (viewport coordinates)
	 * @param yin the input y-coordinate (viewport coordinates)
	 * @param xout an int where the output x (screen) coordinate will be stored
	 * @param yout an int where the output y (screen) coordinate will be stored
	 */
	
	float ar = getAspectRatio();
	xout = std::floor(0.5f * (xin + ar) * screenWidth / ar);
	yout = std::floor(0.5f * (1 - yin) * screenHeight);
	
	// Just a bit of protection
	/*while(xout < 0) xout++;
	while(yout < 0) yout++;
	while(xout >= screenWidth) xout--;
	while(yout >= screenHeight) yout--;*/
}

void ssg::Window::screenToViewport(int xin, int yin, float &xout, float &yout) const {
	/**
	 * Computes the transformation of coordinates from screen coordinates to 
	 * viewport coordinates.
	 * 
	 * Screen Coordinates are the same as in SDL.  They are integers representing
	 * pixels.  The top-left corner of the window is the origin.  The positive
	 * x-direction is to the right and the positive y-direction is downward.
	 * 
	 * Viewport Coordinates are floats independent of pixels.  The origin is at
	 * the center of the screen/window.  The positive x-direction is to the right
	 * and the positive y-direction is upward.  The top and bottom of the screen/
	 * window are at y = 1.0f and y = -1.0f, respectively.  The sides of the screen are
	 * scaled appropriately to maintain the aspect ratio.  For example, if the
	 * window has an aspect ratio of 2:1, the left edge of the screen will be
	 * x = -2.0f and the right edge will be x = 2.0f.
	 * 
	 * @param xin the input (screen) x-coordinate
	 * @param yin the input (screen) y-coordinate
	 * @param xout a float where the output (viewport) x-coordinate will be stored
	 * @param yout a float where the output (viewport) y-coordinate will be stored
	 */
	
	xout = (2 * ((xin + 0.5f) / (float) screenWidth) - 1) * getAspectRatio();
	yout = 1 - 2 * ((yin + 0.5f) / (float) screenHeight);
}


/*
 * Layer Operations
 */

bool Window::registerLayer(Layer *layer){
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


void Window::addLayerTop(Layer *layer){
	/**
	 * Attempts to add the provided layer to the top of the window's layer list.
	 * The new layer will appear on top of all existing layers.
	 * 
	 * @param layer a pointer to the layer to be added to the window
	 */
	if(!registerLayer(layer)) return;
	
	layers.push_back(layer);
}

void Window::addLayerBottom(Layer *layer){
	/**
	 * Attempts to add the provided layer to the bottom of the window's layer list.
	 * The new layer will appear underneath all existing layers.
	 * 
	 * @param layer a pointer to the layer to be added to the window
	 */
	if(!registerLayer(layer)) return;
	
	layers.push_front(layer);
}

void Window::addLayerAt(Layer *layer, int position){
	/**
	 * Attempts to add the provided layer to the window's layer list at the
	 * provided position.
	 * 
	 * @param layer a pointer to the layer to be added to the window
	 * @param position the position at which the layer is to be inserted.  This
	 * is counted from the bottom up, so position = 0 corresponds to inserting a
	 * layer at the bottom.
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

void Window::removeLayer(Layer *layer){
	/**
	 * Attempts to remove a layer with the same id as the provided layer.  Note
	 * that this step does not call the layer's destructor; that is the responsibility
	 * of the user.  Note that calling a layer's destructor automatically calls this,
	 * so that is the preferred way to delete a layer.
	 * 
	 * @param layer a pointer to the layer to remove from the window
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

Layer *Window::getLayerById(std::string id){
	/**
	 * Attempts to retrieve a layer with the specified id.
	 * 
	 * @param id a string containing the desired layer's id.
	 * @return the layer assigned to this window with the provided id, provided
	 * that such a layer exists.  If no such layer exists, NULL is returned.
	 */
	
	std::list<Layer*>::iterator iter;
	for(iter = layers.begin(); iter != layers.end(); iter++){
		Layer *layer = *iter;
		if(layer->id == id) return layer;
	}
	
	return NULL;
}

std::list<Layer*> Window::getLayers(){
	/**
	 * @return a list of all layers currently in this window.
	 */
	return layers;
}


bool Window::isKeyPressed(SDL_Keycode keycode){
	/**
	 * Checks to see if the input key corresponding to the provided SDL2 keycode
	 * is currently being pressed.
	 * 
	 * @param keycode the SDL2 keycode corresponding to the key of interest
	 * @return a boolean indicating whether or not the key is pressed.
	 */
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);
	SDL_Scancode scancode = SDL_GetScancodeFromKey(keycode);
	return keystate[scancode] != 0;
}

bool Window::isLeftMouseButtonPressed(){
	/**
	 * Checks to see if the left mouse button is currently being pressed.
	 * 
	 * @return a boolean indicating whether or not the left mouse button is
	 * currently pressed.
	 */
	return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_LEFT);
}

bool Window::isRightMouseButtonPressed(){
	/**
	 * Checks to see if the right mouse button is currently being pressed.
	 * 
	 * @return a boolean indicating whether or not the right mouse button is
	 * currently pressed.
	 */
	return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_RIGHT);
}

bool Window::isMiddleMouseButtonPressed(){
	/**
	 * Checks to see if the middle mouse button is currently being pressed.
	 * 
	 * @return a boolean indicating whether or not the middle mouse button is
	 * currently pressed.
	 */
	return SDL_GetMouseState(NULL, NULL) & SDL_BUTTON(SDL_BUTTON_MIDDLE);
}

int Window::getMouseX(){
	/**
	 * Obtains the x-component of the mouse cursor's screen coordinates.
	 * 
	 * @return an int containing the (screen) x-coordinate of the mouse cursor.
	 */
	int x;
	SDL_GetMouseState(&x, NULL);
	return x;
}

int Window::getMouseY(){
	/**
	 * Obtains the y-component of the mouse cursor's screen coordinates.
	 * 
	 * @return an int containing the (screen) y-coordinate of the mouse cursor.
	 */
	int y;
	SDL_GetMouseState(NULL, &y);
	return y;
}



