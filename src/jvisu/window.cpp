/**
 * Contains the implementation of jvisu windows.
 */
#include <cstdio>
#include <string>

#include "sdl.h"
#include "window.h"
#include "scene_graph.h"



/*
 * Constructors
 */

JWindow::JWindow(int sx, int sy): screenWidth(sx), screenHeight(sy), active(false) {
	windowName = "jvisu";
	activate();
	
	// Add a background layer
	
	LayerBackground *background = new LayerBackground(this, "Background", 0xff, 0x00, 0x00);
	layers.push_back(background);
	
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
	dispose();
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
		delete layer;
		layers.pop_front();
	}
	
	if(buffer != NULL){
		SDL_FreeSurface(buffer);
		buffer = NULL;
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
	
	// Reset or Create buffer
	if(buffer == NULL){
		buffer = createNewSurface();
	}else{
		SDL_FillRect(buffer, NULL, SDL_MapRGB(buffer->format, 0x00, 0x00, 0x00));
	}
	
	
	// Fill in the buffer by rendering the layers in order
	std::list<Layer*>::iterator iter;
	for(iter = layers.begin(); iter != layers.end(); iter++){
		Layer *layer = *iter;
		layer->render(buffer);
	}
	
	
	// Copy the buffer to the actual surface
	SDL_BlitSurface(buffer, NULL, SDL_GetWindowSurface(window), NULL);
	
	//Update the window surface
	SDL_UpdateWindowSurface(window);
}


void JWindow::processInput(){
	/**
	 * Checks for and processes all SDL input events and calls the appropriate
	 * callbacks.
	 */
	SDL_Event event;
	
	// Cycle through all active events
	while(SDL_PollEvent(&event) != 0){
		
		// Internal Handling of events
		if(event.type == SDL_QUIT){
			printf("Quitting?\n");
			dispose();
		}
		
		
		// Pass the event to callbacks
		callbackManager.processEvent(event);
		
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
SDL_PixelFormat *JWindow::getFormat(){ return SDL_GetWindowSurface(window)->format;}



