/**
 * Contains the implementation of jvisu windows.
 */
#include <cstdio>
#include <string>

#include "sdl.h"
#include "window.h"



/*
 * Constructors
 */

JWindow::JWindow(int sx, int sy): screenWidth(sx), screenHeight(sy),
                                  needsRefresh(false), active(false) {
	windowName = "jvisu";
	activate();
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
	
	
	if(needsRefresh) refresh();
	
	
	// Note: Processing of input should be the final step of the update cycle,
	//       as this may result in the end of the event.
	processInput();
}


void JWindow::refresh(){
	/**
	 * This method re-draws everything to the SDL surface.  Although only called
	 * when it is needed, in practice, this method is generally called every frame.
	 */
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


/*
 * Property Methods
 */
bool JWindow::isActive(){ return active; }
int JWindow::getScreenWidth(){ return screenWidth;}
int JWindow::getScreenHeight(){ return screenHeight;}



