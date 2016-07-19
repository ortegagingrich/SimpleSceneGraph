/*
 * SDL stuff
 */
#include <stdio.h>
#include "sdl.h"


// To keep track of how many windows are active.  If this drops to 0, SDL shuts down.
static int WINDOWS_ACTIVE = 0;
// To keep track of whether or not SDL is presently active.
static bool SDL_ACTIVE = false;


/*
 * Setup/Takedown Methods
 */


static int init(){
	/**
	 * Initializes SDL and returns the result;
	 */
	
	if(SDL_Init(SDL_INIT_VIDEO) < 0) return -1;
	
	// For PNG loading
	if(!(IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG)) return -1;
	
	return 0;
}



int create_SDL_window(SDL_Window **win, const char *window_name, int sx, int sy){
	/**
	 * Attempts to create a new SDL window and place a pointer to it in the
	 * provided address.
	 */
	
	// If necessary, initialize SDL
	if(!SDL_ACTIVE){
		if(init() < 0){
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
	if(window == NULL){
		printf("[Error] Cannot destroy NULL window.\n");
		return;
	}
	SDL_DestroyWindow(window);
	WINDOWS_ACTIVE--;
	if(WINDOWS_ACTIVE <= 0) SDL_Quit();
}


SDL_Renderer *create_SDL_renderer(SDL_Window *window, int hardware_accelerated){
	/**
	 * Attempts to create an SDL renderer.
	 */
	Uint32 flags = hardware_accelerated ? SDL_RENDERER_ACCELERATED : SDL_RENDERER_SOFTWARE;
	return SDL_CreateRenderer(window, -1, flags);
}



/*
 * SDL Wrapper methods
 */

int render_copy_clip(
	SDL_Renderer *renderer,
	SDL_Texture *texture,
	const SDL_Rect *srcrect,
	const SDL_Rect *dstrect,
	double angle
){
	/*
	 * First, determine if clipping is really necessary
	 */
	
	SDL_Rect drect;
	drect.x = 0;
	drect.y = 0;
	drect.w = 1000;
	drect.h = 1000;
	drect = *dstrect;
	
	/*
	 * No Clipping necessary, so just use the SDL function
	 */
	SDL_Point center;
	center.x = 0;
	center.y = 0;
	SDL_RendererFlip flip = SDL_FLIP_NONE;
	
	return SDL_RenderCopyEx(renderer, texture, srcrect, &drect, angle, &center, flip);
}



































void print_SDL_error(){
	printf("SDL ERROR: %s\n", SDL_GetError());
}

