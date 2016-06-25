/*
 * SDL stuff
 */
#include "sdl.h"


// To keep track of how many windows are active.  If this drops to 0, SDL shuts down.
static int WINDOWS_ACTIVE = 0;
// To keep track of whether or not SDL is presently active.
static bool SDL_ACTIVE = false;

int create_SDL_window(SDL_Window **win, const char *window_name, int sx, int sy){
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

void remove_SDL_window(SDL_Window *window){
	/**
	 * Destroys the provided SDL window and shuts down SDL if there are no windows left.
	 */
	SDL_DestroyWindow(window);
	WINDOWS_ACTIVE--;
	if(WINDOWS_ACTIVE <= 0) SDL_Quit();
}

