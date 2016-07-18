/**
 * Some tests/prototyping using raw SDL (not jvisu)
 */
#ifdef __linux__
#include <unistd.h>
#endif
#ifdef _WIN32
#include <windows.h>
#endif

#include <cstdio>
#include "jvisu.h"


#ifdef __linux__
const bool HARDWARE_ACCELERATION = false;
#endif
#ifdef _WIN32
const bool HARDWARE_ACCELERATION = true;
#endif



volatile bool ACTIVE = true;

SDL_Window *WINDOW = NULL;
SDL_Renderer *RENDERER = NULL;
SDL_Texture *TEXTURE = NULL;

int SCREEN_WIDTH = 800;
int SCREEN_HEIGHT = 600;

float ANGLE = 0;
float DELTA_ANGLE = 1;

float OFFSET_X = 0;
float OFFSET_Y = 0;
float OFFSET_DELTA = 10;

float FACTOR_X = 1;
float FACTOR_Y = 1;
float FACTOR_FACTOR = 1.01f;

int TARGET_X = 0;
int TARGET_Y = 0;
int TARGET_W = 256;
int TARGET_H = 256;



static void clear_screen(){
	SDL_SetRenderDrawColor(RENDERER, 0x00, 0x00, 0x00, 0x00);
	SDL_RenderFillRect(RENDERER, NULL);
}


static void setup(){
	SDL_Init(SDL_INIT_VIDEO);
	IMG_Init(IMG_INIT_PNG);
	
	// Create Window
	WINDOW = SDL_CreateWindow("Demo Raw", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
	                           SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
	
	// Create Renderer
	Uint32 rflags = HARDWARE_ACCELERATION ? SDL_RENDERER_ACCELERATED : SDL_RENDERER_SOFTWARE;
	RENDERER = SDL_CreateRenderer(WINDOW, -1, rflags);
	
	
	//Create Texture
	SDL_Surface *loaded = IMG_Load("assets/test/land.png");
	TEXTURE = SDL_CreateTextureFromSurface(RENDERER, loaded);
	SDL_FreeSurface(loaded);
	
	
	// Fill Window with black
	clear_screen();
}


static void cleanup(){
	
	if(TEXTURE != NULL) SDL_DestroyTexture(TEXTURE);
	if(RENDERER != NULL) SDL_DestroyRenderer(RENDERER);
	if(WINDOW != NULL) SDL_DestroyWindow(WINDOW);
	SDL_Quit();
}


static bool key_pressed(SDL_Keycode keycode){
	const Uint8 *keystate = SDL_GetKeyboardState(NULL);
	SDL_Scancode scancode = SDL_GetScancodeFromKey(keycode);
	return keystate[scancode] != 0;
}


static void refresh(){
	clear_screen();
	
	if(!key_pressed(SDLK_SPACE)){
		SDL_Rect rect;
		rect.x = (int) TARGET_X + OFFSET_X;
		rect.y = (int) TARGET_Y + OFFSET_Y;
		rect.w = (int) TARGET_W * FACTOR_X;
		rect.h = (int) TARGET_H * FACTOR_Y;
		
		SDL_RendererFlip flip = SDL_FLIP_NONE;
		SDL_RenderCopyEx(RENDERER, TEXTURE, NULL, &rect, -ANGLE, NULL, flip);
	}
	
	SDL_RenderPresent(RENDERER);
}





static void update_input(){
	SDL_PumpEvents();
	
	if(key_pressed(SDLK_w)){
		OFFSET_Y += OFFSET_DELTA;
	}
	if(key_pressed(SDLK_s)){
		OFFSET_Y -= OFFSET_DELTA;
	}
	if(key_pressed(SDLK_a)){
		OFFSET_X += OFFSET_DELTA;
	}
	if(key_pressed(SDLK_d)){
		OFFSET_X -= OFFSET_DELTA;
	}
	if(key_pressed(SDLK_p)){
		FACTOR_X *= FACTOR_FACTOR;
		FACTOR_Y *= FACTOR_FACTOR;
	}
	if(key_pressed(SDLK_m)){
		FACTOR_X /= FACTOR_FACTOR;
		FACTOR_Y /= FACTOR_FACTOR;
	}
	if(key_pressed(SDLK_q)){
		ANGLE += DELTA_ANGLE;
	}
	if(key_pressed(SDLK_e)){
		ANGLE -= DELTA_ANGLE;
	}
	
	if(key_pressed(SDLK_ESCAPE)){
		ACTIVE = false;
	}
}



static void tick(){
#ifdef __linux__
	usleep(16*1000);
#endif
#ifdef _WIN32
	Sleep(16);
#endif
}




int main(){
	
	printf("\n\nRaw SDL Demo\n");
	setup();
	
	
	// Main Loop
	while(ACTIVE){
		refresh();
		update_input();
		tick();
	}
	
	
	cleanup();
	printf("\n\n");
	return 0;
}

