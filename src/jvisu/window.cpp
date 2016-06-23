/**
 * Contains the implementation of jvisu windows.
 */
#include <cstdio>
#include <string>

#include <SDL2/SDL.h>
#include "window.h"


/*
 * SDL stuff
 */

// To keep track of how many windows are active.  If this drops to 0, SDL shuts down.
int WINDOWS_ACTIVE = 0;
// To keep track of whether or not SDL is presently active.
bool SDL_ACTIVE = false;

static int create_SDL_window(SDL_Window **win, const char *window_name, int sx, int sy){
	/**
	 * Attempts to create a new SDL window and place a pointer to it in the
	 * provided address.
	 */
	
	// If necessary, initialize SDL
	if(!SDL_ACTIVE){
		if(SDL_Init(SDL_INIT_VIDEO) < 0){
			printf("[Error] Failed to initialize SDL.\n");
			return -1;
		}
		SDL_ACTIVE = true;
	}
	
	// Make a window
	SDL_Window* window = SDL_CreateWindow(window_name, SDL_WINDOWPOS_UNDEFINED,
	                     SDL_WINDOWPOS_UNDEFINED, sx, sy, SDL_WINDOW_SHOWN);
	if(window != NULL) WINDOWS_ACTIVE++;
	else{
		printf("[Error] Failed to create SDL Window.\n");
		return -1;
	}
	
	// Fill it with blackness
	SDL_Surface *surface = SDL_GetWindowSurface(window);
	Uint32 backgroundColor = SDL_MapRGB(surface->format, 0x00, 0x00, 0x00);
	SDL_FillRect(surface, NULL, backgroundColor);
	SDL_UpdateWindowSurface(window);
	
	*win = window;
	return 0;
}

static void remove_SDL_window(SDL_Window *window){
	/**
	 * Destroys the provided SDL window and shuts down SDL if there are no windows left.
	 */
	SDL_DestroyWindow(window);
	WINDOWS_ACTIVE--;
	if(WINDOWS_ACTIVE <= 0) SDL_Quit();
}



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



