/* 
 * Contains code for some tutorials
 */
#include <SDL2/SDL.h>
#include <stdio.h>

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;


int tutorial_1(){
	printf("Tutorial 1\n");
	
	
	int result = SDL_Init(SDL_INIT_VIDEO);
	if(result < 0) return result;
	
	SDL_Window* window = SDL_CreateWindow("jvisu", SDL_WINDOWPOS_UNDEFINED,
	                     SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
	                     SDL_WINDOW_SHOWN);
	
	if(window == NULL){
		printf(" Window could not be created.  SDL_Error: %s\n", SDL_GetError());
		return -1;
	}
	
	SDL_Surface* screenSurface = SDL_GetWindowSurface(window);
	
	
	// The actual visual tests
	Uint32 backgroundColor = SDL_MapRGB(screenSurface->format, 0xFF, 0xFF, 0xFF);
	SDL_FillRect(screenSurface, NULL, backgroundColor);
	
	SDL_UpdateWindowSurface(window);
	
	
	SDL_Delay(2000);
	SDL_DestroyWindow(window);
	SDL_Quit();
	
	
	return 0;
}
