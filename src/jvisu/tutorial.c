/* 
 * Contains code for some tutorials
 */
#include "sdl.h"
#include <stdio.h>

#ifdef _WIN32

#define bool int
#define true 1
#define false 0

#endif


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


/*
 * Tutorial 2
 */
SDL_Window *gWindow = NULL;
SDL_Surface *gScreenSurface = NULL;
SDL_Surface *gImage = NULL;

static bool init(){
	
	int status = SDL_Init(SDL_INIT_VIDEO);
	if(status < 0) return false;
	
	gWindow = SDL_CreateWindow("jvisu", SDL_WINDOWPOS_UNDEFINED,
	                     SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT,
	                     SDL_WINDOW_SHOWN);
	if(gWindow == NULL) return false;
	
	gScreenSurface = SDL_GetWindowSurface(gWindow);
	
	return true;
}

static bool loadMedia(){
	char imageName[] = "assets/test/wood.bmp";
	gImage = SDL_LoadBMP(imageName);
	if(gImage == NULL){
		printf("Unable to load image %s.  SDL Error: %s\n", imageName, SDL_GetError());
		return false;
	}
	return true;
}

static void close(){
	SDL_FreeSurface(gImage);
	gImage = NULL;
	
	SDL_DestroyWindow(gWindow);
	gWindow = NULL;
	
	SDL_Quit();
}


int tutorial_2(){
	printf("Tutorial 2\n");
	
	if(!init()){
		printf("Initialization Failure\n");
		return -1;
	}
	
	if(!loadMedia()){
		printf("Media Loading Failure\n");
		return -1;
	}
	
	SDL_BlitSurface(gImage, NULL, gScreenSurface, NULL);
	
	SDL_UpdateWindowSurface(gWindow);
	
	
	SDL_Delay(2000);
	
	close();
	return 0;
}


/*
 * Tutorial 3
 */

int tutorial_3(){
	printf("Tutorial 3\n");
	
	if(!init()){
		printf("Initialization Failure\n");
		return -1;
	}
	
	if(!loadMedia()){
		printf("Media Loading Failure\n");
		return -1;
	}
	
	SDL_BlitSurface(gImage, NULL, gScreenSurface, NULL);
	SDL_UpdateWindowSurface(gWindow);
	
	
	bool quit = false;
	SDL_Event e;
	/*
	 * Main Loop
	 */
	while(!quit){
		// Cycle through event queue
		while(SDL_PollEvent(&e) != 0){
			if(e.type == SDL_QUIT){
				quit = true;
			}
		}
	}
	
	Uint32 backgroundColor = SDL_MapRGB(gScreenSurface->format, 0x00, 0x00, 0x00);
	SDL_FillRect(gScreenSurface, NULL, backgroundColor);
	SDL_UpdateWindowSurface(gWindow);
	
	
	SDL_Delay(500);
	close();
	
	return 0;
}







































